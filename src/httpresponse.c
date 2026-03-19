#include <../Include/httpresponse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char * httpresponse_build(struct httpresponse * res){
    int size = 64; // status line
    char size_str[32];
    sprintf(size_str, "%zu", strlen(res->body));
    headers_add(&res->headers, "Content-Length", strdup(size_str));
    for (size_t i = 0; i < res->headers.count; i++)
        size += strlen(res->headers.items[i].key) + strlen(res->headers.items[i].value) + 4;
    if (res->body)
        size += strlen(res->body);

    char *output = malloc(size + 4);
    int offset = 0;

    offset += sprintf(output + offset, "HTTP/1.1 %d %s\r\n",
                      res->status_code, res->status_message);

    for (size_t i = 0; i < res->headers.count; i++) {
        offset += sprintf(output + offset, "%s: %s\r\n",
                          res->headers.items[i].key,
                          res->headers.items[i].value);
    }

    offset += sprintf(output + offset, "\r\n");

    if (res->body) {
        offset += sprintf(output + offset, "%s", res->body);
    }
    return output;
}

char * return_body(char * body){
    struct httpresponse res;
    res.status_code = 200;
    res.status_message = "OK";
    res.body = body;
    headers_init(&res.headers);
    headers_add(&res.headers, "Content-Type", "text/plain");
    headers_add(&res.headers, "Connection", "close");
    return httpresponse_build(&res);
}