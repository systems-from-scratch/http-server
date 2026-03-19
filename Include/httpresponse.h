#ifndef httpresponse_h
#define httpresponse_h
#include "headers.h"

struct httpresponse
{
    int status_code;
    char * status_message;
    header_array headers;
    char * body;

};

char * httpresponse_build(struct httpresponse * res);
char * return_body(char * body);
#endif /* httpresponse_h */