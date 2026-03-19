#include "Include/server.h"
#include "Include/httprequest.h"
#include "Include/routes.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n"
           "Connection: close\r\n"
           "\r\n"
           "<html><body><h1>Hello, World!</h1></body></html>";
}

char* handle_ping(struct httprequest *req) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Connection: close\r\n"
           "\r\n"
           "pong";
}

void launch(struct Server* server) {
    char buffer[BUFFER_SIZE];
    int address_length = sizeof(server->address);
    int new_socket;

    // set up router
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

        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

        if (bytes_read < 0) {
            perror("read failed");
            close(new_socket);
            continue;
        }

        struct httprequest req = http_request_constructor(buffer);

        printf("[request] method  : %s\n", method_to_str(req.method));
        printf("[request] uri     : %s\n", req.URI ? req.URI : "(null)");
        printf("[request] version : %.1f\n", req.httpversion);

        router_dispatch(r, new_socket, &req);
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