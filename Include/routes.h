#ifndef routes_h
#define routes_h

#include "httprequest.h"
#include <stdlib.h>
typedef struct route
{
    int method;
    char * uri;
    char* (*handler)(struct httprequest * req);
} route;

typedef struct routes{
    int capacity;
    int count;
    route * routes;
} routes;

routes * create_routes(int capacity);

routes* add_routes(routes * routelist,int method, char* uri, char * (handler)(struct httprequest * req));
void router_dispatch(routes * r, int socket,struct httprequest *req);



#endif