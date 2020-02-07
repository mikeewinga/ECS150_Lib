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

typedef struct tcb* tcb_t;

struct tcb
{
    uthread_t tid;
    uthread_ctx_t* context;
    void* stack;
    int state;
    uthread_t block_tid;
};

// Global Variables
queue_t ready_q; 
tcb_t running_t;
queue_t blocked_q;
queue_t zombie_q; 
uthread_t curr_tid = 1;
uthread_t join_tid = 0;

void uthread_yield(void)
{
    tcb_t ready=(tcb_t)malloc(sizeof(tcb_t));
    if(running_t->state == BLOCKED){
        tcb_t blocked=(tcb_t)malloc(sizeof(tcb_t));
        blocked = running_t; 
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**)ready);
        // change their states respct.
        ready->state = RUNNING;
        running_t = ready;
        uthread_ctx_switch(blocked->context, running_t->context);
    }
    else{
        tcb_t yielding=(tcb_t)malloc(sizeof(tcb_t)); 
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**)ready);
        // store the running thread
        yielding = running_t;
        // change their states respct.
        running_t->state = READY;
        ready->state = RUNNING;
        // move the threads
        queue_enqueue(ready_q, running_t);
        running_t = ready;
        uthread_ctx_switch(yielding->context, running_t->context);
    }
}

uthread_t uthread_self(void)
{
    return running_t->tid; 
}

void init_uthread_mgmt(void)
{
    ready_q = queue_create();
    blocked_q = queue_create();
    zombie_q = queue_create();
    running_t = (tcb_t)malloc(sizeof(tcb_t));
}

int uthread_create(uthread_func_t func, void *arg)
{   
    if(curr_tid < 2){
        init_uthread_mgmt();
    }
    tcb_t tcb = (tcb_t)malloc(sizeof(tcb_t));
    tcb->tid = curr_tid;
    curr_tid += 1;
    tcb->state = READY;
    tcb->stack = uthread_ctx_alloc_stack();
    uthread_ctx_init(tcb->context, tcb->stack, func, arg);
    tcb->block_tid = 0;
    if(tcb == NULL){
        return (-1);
    }

    queue_enqueue(ready_q, &tcb);
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
    if(retval == 0){
        // remove next ready thread
        tcb_t ready=(tcb_t)malloc(sizeof(tcb_t));
        tcb_t completed;
        queue_dequeue(ready_q, (void**)ready);
        completed = running_t;
        // change the states of each thread respct.
        running_t->state = ZOMBIE;
        ready->state = RUNNING;
        // place each thread to their new position
        queue_enqueue(zombie_q, completed);
        running_t = ready;

        /*if(completed->block_tid != 0){
            uthread_join()
        }*/

        // switch context
        uthread_ctx_switch(completed->context, running_t->context);    
    }
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
    tcb_t blocked;
    running_t->state = BLOCKED;
    blocked = running_t;
    queue_enqueue(blocked_q, blocked);
	while(1)
    {
        /*        
        if(join_tid != 0){
            break;
	    }
        else{
            uthread_yield();
        }
        */
        if(queue_length(ready_q) == 0){
            break;
        }
        else{
            uthread_yield();        
        }
    }
    tid += 1;
    tid -= 1;
    *retval += 1;
    *retval -= 1;
    return *retval;
}

