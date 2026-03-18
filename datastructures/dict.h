#ifndef DS_DICT_H
#define DS_DICT_H

#include <stddef.h>

typedef struct dict_entry {
    char *key;
    char *value;
    struct dict_entry *next;
} dict_entry;

typedef struct {
    dict_entry **buckets;
    size_t capacity;
} dict;

dict* dict_create(size_t capacity);
void dict_set(dict *d, const char *key, const char *value);
char* dict_get(dict *d, const char *key);
void dict_delete(dict *d, const char *key);
void dict_free(dict *d);

#endif