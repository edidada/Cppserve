#include "../Cmdoule.h"

#define MAX_EVENTS 1024 // 监听上限数
#define BUFLEN 4096     // 缓冲区大小

// 查找键值对
void FindKeyValue(HTTP_HEADER &Te, std::string s, int start, std::string Key);

// 查找Json
void FindJson(std::string &json, std::string s, int start);

struct myevent_s
{
    int fd;                                           // 要监听的文件描述符
    int events;                                       // 对应的监听事件
    void *arg;                                        // 泛型参数
    void (*call_back)(int fd, int events, void *arg); // 回调函数
    int status;                                       // 是否在监听:1->在红黑树上(监听), 0->不在(不监听)
    char buf[BUFLEN];
    int len;
    long last_active; // 记录每次加入红黑树 g_efd 的时间值
};

class Header
{
public:
    std::string Http;
    std::string HttpRequestWay;
    std::string HttpRequestUrl;
    HTTP_HEADER HttpUserAgent;
    HTTP_HEADER HttpAccept;
    HTTP_HEADER HttpConnection;
    HTTP_HEADER HttpAcceptEncoding;
    HTTP_HEADER HttpAcceptLanguage;
    HTTP_HEADER HttpCookie;
    std::string HttpBody;
    void StrChangeChar(std::string, char[]);          // 字符串转char类型
    int SendRequestHeader(int code, std::string msg); // 封装响应头部
    void SerializationHeader(char res[]);             // 反序列化请求头
    int serverID;
    myevent_s *ev;
    int g_efd;

private:
    std::string HTTP_VERSION = HTTP_1_1;
    const std::string Header = "HTTP/1.1 200 ok\r\n";
    const std::string ContentTypeJson = "content-type: application/json;charset=utf-8\r\n";
    const std::string connection = "connection: close\r\n\r\n";
};

// string类型转char类型
void Header::StrChangeChar(std::string str, char msg[])
{
    strcpy(msg, str.c_str());
}

/**
 * @brief 发送http响应结果
 * @param code 发送的http请求状态码
 * @param msg 发送的http请求json字符串
 */

int Header::SendRequestHeader(int code, std::string msg)
{
    std::string head;
    int i=code%100;

    //判断状态码
    if (code/100 == 2)
    {
        head = HTTP_VERSION + EMPTY + HTTP_STATUS[i]+ ENTER_WRAP + ContentTypeJson + connection;
    }else if(code/100==3){
        head = HTTP_VERSION + EMPTY + HTTP_STATUS[HTTP_CODE_3XX+i]+ ENTER_WRAP + ContentTypeJson + connection;
    }else if(code/100==4){
        head = HTTP_VERSION + EMPTY + HTTP_STATUS[HTTP_CODE_4XX+i]+ ENTER_WRAP + ContentTypeJson + connection;
    }

    char res[1024];
    StrChangeChar(head + msg, res);
    int n = send(serverID, res, strlen(res), 0);
    ColorCout_256<std::string>(HttpRequestWay, 231, 32);
    ColorCout_256<std::string>(HttpRequestUrl, 231, 170);
    if (code == 200)
    {
        ColorCout_256<int>(code, 231, 2);
    }
    else
    {
        ColorCout_256<int>(code, 231, 1);
    }
    printf("\n");
    close(serverID);
    return n;
}

// 解析请求头
void Header::SerializationHeader(char res[])
{
    int start = 0;
    std::string str(res);
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == ' ')
        {
            HttpRequestWay.assign(str, 0, i);
            break;
        }
    }
    int lock = 0;
    for (int i = HttpRequestWay.size() - 1; i < str.size(); ++i)
    {
        if (str[i] == '/')
        {
            lock = 1;
            start = i;
        }
        if (lock)
        {
            if (str[i] == ' ')
            {
                HttpRequestUrl.assign(str, start + 1, i - 1 - start);
                for (int j = i + 1; j < str.size(); j++)
                {
                    if (str[j] == '\r')
                    {
                        Http.assign(str, i + 1, j - i);
                        break;
                    }
                }
                break;
            }
        }
    }
    int l = HttpRequestWay.size() + HttpRequestUrl.size() + Http.size();
    // 序列化头部键值对
    int conn = str.find("Connection", l);
    int ua = str.find("User-Agent", l);
    int acc = str.find("Accept", l);
    int ae = str.find("Accept-Encoding", l);
    int al = str.find("Accept-Language", l);
    int coo = str.find("Cookie", l);
    FindKeyValue(HttpConnection, str, conn, "Connection");
    FindKeyValue(HttpAccept, str, acc, "Accept");
    FindKeyValue(HttpUserAgent, str, ua, "User-Agent");
    FindKeyValue(HttpAcceptEncoding, str, ae, "Accept-Encoding");
    FindKeyValue(HttpAcceptLanguage, str, al, "Accept-Language");
    FindKeyValue(HttpCookie, str, coo, "Cookie");
    FindJson(HttpBody, str, coo);
}

// 查找键值对
void FindKeyValue(HTTP_HEADER &Te, std::string s, int start, std::string Key)
{
    int lock = 0, begin = 0;
    for (int i = start + Key.size(); i < s.size(); i++)
    {
        if (s[i] == ' ' && !lock)
        {
            lock = 1;
            begin = i;
        }
        if (lock)
        {
            if (s[i] == '\r')
            {
                Te[Key].assign(s, begin + 1, i - begin);
                break;
            }
        }
    }
}

void FindJson(std::string &json, std::string s, int start)
{
    if (s.find("{", start) == std::string::npos && s.rfind("}", s.size()) == std::string::npos)
    {
        return;
    }
    int l = s.find("{", start);
    int b = s.rfind("}", s.size());
    json.assign(s, l, b - l + 1);
    // std::cout<<"json:"<<json<<std::endl;
}
