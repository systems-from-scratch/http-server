#include <string.h>
#include <stdlib.h>
#include "httprequest.h"
#include "datastructures/headers.h"

int method_select(const char *method) {
    if (!method) return -1;

    if (strcmp(method, "GET") == 0) return GET;
    if (strcmp(method, "POST") == 0) return POST;
    if (strcmp(method, "PATCH") == 0) return PATCH;
    if (strcmp(method, "HEAD") == 0) return HEAD;
    if (strcmp(method, "PUT") == 0) return PUT;
    if (strcmp(method, "DELETE") == 0) return DELETE;
    if (strcmp(method, "CONNECT") == 0) return CONNECT;
    if (strcmp(method, "OPTIONS") == 0) return OPTIONS;

    return -1;
}

static void lowercase(char *s) {
    for (; *s; s++) {
        if (*s >= 'A' && *s <= 'Z') *s += 32;
    }
}

struct httprequest http_request_constructor(const char *request_string) {
    struct httprequest request;
    memset(&request, 0, sizeof(request));

    if (!request_string) return request;

    char *buffer = strdup(request_string);
    if (!buffer) return request;

    request.raw_buffer = buffer;

    char *body_start = strstr(buffer, "\r\n\r\n");
    if (body_start) {
        *body_start = '\0';
        request.body = body_start + 4;
    }

    char *saveptr;
    char *line = strtok_r(buffer, "\r\n", &saveptr);
    if (!line) return request;

    char *saveptr2;
    char *method  = strtok_r(line, " ", &saveptr2);
    char *uri     = strtok_r(NULL, " ", &saveptr2);
    char *version = strtok_r(NULL, " ", &saveptr2);
    request.method = method_select(method);
    request.URI = uri ? uri : NULL;

    if (version && strncmp(version, "HTTP/", 5) == 0) {
        request.httpversion = (float) atof(version + 5);
    }

    headers_init(&request.headers);

    char *header_line = strtok_r(NULL, "\r\n", &saveptr);
    while (header_line) {
        char *colon = strchr(header_line, ':');

        if (colon) {
            *colon = '\0';

            char *key = header_line;
            char *value = colon + 1;

            while (*value == ' ') value++;

            lowercase(key);

            headers_add(&request.headers, key, value);
        }

        header_line = strtok_r(NULL, "\r\n", &saveptr);
    }

    return request;
}