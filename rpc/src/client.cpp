#include "client.h"
#include <netinet/in.h>
#include<arpa/inet.h>

/**
 * @brief 运行rpc远程调用
 * @param func 需要调用的函数名
 * @return 成功true，失败false
 *
*/
bool Crpc::RpcRun(std::string func)
{
     int sock = socket(AF_INET, SOCK_STREAM, 0);


    // create address
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr(address_ip.c_str());
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;

    // connect
    if ((connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) == -1)
    {
        perror("connect error because of: ");
        close(sock);
        return false;
    }

    send(sock, func.c_str(), strlen(func.c_str()),0); 

    char recv_buffer[255];
    recv(sock, recv_buffer, sizeof(recv_buffer), 0);

    printf("printf recv: %s\n", recv_buffer);

    close(sock);
    return true;
}