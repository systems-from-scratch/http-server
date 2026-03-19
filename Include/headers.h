#ifndef HEADERS_H
#define HEADERS_H

#include <stddef.h>

typedef struct {
    char *key;
    char *value;
} header;

typedef struct {
    header *items;
    size_t count;
    size_t capacity;
} header_array;

void headers_init(header_array *h);
void headers_add(header_array *h, char *key, char *value);
char* headers_get(header_array *h, const char *key);
void headers_free(header_array *h);

#endif