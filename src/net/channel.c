//
// Created by httime on 3/22/20.
//


#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../inc/sock.h"
#include "../inc/judge.h"

#define CLIENT_SIZE 1024


void channel(){
    int sock = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serve_addr, client_addr;
    int opt = 1;

    bzero(&serve_addr, sizeof(serve_addr));
    bzero(&client_addr, sizeof(client_addr));

    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serve_addr.sin_port = htons(SERVE_PORT);
    serve_addr.sin_family = AF_INET;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    Bind(sock, (struct sockaddr *)&serve_addr, sizeof(serve_addr));
    Listen(sock, 5);
    int client_fd[CLIENT_SIZE];
    int maxfd = sock + 1;
    fd_set reads;
    char buf[1024];
    fd_set all;
    FD_ZERO(&all);
    FD_ZERO(&reads);
    FD_SET(sock, &all);

    int max = -1;
    while (1){

        reads = all;
        int num = select(maxfd, &reads, NULL,NULL,NULL);
        if (num<0){
            perror("select err");
        }
        if (FD_ISSET(sock, &reads)){
           int len =  sizeof(client_addr);
           int connfd =  Accept(sock, (struct sockaddr *)&client_addr, &len);
           FD_SET(connfd, &all);
            for (int j = 0; j < CLIENT_SIZE; ++j) {
                if (client_fd[j] == 0){
                    client_fd[j] = connfd;
                    max++;
                    break;
                }
            }
            if (maxfd < connfd){
                maxfd = connfd;
            }

        }


        for (int i = -1; i < max; ++i) {
            if (client_fd[i] == 0){
                continue;
            }

            if (FD_ISSET(client_fd[i], &reads)){
                int n;
                if ((n = read(client_fd[i], buf, sizeof(buf))) == 0){
                    close(client_fd[i]);
                    FD_CLR(client_fd[i], &all);
                    client_fd[i] = 0;
                } else{
                    pthread_t pthread;
                    pthread_create(&pthread, NULL, handlers, &buf);
                }
            }
        }
    }
    close(sock);
}