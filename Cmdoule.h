#ifndef _NAME_H
#define _NAME_H


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



/**
 *  @brief http请求头部键值对
 * 1.key为键名
 * 2.value为值
*/
typedef std::unordered_map<std::string,std::string> HTTP_HEADER;


template<typename T>	//T表示任何可以被cout输出的类型
void ColorCout_256(T t, int ForeColor, int BackColor)
{
	//ForeColor和BackColor的取值区间为[0,255]
	std::cout << "\033[" << "38;5;" << ForeColor << "m";
	std::cout << "\033[" << "48;5;" << BackColor << "m";
	std::cout << t << "\033[m ";
}

static void InitPrint(){
	const int res[][65]={
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,1,1},
    {0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,0},
    {0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0}
};

	int index=2;
	for(int i=0;i<6;i++){
		for(int j=0;j<65;j++){
			if(res[i][j]==1){
				std::cout << "\033[" << "38;5;" << index++ << "m";
				std::cout << "\\" << "\033[m";
			}else{
				std::cout<<" ";
			}
		}
		std::cout<<std::endl;
	}
}


#endif

