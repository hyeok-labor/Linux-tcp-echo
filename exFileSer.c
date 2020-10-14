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
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr = {
        0,
    };
    struct sockaddr_in client_addr = {
        0,
    };
    char recvBuf[MAX_SIZE];
    socklen_t client_size = sizeof(client_addr);
    int recvByte = 0;
    int sendByte = 0;
    //server대한 ip/port 부여하기
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //자신의 IP를 서버 IP로 지정
    serv_addr.sin_port = htons(atoi(argv[1]));

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 5);
    pid_t pid;

    while (1)
    {
        printf("<wait...>");
        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_size);
        pid = fork();
        if (pid == 0)
        { //child process
            clientprocess(client_sock);
        }
        else
        { //parent process;
            printf("client ip : %s \n", inet_ntoa(client_addr.sin_addr));
        }
    }
    return 0;
}

void clientprocess(int sock)
{
    char recvBuff[MAX_SIZE];
    while (1)
    {
        read(sock, recvBuff, MAX_SIZE);
        if (!strncmp("QUIT", recvBuff, strlen("QUIT")))
        {
            shutdown(sock, SHUT_RD);
            break;
        }
        printf("[%d]%s\n", sock, recvBuff);
        write(sock, recvBuff, MAX_SIZE);
    }
    printf("...e[%d]%s\n", sock, recvBuff);
    write(sock, recvBuff, MAX_SIZE);
    close(sock);
}
