#ifndef client_h
#define client_h
#include "../Include/routes.h"


// in client.h
typedef struct client_args {
    int socket;
    routes *r;
} client_args;


void* handle_client(void* arg);


#endif