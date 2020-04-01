//
// Created by httime on 3/23/20.
//

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>

int Socket(int __domain, int __type, int __protocol){
    int sock = socket(__domain, __type, __protocol);
    if (sock < 0){
        perror("socket creat err");
    }
    return sock;
}

void Bind(int __fd, __SOCKADDR_ARG __addr, socklen_t __len){
        int b = bind(__fd, __addr, __len);
        if (b < 0){
            perror("bind err");
        }
}

void Listen(int fd, int len){
    int listens = listen(fd, len);
    if (listens < 0){
        perror("listen err");
    }

}


int Accept (int fd, const struct sockaddr *addr, socklen_t *__restrict addr_len){
    int ret;
    again:
    if((ret = accept(fd, addr, addr_len))< 0){
        if ((errno == ECONNABORTED) || (errno == EINTR)){
            goto again;
        } else{
            perror("accept err");
            exit(0);
        }
    }
    return ret;
}