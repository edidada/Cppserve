#include <string>
#include<cstring>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <ctype.h>
#include<functional>
#include <unistd.h>
#include <mutex>
#include<deque>
#include <sys/socket.h>

/**
 * @brief 数据库结果存储数据类型
 * 1.key为字段
 * 2.value为容器，存储该字段的数据列表
*/
typedef  std::unordered_map<std::string,std::vector<std::string>> SQLDATA;



