#include <../Include/httpresponse.h>

char * httpresponse_build(struct httpresponse * res){
    int size = 64; // status line
    for (int i = 0; i < res->headers.count; i++)
        size += strlen(res->headers.items[i].key) + strlen(res->headers.items[i].value) + 4;
    if (res->body)
        size += strlen(res->body);

    char *output = malloc(size + 4);
    int offset = 0;

    offset += sprintf(output + offset, "HTTP/1.1 %d %s\r\n",
                      res->status_code, res->status_message);

    for (int i = 0; i < res->headers.count; i++) {
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