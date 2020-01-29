#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node node_t;

struct node
{
    void* data;
    node_t* next;
    node_t* prev;
};

struct queue
{
    node_t* head;
    node_t* tail;
    int num_nodes;
};

node_t node_create(void* data)
{
    node_t* n = (node_t*)malloc(sizeof(node_t));
    n->data = data;
    if (n->data != NULL) {
        return((node_t*)NULL);
    }
    return(n);
}

queue_t queue_create(void)
{
    queue_t q = (queue_t)malloc(sizeof(queue_t));
    q->head = NULL;
    q->tail = NULL;
    q->num_nodes = 0;
    if (!q) {
        return(NULL);
    }
    return(*q);
}

int queue_destroy(queue_t queue)
{
    if (queue == NULL || queue->num_nodes != 0) {
        return(-1);
    }
    else {
        free(queue);
        return(0);
    }
}

int queue_enqueue(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL) {
        return(-1);
    }
    node_t new_node = node_create(data);
    if (queue->num_nodes == 0) {
        new_node.prev = NULL;
        new_node.next = NULL;
        queue->head = new_node;
        queue->tail = new_node;
    }
    else {
        new_node.prev = queue->tail;
        new_node.next = queue->tail->next;
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->num_nodes += 1;
    return(0);
}

int queue_dequeue(queue_t queue, void **data)
{
    node_t old_node = queue->head;
    if (queue == NULL || data == NULL || !old_node.data) {
        return(-1);
    }
    data = queue->head.data;
    if (queue->num_nodes == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else {
        old_node.next.prev = old_node.prev;
        queue->head = old_node.next;
    }
    free(old_node);
    return(0);
}

int queue_delete(queue_t queue, void *data)
{
    /* Need to change to work with node_t */
    node_t* next_node;
    if (queue->next != NULL) {
        next_q = queue->next;
        queue_destroy(next_q);
    }
    free(queue);
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
    /* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
    /* TODO Phase 1 */
}

