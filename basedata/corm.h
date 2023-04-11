#include "../Cmdoule.h"
#include <mysql/mysql.h>




class Cmysql
{
private:
    MYSQL mysql_conn;

    // 数据库连接句柄
    MYSQL *DB;

public:
    struct InsertModel
    {
        std::string Value; // 值
        std::string Key;   // 字段名
    };

    // 保存获取的结果集
    SQLDATA ResultData;
    Cmysql();
    void InitMysql(const char *address, const char *user, const char *password, const char *DbName, int port);

    void ConditionSearch(std::string TableName, std::string Condition, std::string Vaule, ...);
    void CloseMysql();
    void GetResult();
    void OutResultTerminal();
    void InsertData(std::string TableName, std::unordered_map<std::string, std::string> in);
};

class CmysqlPool
{
public:
    int n;                           // 数据库连接池数量
    //std::deque<std::string> TaskSql; // 数据库执行任务队列

    SQLDATA SubmitTask(std::string sql);
    void InitCmysqlPool(int num, const char *address, const char *user, const char *password, const char *DbName, int port);

private:
    std::deque<MYSQL *> DBList; // 存放数据库连接池数据库句柄
    std::mutex DBList_lock;
    MYSQL *GetMysqlDB();

    SQLDATA GetResultPool(MYSQL *handle);
};

