//
// Created by httime on 3/16/20.
//

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PORT 5588
#define SERVER_ADDR "127.0.0.1"

#define ERR_EXIT(m) \
    do \
{ \
    perror(m); \
    exit(EXIT_FAILURE); \
    } while(0)

void registers() {

    struct sockaddr_in in;
    int size,on = 1;

    bzero(&in, sizeof(in));
    in.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    in.sin_family = AF_INET;
    in.sin_port = htons(PORT);
    char sendbuf[1024];
    char recvbuf[1024];

    int ret;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) perror("sock err");

    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    while (1)
    {
        memset(&sendbuf, 0, 1024);
        memset(&recvbuf, 0, 1024);
        /*读取用户输入到buf中*/
        fgets(sendbuf, 256, stdin);
        int n;
        if ((n = sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &in, sizeof(struct sockaddr))) < 0){
            printf("%d\n", errno);
            printf("%s\n", sendbuf);
            exit(0);
        }
        printf("向服务器发送：%s\n",sendbuf);
        ret = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        if (ret == -1)
        {
            if (errno == EINTR)
                continue;
            ERR_EXIT("recvfrom");
        }
        printf("从服务器接收：%s\n",recvbuf);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock);
}

