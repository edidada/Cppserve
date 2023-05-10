#include "server.h"
#include <netinet/in.h>
#include <arpa/inet.h>

bool Srpc::InitRpcRun()
{
    char send_message[255] = "hello i am server";
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // address
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr(address_ip.c_str());
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;

    // bind
    if ((bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) != 0)
    {
        fprintf(stderr, "couldn't bind: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // listen
    if ((listen(server_sock, 10)) != 0)
    {
        fprintf(stderr, "couldn't listen: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int client_sock;
        // accept
        if ((client_sock = accept(server_sock, NULL, NULL)) == -1)
        {
            fprintf(stderr, "couldn't accept: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        char recv_buffer[255];
        recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);
        printf("%s\n", recv_buffer);
        send(client_sock, send_message, sizeof(send_message), 0);

        close(client_sock);
    }
    close(server_sock);
}