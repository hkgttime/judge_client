#ifndef HEADER_FILE
#define HEADER_FILE

#include <bits/socket.h>

#define SERVE_PORT 4000
#define CLIENT_INIT(client_fd)  __CLIENT_INIT(client_fd)

extern int Socket(int __domain, int __type, int __protocol);
void Bind(int __fd, const struct sockaddr* __addr, socklen_t __len);
void Listen(int __fd, int __len);
int Accept (int fd, const struct sockaddr *addr, socklen_t *__restrict addr_len);

#define __CLIENT_INIT(client_fd) \

#endif