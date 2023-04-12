#include "../httpheader.h"
#include "../threadpool/threadpool.h"

/**
 * @brief 路由存储数据类型
 * 1.key为路由访问路径
 * 2.对应需要执行的函数逻辑，存储该函数的地址，传入Header对象
*/
typedef std::unordered_map<std::string,std::function<void(Header)>> RouterUrl;

class Router{
public:
    //添加请求地址
    void AddUrl(std::string url,std::function<void(Header)> p);
    void Post(std::string url,std::function<void(Header)> p);
    void Get(std::string url,std::function<void(Header)> p);
    void CallRequest(Header&);
    void StartThreadPool(ThreadPool* p){
        pool=p;
        use_thread= true;
    }
    static Router* AcquireRouter();

private:
    Router(){}
    RouterUrl router;
    RouterUrl PostRouter;
    RouterUrl GetRouter;

    ThreadPool* pool;
    bool use_thread= false;
};

Router* Router::AcquireRouter(){
    static Router router;
    return &router;
}

//添加GET请求
void Router::Get(std::string url, std::function<void(Header)> p) {
    GetRouter[url]=p;
}

//添加POST请求
void Router::Post(std::string url, std::function<void(Header)> p) {
    PostRouter[url]=p;
}


//添加请求
void Router::AddUrl(std::string url, std::function<void(Header)> p) {
    router[url]=p;
}

//key为url路径，value为调用函数
void Router::CallRequest(Header& header) {
    if(use_thread) {
        if (header.HttpRequestWay=="GET"||header.HttpRequestWay=="get") {
            if(GetRouter.count(header.HttpRequestUrl) == 1){
                pool->submit(GetRouter[header.HttpRequestUrl],header);
            }else{
                std::cout<<"router error"<<std::endl;
                close(header.serverID);
            }
        }else if(header.HttpRequestWay=="POST"||header.HttpRequestWay=="post"){
            if(PostRouter.count(header.HttpRequestUrl) == 1){
                pool->submit(PostRouter[header.HttpRequestUrl],header);
            }else{
                std::cout<<"router error"<<std::endl;
                close(header.serverID);
            }
        }
    } else{
        if (header.HttpRequestWay=="GET"||header.HttpRequestWay=="get") {
            if(GetRouter.count(header.HttpRequestUrl) == 1){
                pool->submit(GetRouter[header.HttpRequestUrl],header);
            }else{
                std::cout<<"router error"<<std::endl;
                close(header.serverID);
            }
        }else if(header.HttpRequestWay=="POST"||header.HttpRequestWay=="post"){
            if(PostRouter.count(header.HttpRequestUrl) == 1){
                pool->submit(PostRouter[header.HttpRequestUrl],header);
            }else{
                std::cout<<"router error"<<std::endl;
                close(header.serverID);
            }
        }
    }
}


