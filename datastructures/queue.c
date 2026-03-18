#include "queue.h"
#include <stdlib.h>

queue* queue_create() {
    queue *q = (queue*) malloc(sizeof(queue));
    if (!q) return NULL;
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

void queue_push(queue *q, void *data) {
    if (!q) return;

    qnode *node = (qnode*) malloc(sizeof(qnode));
    if (!node) return;

    node->data = data;
    node->next = NULL;

    if (q->rear) {
        q->rear->next = node;
    } else {
        q->front = node;
    }

    q->rear = node;
    q->size++;
}

void* queue_pop(queue *q) {
    if (!q || !q->front) return NULL;

    qnode *temp = q->front;
    void *data = temp->data;

    q->front = temp->next;
    if (!q->front) q->rear = NULL;

    free(temp);
    q->size--;

    return data;
}

void* queue_peek(queue *q) {
    if (!q || !q->front) return NULL;
    return q->front->data;
}

int queue_is_empty(queue *q) {
    return (q == NULL || q->size == 0);
}

void queue_free(queue *q, int free_data) {
    if (!q) return;

    qnode *curr = q->front;
    while (curr) {
        qnode *next = curr->next;
        if (free_data && curr->data) free(curr->data);
        free(curr);
        curr = next;
    }

    free(q);
}