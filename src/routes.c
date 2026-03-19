#include "../Include/routes.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
routes * create_routes(int capacity){
    routes* routeslist = (routes*) malloc(sizeof(routes));
    routeslist->capacity = capacity;
    routeslist->count = 0;
    routeslist->routes = (route *) malloc(sizeof(route)*capacity);
    return routeslist;
}

routes* add_routes(routes * routelist,int method, char* uri, char * (handler)(struct httprequest * req)){
    if (routelist->capacity==routelist->count){
        routes * newlist = create_routes(routelist->capacity*2);
        newlist->count = routelist->count;
        for (size_t i = 0; i < routelist->count; i++)
        {
            newlist->routes[i] = routelist->routes[i] ;
        }
        free(routelist->routes);
        free(routelist);
        routelist = newlist;
    }
    route newroute = {method,uri,handler};
    routelist->routes[routelist->count] = newroute;
    routelist->count++;
    return routelist;
}

void router_dispatch(routes * r, int socket,struct httprequest *req){
    for (size_t i = 0; i < r->count; i++)
    {
        if (r->routes[i].method==req->method && (strcmp(r->routes[i].uri,req->URI)==0)){
            char * response = r->routes[i].handler(req);
            write(socket,response,strlen(response));
            return;
        }
    }
    
    char *response =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "404 - Not Found";
    write(socket,response,strlen(response));
    return;
    }

