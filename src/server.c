#include "../Include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Include/routes.h"
#include "../Include/httprequest.h"
#include "../Include/httpresponse.h"
#include "../Include/client.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../Include/render_template.h"
#define BUFFER_SIZE 30000

void launch(struct Server* server, routes *r) {
    int address_length = sizeof(server->address);
    int new_socket;
    while (1) {
        printf("\n[server] waiting for connection...\n");
        fflush(stdout);

        new_socket = accept(server->socket,
                           (struct sockaddr*)&server->address,
                           (socklen_t*)&address_length);
        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }
        client_args *args = malloc(sizeof(client_args));
        args->socket = new_socket;
        args->r = r;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, args);
        pthread_detach(thread);
    }
}

struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog){
    struct Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    server.launch = launch;
    
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);  
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain,service,protocol);
    if (server.socket<0){
        perror("Failed to connect sockket");
        exit(1);
    }

    if ((bind(server.socket, (struct sockaddr*)&server.address,sizeof(server.address)))<0){
        perror("Failed to bind socket\n");
        exit(1);
    }

    if ((listen(server.socket,server.backlog))<0){
        perror("Failed to listen");
        exit(1);
    }


    return server;
}

struct Server make_server(int port) {
    return server_constructor(
        AF_INET,      
        SOCK_STREAM,  
        0,            
        INADDR_ANY,   
        port,         
        10        
    );
}