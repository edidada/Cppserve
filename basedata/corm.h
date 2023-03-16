#include "../Cmdoule.h"
#include <mysql/mysql.h>


class Cmysql
{
private:
    MYSQL mysql_conn;
    //数据库连接句柄
    MYSQL *DB;
public:
    
    struct InsertModel
    {
        std::string Value;  //值
        std::string Key;    //字段名
    };
    
    //保存获取的结果集
    std::unordered_map<std::string,std::vector<std::string>> ResultData;
    Cmysql();

    
    void InitMysql(const char *address, const char *user, const char *password, const char *DbName, int port);
    void ConditionSearch(std::string TableName, std::string Condition, std::string Vaule, ...);
    void CloseMysql();
    void GetResult();
    void OutResultTerminal();
    void InsertData(std::string TableName,std::unordered_map<std::string,std::string> in);
};


