#include "Include/server.h"
#include "Include/httprequest.h"
#include "Include/httpresponse.h"
#include "Include/routes.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 30000

const char* method_to_str(int method) {
    switch(method) {
        case GET:    return "GET";
        case POST:   return "POST";
        case PUT:    return "PUT";
        case DELETE: return "DELETE";
        case PATCH:  return "PATCH";
        default:     return "UNKNOWN";
    }
}

char* handle_index(struct httprequest *req) {
    struct httpresponse res;
    res.status_code = 200;
    res.status_message = "OK";
    res.body = "<html><body><h1>Hello, World!</h1></body></html>";
    headers_init(&res.headers);
    headers_add(&res.headers, "Content-Type", "text/html");
    headers_add(&res.headers, "Connection", "keep-alive");
    return httpresponse_build(&res);
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

void launch(struct Server* server) {
    char buffer[BUFFER_SIZE];
    int address_length = sizeof(server->address);
    int new_socket;

    routes *r = create_routes(10);
    r = add_routes(r, GET, "/", handle_index);
    r = add_routes(r, GET, "/ping", handle_ping);

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
        while (1){
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

        if (bytes_read <= 0) {
            perror("read failed");
            close(new_socket);
            break;
        }

        struct httprequest req = http_request_constructor(buffer);

        printf("[request] method  : %s\n", method_to_str(req.method));
        printf("[request] uri     : %s\n", req.URI ? req.URI : "(null)");
        printf("[request] version : %.1f\n", req.httpversion);

        router_dispatch(r, new_socket, &req);
        char * conn  = headers_get(&req.headers,"connection");
        if (!conn || strcmp(conn, "keep-alive") != 0) break;
    }

    close(new_socket);
    }
}

int main() {
    printf("[server] starting on port 8000\n");
    struct Server server = server_constructor(
        AF_INET, SOCK_STREAM, 0,
        INADDR_ANY, 8000, 10,
        launch
    );
    server.launch(&server);
    return 0;
}