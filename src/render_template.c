#include "../Include/render_template.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Include/httpresponse.h"
#include "../Include/headers.h"
char* get_mime_type(const char* filename) {
    if (strstr(filename, ".html")) return "text/html";
    if (strstr(filename, ".css"))  return "text/css";
    if (strstr(filename, ".js"))   return "application/javascript";
    return "text/plain";
}

char * render_template(char * filename){
    char buf[1000];
    char * res_str;
    struct httpresponse *res;
    res = (struct httpresponse*) malloc(sizeof(struct httpresponse));
    getcwd(buf, sizeof(buf));
    strcat(buf, "/templates/");
    strcat(buf, filename);
    FILE* file_pointer;
    file_pointer = fopen(buf,"r");
    if (file_pointer!=NULL){
    fseek(file_pointer,0,SEEK_END);
    long file_size = ftell(file_pointer);
    rewind(file_pointer);

    char * buffer = (char*) malloc(file_size+1);
    size_t bytesread = fread(buffer,1,file_size,file_pointer);
    buffer[bytesread] = '\0';
    res->status_code = 200;
    res->status_message = "OK";
    char size_str[32];
    sprintf(size_str, "%ld", file_size);
    headers_init(&res->headers);
    headers_add(&res->headers,"Content-Type",get_mime_type(filename));
    headers_add(&res->headers,"Content-Length",strdup(size_str));
    headers_add(&res->headers,"Connection","keep-alive");
    res->body = buffer;
    res_str = httpresponse_build(res);
    free(buffer);
    free(res);
    }
    else{
        res->status_code = 404;
        res->status_message = "Not Found";
        res->body = "File not found";
        res_str = httpresponse_build(res);
        free(res);
    }
    return res_str;
}