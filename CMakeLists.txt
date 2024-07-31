cmake_minimum_required(VERSION 3.20)
project(web)


set(CMAKE_CXX_STANDARD 23)

#设置头文件的搜索路径
include_directories(./basedata)

include_directories(./http ./str ./rpc)

add_definitions("-Wall -g")

aux_source_directory(./http/ HTTP)

aux_source_directory(./basedata/src SQL)

aux_source_directory(./rpc/src RPC)

add_executable(web test/main.cpp ${HTTP} ${SQL} ${RPC})

target_link_libraries(web libmysqlclient.so pthread)

