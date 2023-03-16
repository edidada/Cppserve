#include <iostream>


void test(Header hea){
    ChJson te;
    te["测试"]="成功";
    te["测试2"]="哈哈哈";
    std::string st= Json(te);

    hea.SendRequestHeader(200,st);
}

void login(Header hea){
    ChJson te;
    te["测试"]="成功";
    std::string st= Json(te);
    Cmysql c;

    //连接数据库相关
    c.InitMysql("127.0.0.1", "root", "131420", "test", 3306);
    c.ConditionSearch("user", "age>=", "15", "name", "age");
    c.GetResult();
    
    std::unordered_map<std::string,std::string> tmp={
        {"name","测试"},
        {"age","33"},
        {"number","11445566"},
        {"ID","8"}
    };
    c.InsertData("user",tmp);

    c.CloseMysql();


    hea.SendRequestHeader(200,st);
}
void fin(Header hea){
    ChJson te;
    te["查找"]="成功";
    std::string st= Json(te);
    hea.SendRequestHeader(200,st);
}