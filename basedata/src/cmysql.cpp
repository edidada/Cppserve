#include "corm.h"

const std::string comma = ",";

const std::string INSTERT_SQL = "insert into";
const std::string VALUES_SQL = "values";



void CmysqlPool::InitCmysqlPool(int num, const char *address, const char *user, const char *password, const char *DbName, int port = 3306)
{

    n = num;
    for (int i = 0; i < n; i++)
    {
        MYSQL *con = NULL;
        con = mysql_init(con);

        if (con == NULL)
        {
            exit(1);
        }
        mysql_real_connect(con, address, user, password, DbName, port, NULL, 0);

        if (con == NULL)
        {
            exit(1);
        }
        DBList.push_back(con);
    }
}

MYSQL *CmysqlPool::GetMysqlDB()
{
    // 判断队列里面是否有DB
    std::lock_guard<std::mutex> lock(DBList_lock);
    if (!DBList.empty())
    {

        auto tmp = DBList.front();
        DBList.pop_front();
        return tmp;
    }

    return nullptr;
}

SQLDATA CmysqlPool::GetResultPool(MYSQL *handle)
{
    SQLDATA data;
    // 获取结果集
    MYSQL_RES *res = mysql_store_result(handle);

    // 获取结果集行数
    int num = mysql_num_rows(res);

    // 获取结果集列数
    int field = mysql_field_count(handle);

    // 获取结果集列字段属性
    MYSQL_FIELD *ValueName;
    ValueName = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        // 取出一行记录
        MYSQL_ROW r = mysql_fetch_row(res);
        for (int j = 0; j < field; ++j)
        {
            data[ValueName[j].name].push_back(r[j]);
        }
    }
    std::lock_guard<std::mutex> lock(DBList_lock);
    DBList.emplace_back(handle);
    return data;
}


/**
* @brief 提交执行的sql语句任务并获取结果
* @param sql 执行的sql语句字符串
* @return 返回结果集
*/
SQLDATA CmysqlPool::SubmitTask(std::string sql)
{
    SQLDATA data;
    MYSQL *DBPool = nullptr;
    do
    {
        DBPool = GetMysqlDB();
        if (DBPool != nullptr)
        {
            int k = mysql_query(DBPool, sql.c_str());
            if (k != 0)
            {
                printf("SubmitTask:%s执行失败\n", sql.c_str());
                return data;
            }
            return GetResultPool(DBPool);
        }
    } while (!DBPool);
    return data;
}


CmysqlPool* CmysqlPool::AcquireCmysqlPool(){
    static CmysqlPool myPool;
    return &myPool;
}

Cmysql::Cmysql()
{

    DB = mysql_init(&mysql_conn);
}

/**
* @brief 初始化数据库连接
* @param address 数据库连接地址
* @param user 用户名
* @param password 密码
* @param DbName 连接的数据库名
* @param port:连接数据库端口
*/
void Cmysql::InitMysql(const char *address, const char *user, const char *password, const char *DbName, int port)
{

    if (DB == NULL)
    {
        printf("mysql init err\n");
        exit(1);
    }
    // 等价&mysql_conn,"127.0.0.1"...
    if (mysql_real_connect(DB, address, user, password, DbName, 3306, NULL, 0) == NULL)
    {
        printf("connect err\n");
        exit(1);
    }
    printf("Mysql 连接成功\n");
}


/**
 * @brief 根据条件搜索数据内容
 * @param Mysql 初始化数据库返回的数据库句柄
 * @param TableName 表名
 * @param Condition 条件
 * @param Vaule 值
 * @param ... 需要获得的字段
 *例如,ConditionSearch("user", "age>=", "15", "name", "age");对应sql语句：select name,age from user where age>=15;
 */
void Cmysql::ConditionSearch(std::string TableName, std::string Condition, std::string Vaule, ...)
{
    std::string sql = "select" + EMPTY;
    va_list v;   // 定义va_list
    char *value; // 用于获取值

    va_start(v, Vaule); // 使v跟可变参数表绑定
    int n = sizeof(v) / (sizeof(char *));
    n--;
    while (n--)
    {
        value = va_arg(v, char *);
        sql += value + comma;
    }
    sql.pop_back();
    sql += EMPTY + "from" + EMPTY + TableName + EMPTY + "where" + EMPTY + Condition + Vaule;
    va_end(v); // 结束参数的获取
    const char *q = sql.c_str();
    // const char* q="select name,age from user where age>=15;";
    int k = mysql_query(DB, q);
    if (k != 0)
    {
        printf("查找失败\n");
        return;
    }
}

// 关闭数据库连接
void Cmysql::CloseMysql()
{
    mysql_close(DB);
}

// 获取结果集数据
void Cmysql::GetResult()
{
    SQLDATA ResultData;
    // 获取结果集
    MYSQL_RES *res = mysql_store_result(DB);

    // 获取结果集行数
    int num = mysql_num_rows(res);

    // 获取结果集列数
    int field = mysql_field_count(DB);

    // 获取结果集列字段属性
    MYSQL_FIELD *ValueName;
    ValueName = mysql_fetch_fields(res);
    for (int i = 0; i < num; ++i)
    {
        // 取出一行记录
        MYSQL_ROW r = mysql_fetch_row(res);
        for (int j = 0; j < field; ++j)
        {
            ResultData[ValueName[j].name].push_back(r[j]);
        }
    }
}

// 输出结果集数据到终端
void Cmysql::OutResultTerminal()
{
    for (auto x : ResultData)
    {
        printf("%s:", x.first.c_str());
        for (auto k : x.second)
        {
            printf("%s ", k.c_str());
        }
        printf("\n");
    }
}

void Cmysql::InsertData(std::string TableName, std::unordered_map<std::string, std::string> in)
{
    std::string tmp1, tmp2;
    for (auto k : in)
    {
        tmp1 += k.first + comma;
        tmp2 += "\"" + k.second + "\"" + comma;
    }
    // 去除最后多余逗号
    tmp1.pop_back();
    tmp2.pop_back();

    std::string sql = INSTERT_SQL + EMPTY + TableName + EMPTY + "(" + tmp1 + ")" + EMPTY + VALUES_SQL + EMPTY + "(" + tmp2 + ")";
    int k = mysql_query(DB, sql.c_str());
    if (k != 0)
    {
        printf("插入失败\n");
        return;
    }
}


