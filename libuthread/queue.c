#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node* node_t;

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
    node_t n = malloc(sizeof(node_t));
    if (&data == NULL) {
        free(n);
        return(*((node_t*)NULL));
    }
    n->data = data;
    return(n);
}

queue_t queue_create(void)
{
    queue_t q = (queue_t)malloc(sizeof(queue_t));
    q->head = ((node_t*)NULL);
    q->tail = ((node_t*)NULL);
    q->num_nodes = 0;
    if (!q) {
        free(q);
        return(*((queue_t*)NULL));
    }
    return(q);
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
        new_node->prev = ((node_t*)NULL);
        new_node->next = ((node_t*)NULL);
        queue->head = &new_node;
        queue->tail = &new_node;
    }
    else {
        node_t tail_node= *queue->tail;
        new_node->prev = queue->tail;
        new_node->next = tail_node->next;
        tail_node->next = &new_node;
        queue->tail = &new_node;
    }
    queue->num_nodes += 1;
    return(0);
}

int queue_dequeue(queue_t queue, void **data)
{
    node_t old_node = *queue->head;
    if (queue == NULL || data == NULL || !old_node->data) {
        return(-1);
    }
    while(old_node != NULL){
        if( data == old_node->data){
            if (queue->num_nodes == 1) {
                queue->head = NULL;
                queue->tail = NULL;
            }
            else {
                node_t next_oldest_node = *old_node->next;
                next_oldest_node->prev = old_node->prev;
                queue->head = old_node->next;
            }
            free(old_node);
            queue->num_nodes-=1;
        }
        else{
            old_node = old_node->next;
        }
    }
    return(0);
}

int queue_delete(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL) {
        return(-1);
    }
    node_t n=queue->head;
    while(1){
        if(n->data==data){
            node_t n_next=n->next;
            node_t n_prev=n->prev;
            n_next->prev=n_prev;
            n_prev->next=n_next;
            free(n);
            break;
        }
        else{
            if(n->next == queue->tail){
                return(-1);
            }
            n=n->next;
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
    void* output;
    while(current != NULL){
        void* node_val= current->data;
        output = func(node_val, arg);
        if(&output){
            data = output;
            return 0;
        }
    }
    return 0;
}

int queue_length(queue_t queue)
{
    return(&(queue->num_nodes));
}
