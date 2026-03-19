#ifndef httprequest_h
#define httprequest_h
#include "headers.h"

enum httpmethods{
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACK
};


struct httprequest {
    int method;
    char *URI;
    float httpversion;

    header_array headers;

    char *body;
    char *raw_buffer; // owns memory
};

struct httprequest http_request_constructor(const char* request_string);

#endif /* httprequest_h */