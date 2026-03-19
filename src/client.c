#include "../Include/client.h"
#include "../Include/httprequest.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

void* handle_client(void* arg) {
    client_args *args = (client_args*)arg;
    int new_socket = args->socket;
    routes *r = args->r;
    free(arg); 

    char buffer[30000];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);

        if (bytes_read <= 0) {
            break;
        }

        struct httprequest req = http_request_constructor(buffer);

        printf("[request] method  : %s\n", method_to_str(req.method));
        printf("[request] uri     : %s\n", req.URI ? req.URI : "(null)");
        printf("[request] version : %.1f\n", req.httpversion);

        router_dispatch(r, new_socket, &req);
        char *conn = headers_get(&req.headers, "connection");
        if (!conn || strcmp(conn, "keep-alive") != 0) break;
    }

    close(new_socket);
    return NULL;
}