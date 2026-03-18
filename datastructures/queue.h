#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include <stddef.h>

typedef struct qnode {
    void *data;
    struct qnode *next;
} qnode;

typedef struct {
    qnode *front;
    qnode *rear;
    size_t size;
} queue;

queue* queue_create();
void queue_push(queue *q, void *data);
void* queue_pop(queue *q);
void* queue_peek(queue *q);
int queue_is_empty(queue *q);
void queue_free(queue *q, int free_data);

#endif