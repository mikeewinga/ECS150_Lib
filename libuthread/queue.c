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

queue_t queue_create(void)
{
    queue_t q=(queue_t*)malloc(sizeof(queue_t));
    q->head=NULL;
    q->tail=NULL;
    q->num_nodes=0;
    if(!q) {
        return(NULL);
    }
    return(*q);
}

int queue_destroy(queue_t queue)
{
    if(queue==NULL || queue->num_nodes!=0){
        return(-1);
    }
    else{
        free(queue);
        return(0);
    }
}

int queue_enqueue(queue_t queue, void *data)
{
    if(queue==NULL || data==NULL){
        return(-1);
    }
    node_t new_node;
    new_node.data = data;
    new_node.prev=queue->tail;
    new_node.next=queue->tail->next;
    queue->tail->next=new_node;
    queue->tail=new_node;
    queue->num_nodes+=1;
    return(0);
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
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

