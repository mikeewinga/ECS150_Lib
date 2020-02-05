#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node* node_t;

struct node
{
    void* data;
    node_t next;
    node_t prev;
};

struct queue
{
    node_t head;
    node_t tail;
    int num_nodes;
};

static node_t node_create(void* data)
{
    node_t n = (node_t)malloc(sizeof(node_t));
    if (data == NULL) {
        free(n);
        return(NULL);
    }
    n->data = data;
    return(n);
}

queue_t queue_create(void)
{
    queue_t q = (queue_t)malloc(sizeof(queue_t));
    q->head = ((node_t)NULL);
    q->tail = ((node_t)NULL);
    q->num_nodes = 0;
    if (!q) {
        free(q);
        return(*((queue_t*)NULL));
    }
    return(q);
}

int queue_destroy(queue_t queue)
{
    if ( queue == NULL || queue->num_nodes != 0) {
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
        new_node->prev = NULL;
        new_node->next = NULL;
        queue->head = new_node;
        queue->tail = new_node;
    }
    else {
        node_t tail_node = queue->tail;
        new_node->prev = tail_node->prev;
        new_node->next = tail_node;
        tail_node->prev = new_node;
        queue->tail = new_node;
    }
    queue->num_nodes += 1;
    return(0);
}

int queue_dequeue(queue_t queue, void **data)
{
    // node at front of the queue
    node_t oldest_node = queue->head;
    if (queue == NULL || data == NULL || !oldest_node->data) {
        return(-1);
    }
    *data = oldest_node->data;
    if (queue->num_nodes == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else {
        node_t prev_oldest_node = oldest_node->prev;
        node_t next_oldest_node = oldest_node->next;
        prev_oldest_node->next = next_oldest_node;
        next_oldest_node->prev = prev_oldest_node;
    }
    free(oldest_node);
    queue->num_nodes-=1;
    return(0);
}

int queue_delete(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL) {
        return(-1);
    }
    node_t n= queue->head;
    while(1){
        if(n->data==data){
            node_t n_next= n->next;
            node_t n_prev= n->prev;
            n_next->prev= n_prev;
            n_prev->next= n_next;
            free(n);
            break;
        }
        else{
            if(n->next == queue->tail){
                return(-1);
            }
            n = n->next;
            continue;
        }
    }
    return(0);
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
    if(queue == NULL || func == NULL){
        return(-1);
    }
    node_t current = queue->head;
    while(current != NULL){
        if(func(current->data, arg) == 1){       
            if(data != NULL){
                *data = current->data;
            }
            break;
        }
        current = current->prev; 
        }
    return 0;
}

int queue_length(queue_t queue)
{
    return(queue->num_nodes);
}
