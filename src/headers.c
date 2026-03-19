#include "../Include/headers.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h> // for strcasecmp

#define INITIAL_CAP 16

void headers_init(header_array *h) {
    h->count = 0;
    h->capacity = INITIAL_CAP;
    h->items = (header*) malloc(sizeof(header) * h->capacity);
}

void headers_add(header_array *h, char *key, char *value) {
    if (h->count == h->capacity) {
        h->capacity *= 2;
        h->items = (header*) realloc(h->items, sizeof(header) * h->capacity);
    }

    h->items[h->count].key = key;
    h->items[h->count].value = value;
    h->count++;
}

char* headers_get(header_array *h, const char *key) {
    for (size_t i = 0; i < h->count; i++) {
        if (strcasecmp(h->items[i].key, key) == 0)
            return h->items[i].value;
    }
    return NULL;
}

void headers_free(header_array *h) {
    if (!h) return;
    free(h->items);
}