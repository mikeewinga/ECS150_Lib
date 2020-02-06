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
#define RUNNING 2
#define BLOCKED 3
#define ZOMBIE 4

// Global Variables
queue_t ready_q; 
queue_t running_q;
queue_t blocked_q;
queue_t zombie_q; 
uthread_t curr_tid = 0;

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
    // dequeue the currently running thread
    tcb_t running;
    queue_dequeue(running_q, running); 
    // dequeue the next ready thread
    tcb_t ready;
    queue_dequeue(ready_q, ready);
    
    // change their states respectivley
    running->state = READY;
    ready->state = RUNNING;
    
    // enqueue each respectively
    queue_enqueue(ready_q, running);
    queue_enqueue(running_q, ready);
}

uthread_t uthread_self(void)
{
    // need to figure out a way to do this     
    return 0;
}

tcb_t init_uthread_mgmt(void)
{
    ready_q = queue_create();
    running_q = queue_create();
    blocked_q = queue_create();
    zombie_q = queue_create();

    tcb_t main;
    main->tid = curr_tid;
    curr_tid += 1;
    main->state = RUNNING;
    main->stack = uthread_ctx_alloc_stack();
    uthread_ctx_init(main->context, tcb->stack, void, void);
}

int uthread_create(uthread_func_t func, void *arg)
{   
    if(curr_tid == 0){
        tcb_t main = init_uthread_mgmt();
    }

    tcb_t tcb;
    tcb->tid = curr_tid;
    curr_tid += 1;
    tcb->state = READY;
    tcb->stack = uthread_ctx_alloc_stack();
    uthread_ctx_init(tcb->context, tcb->stack, func, arg);
    if(tcb == NULL){
        return (-1);
    }
    queue_enqueue(ready_q, tcb);
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
        if(queue_dequeue(queue_length(ready_q) == 0){
            break;
	    }
        //uthread_yield();
    }
    return 0;
}

