// echo server (tcp/multi)
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock;
    int client_sock;
    struct sockaddr_in serv_addr = {
        0,
    };
    struct sockaddr_in client_addr = {
        0,
    };
    socklen_t client_size = sizeof(client_addr);

    // TCP
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(serv_sock, 5);

    pid_t pid;

    while (1)
    {
        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr,
                             &client_size);
        pid = fork();
        if (pid == 0)
        { // child process
            clientprocess(client_sock);
        }
        else
        { // parent
            printf("client ip : %d\n", getppid());
        }
    }

    return 0;
}

void clientprocess(int sock)
{
    char recvBuf[MAX_SIZE];

    while (1)
    {
        read(sock, recvBuf, MAX_SIZE);
        printf("[%d]%s\n", sock, recvBuf);
        write(sock, recvBuf, MAX_SIZE);
    }
    close(sock);
}
