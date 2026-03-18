#include "dict.h"
#include <stdlib.h>
#include <string.h>

static unsigned long hash(const char *str) {
    unsigned long h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + c; // djb2
    return h;
}

static dict_entry* create_entry(const char *key, const char *value) {
    dict_entry *e = (dict_entry*) malloc(sizeof(dict_entry));
    if (!e) return NULL;

    e->key = strdup(key);
    e->value = strdup(value);
    e->next = NULL;

    return e;
}

dict* dict_create(size_t capacity) {
    dict *d = (dict*) malloc(sizeof(dict));
    if (!d) return NULL;

    d->capacity = capacity;
    d->buckets = (dict_entry**) calloc(capacity, sizeof(dict_entry*));
    return d;
}

void dict_set(dict *d, const char *key, const char *value) {
    if (!d || !key) return;

    unsigned long idx = hash(key) % d->capacity;

    dict_entry *curr = d->buckets[idx];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            free(curr->value);
            curr->value = strdup(value);
            return;
        }
        curr = curr->next;
    }

    dict_entry *new_entry = create_entry(key, value);
    if (!new_entry) return;

    new_entry->next = d->buckets[idx];
    d->buckets[idx] = new_entry;
}

char* dict_get(dict *d, const char *key) {
    if (!d || !key) return NULL;

    unsigned long idx = hash(key) % d->capacity;

    dict_entry *curr = d->buckets[idx];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }

    return NULL;
}

void dict_delete(dict *d, const char *key) {
    if (!d || !key) return;

    unsigned long idx = hash(key) % d->capacity;

    dict_entry *curr = d->buckets[idx];
    dict_entry *prev = NULL;

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) prev->next = curr->next;
            else d->buckets[idx] = curr->next;

            free(curr->key);
            free(curr->value);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void dict_free(dict *d) {
    if (!d) return;

    for (size_t i = 0; i < d->capacity; i++) {
        dict_entry *curr = d->buckets[i];
        while (curr) {
            dict_entry *next = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            curr = next;
        }
    }

    free(d->buckets);
    free(d);
}