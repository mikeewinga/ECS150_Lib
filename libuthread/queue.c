#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct queue Queue;

struct queue 
{
    void* data;
    Queue* next;
    Queue* prev;
};

queue_t queue_create(void)
{
    Queue HEAD = (Queue*)malloc(sizeof(Queue));
    HEAD.data = NULL;
    HEAD.next = NULL;
    HEAD.prev = NULL;
    if(!HEAD) {
        return(NULL);
    }
    return(*HEAD);
    /* Maybe add a TAIL? */
}

int queue_destroy(queue_t queue)
{
    /* TODO Phase 1 */
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

