#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

// Thread State Macros
#define READY 1
#define BLOCKED 2
#define RUNNING 3
#define ZOMBIE 4

queue_t thread_q;

typedef struct tcb* tcb_t;

struct tcb
{
    uthread_t tid;
    uthread_ctx_t* context;
    void* stack;
    int state;
};

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
    return 0;
}

int uthread_create(uthread_func_t func, void *arg)
{
    if(thread_q == NULL){
        thread_q = queue_create();
    }
    tcb_t tcb;
    tcb->tid = queue_length(thread_q) + 1;
    tcb->stack = uthread_ctx_alloc_stack();
    uthread_ctx_init(tcb->context, tcb->stack, func, arg);
    if(tcb == NULL){
        return (-1);
    }
    queue_enqueue(thread_q, tcb);
    return tcb->tid;
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
    return 0;
}

