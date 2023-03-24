#include "../Cmdoule.h"
#include <mysql/mysql.h>




class Cmysql
{
private:
    MYSQL mysql_conn;

    std::mutex DBList_lock;
    //数据库连接句柄
    MYSQL *DB;
    int n;  //数据库连接池数量
    std::deque<std::string> TaskSql;

    std::deque<MYSQL*> DBList;    //存放数据库句柄 

    bool PooLock=false;         //连接池是否开启

public:
    
    struct InsertModel
    {
        std::string Value;  //值
        std::string Key;    //字段名
    };
    
    //保存获取的结果集
   SQLDATA ResultData;
    Cmysql();
    Cmysql(int n);
    void SqlPool();
    void InitMysql(const char *address, const char *user, const char *password, const char *DbName, int port);
    void ConditionSearch(std::string TableName, std::string Condition, std::string Vaule, ...);
    void CloseMysql();
    void GetResult();
    void OutResultTerminal();
    void InsertData(std::string TableName,std::unordered_map<std::string,std::string> in);
    MYSQL* GetMysqlDB();
    SQLDATA GetResultPool(MYSQL* handle);

    SQLDATA SubmitTask(std::string sql);
};


