#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>
#include "routes.h"
struct Server
{
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;
    routes * r;
    struct sockaddr_in address;

    void (*launch)(struct Server * server,routes *r);

    int socket;
};

struct Server server_constructor(int domain, int service, int protocol,
                                  u_long interface, int port, int backlog);

struct Server make_server(int port);

#endif /* Server_h */