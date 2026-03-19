#include "Include/server.h"
#include "Include/httprequest.h"
#include "Include/httpresponse.h"
#include "Include/routes.h"
#include "Include/client.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "Include/render_template.h"
#define BUFFER_SIZE 30000


char* handle_index(struct httprequest *req) {
    return render_template("index.html");
}

char* handle_ping(struct httprequest *req) {
    struct httpresponse res;
    res.status_code = 200;
    res.status_message = "OK";
    res.body = "pong";
    headers_init(&res.headers);
    headers_add(&res.headers, "Content-Type", "text/plain");
    headers_add(&res.headers, "Connection", "close");
    return httpresponse_build(&res);
}

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

int main() {
    printf("[server] starting on port 8000\n");
    struct Server server = server_constructor(
        AF_INET, SOCK_STREAM, 0,
        INADDR_ANY, 8000, 10,
        launch
    );
    routes *r = create_routes(10);
    r = add_routes(r, GET, "/", handle_index);
    r = add_routes(r, GET, "/ping", handle_ping);
    server.launch(&server,r);
    return 0;
}