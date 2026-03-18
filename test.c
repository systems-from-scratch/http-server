#include "server.h"
#include "httprequest.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 30000

const char* method_to_str(int method) {
    switch(method) {
        case GET:     return "GET";
        case POST:    return "POST";
        case PUT:     return "PUT";
        case DELETE:  return "DELETE";
        case PATCH:   return "PATCH";
        default:      return "UNKNOWN";
    }
}

void launch(struct Server* server) {
    char buffer[BUFFER_SIZE];
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

        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

        if (bytes_read < 0) {
            perror("read failed");
            close(new_socket);
            continue;
        }

        // parse the request
        struct httprequest req = http_request_constructor(buffer);

        printf("[request] method  : %s\n", method_to_str(req.method));
        printf("[request] uri     : %s\n", req.URI ? req.URI : "(null)");
        printf("[request] version : %.1f\n", req.httpversion);
        printf("[request] body    : %s\n", req.body ? req.body : "(empty)");

        // route based on method + URI
        const char *response;

        if (req.method == GET && req.URI && strcmp(req.URI, "/") == 0) {
            response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<html><body><h1>Hello, World!</h1></body></html>";
        } else if (req.method == GET && req.URI && strcmp(req.URI, "/ping") == 0) {
            response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "pong";
        } else {
            response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "404 - Not Found";
        }

        write(new_socket, response, strlen(response));
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