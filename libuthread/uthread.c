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

// Globals queues to hold threads of respective states
queue_t ready_q; 
queue_t blocked_q;
queue_t thread_q;
queue_t zombie_q; 

typedef struct tcb* tcb_t;

struct tcb
{
    uthread_t tid;
    uthread_ctx_t* context;
    void* stack;
    int state;
};

/*
 * uthread_yield - Yield execution
 *
 * This function is to be called from the currently active and running thread in
 * order to yield for other threads to execute.
 */
void uthread_yield(void)
{
    /*
    for this step, sched is non-preemptive and threads must call this function to run next avail thread. 
    non-compliant threads can keep resources to self

    i think we want to
        dequeue current thread from running queue, change state from RUNNING to READY, enqueue it onto ready queue
        then dequeue the thread to be yielded to from ready queue, change state from READY to RUNNING, and enqueue onto running queue
    */


   return;
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
/*
 * uthread_exit - Exit from currently running thread
 * @retval: Return value
 *
 * This function is to be called from the currently active and running thread in
 * order to finish its execution. The return value @retval is to be collected
 * from a joining thread.
 *
 * A thread which has not been 'collected' should stay in a zombie state. This
 * means that until collection, the resources associated to a zombie thread
 * should not be freed.
 *
 * This function shall never return.
 */
void uthread_exit(int retval)
{
    /* TODO Phase 2 */
}

/*
 * uthread_join - Join a thread
 * @tid: TID of the thread to join
 * @retval: Address of an integer that will receive the return value
 *
 * This function makes the calling thread wait for the thread @tid to complete
 * and assign the return value of the finished thread to @retval (if @retval is
 * not NULL).
 *
 * A thread can be joined by only one other thread.
 *
 * Return: -1 if @tid is 0 (the 'main' thread cannot be joined), if @tid is the
 * TID of the calling thread, if thread @tid cannot be found, or if thread @tid
 * is already being joined. 0 otherwise.
 */
int uthread_join(uthread_t tid, int *retval)
{
    /*
    Execute an infinite loop in which
    If there are no more threads which are ready to run in the system, break the loop and return
    Otherwise simply yield to next available thread
    */
   
	while(1)
    {
        //check if queue of ready threads is empty, then break
        if(queue_dequeue(ready_q, (void **)(thread_q)) == -1)
	    {
            break;
	    }
        //otherwise keep yielding to next avail thread
        //TODO, idk if we actually call yield 
        uthread_yield();
    }
	/* TODO Phase 3 */
    return 0;
}

