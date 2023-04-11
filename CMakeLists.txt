cmake_minimum_required(VERSION 3.20)
project(web)


set(CMAKE_CXX_STANDARD 23)

add_definitions("-Wall -g")

aux_source_directory(./http/ HTTP)

aux_source_directory(./basedata/src SQL)

add_executable(web test/main.cpp ${HTTP} ${SQL})

target_link_libraries(web libmysqlclient.so)


