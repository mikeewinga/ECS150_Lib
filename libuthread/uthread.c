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

void uthread_yield(void)
{
    // TCB that will hold the next ready thread
    tcb_t ready = (tcb_t)malloc(sizeof(struct tcb));
    // if the currently running thread has already been enqueued
    if(running_t->state == BLOCKED || running_t->state == ZOMBIE){
        tcb_t current_thread = (tcb_t)malloc(sizeof(struct tcb));
        current_thread = running_t; 
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**)ready);
        // change its state
        ready->state = RUNNING;
        // replace the running thread with the ready one
        running_t = ready;
        // context switch
        uthread_ctx_switch(current_thread->context, running_t->context);
    }
    // if both running and ready threads need to be enqueued
    else{
        tcb_t yielding = (tcb_t)malloc(sizeof(struct tcb)); 
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**)ready);
        // store the running thread
        yielding = running_t;
        // change their states respctively
        running_t->state = READY;
        ready->state = RUNNING;
        // enqueue running thread into ready
        queue_enqueue(ready_q, running_t);
        // replace running thread with ready thread
        running_t = ready;
        //context switch
        uthread_ctx_switch(yielding->context, running_t->context);
    }
}

uthread_t uthread_self(void)
{
    return running_t->tid; 
}

void init_uthread_mgmt(void)
{
    // create the global queues
    ready_q = queue_create();
    blocked_q = queue_create();
    zombie_q = queue_create();
    // create global running TCB
    running_t = (tcb_t)malloc(sizeof(struct tcb));
}

int uthread_create(uthread_func_t func, void *arg)
{   
    // if this is the first thread created
    // initialize the global structures
    if(curr_tid < 2){
        init_uthread_mgmt();
    }
    tcb_t new_tcb = (tcb_t)malloc(sizeof(struct tcb));
    new_tcb->tid = curr_tid;
    curr_tid += 1;
    new_tcb->state = READY;
    new_tcb->stack = uthread_ctx_alloc_stack();
    uthread_ctx_init(new_tcb->context, new_tcb->stack, func, arg);
    new_tcb->block_tid = 0;
    if(new_tcb == NULL){
        return (-1);
    }
    queue_enqueue(ready_q, new_tcb);
    return new_tcb->tid;
}

void uthread_exit(int retval)
{
    // if the function returned good
    if(retval == 0){
        // create a TCB for the just-completed thread
        tcb_t completed = (tcb_t)malloc(sizeof(struct tcb));
        // set its state
        running_t->state = ZOMBIE;
        // store into new TCB
        completed = running_t;
        // enqueue into zombie queue
        queue_enqueue(zombie_q,completed);
        // yield to next thread
        uthread_yield();
    }
}

int uthread_join(uthread_t tid, int *retval)
{  
    // create a TCB for the blocked thread
    tcb_t blocked = (tcb_t)malloc(sizeof(struct tcb));
    running_t->state = BLOCKED;
    // set its block_tid to child tid
    running_t->block_tid = tid;
    // change its state
    blocked = running_t;
    // enqueue into blocked queue
    queue_enqueue(blocked_q, blocked);
	while(1)
    {
     /* for later use in phase 3
        
        if(join_tid != 0){
            break;
	    }
        else{
            uthread_yield();
        } */
        if(queue_length(ready_q) == 0){
            break;
        }
        else{
            uthread_yield();        
        }
    }
    return *retval;
}

