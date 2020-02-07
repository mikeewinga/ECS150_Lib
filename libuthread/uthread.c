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
<<<<<<< HEAD
    // if the currently running thread has already been enqueued
    if(running_t->state == BLOCKED || running_t->state == ZOMBIE){
        tcb_t ready = running_t;
        tcb_t current_thread = running_t;
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**) ready);
        // change its state
=======
    printf("thread yielded\n");
    tcb_t ready=(tcb_t)malloc(sizeof(tcb_t));
    if(running_t->state == BLOCKED || running_t->state == ZOMBIE){
        tcb_t current_thread = (tcb_t)malloc(sizeof(tcb_t));
        current_thread = running_t; 
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**)ready);
        // change their states respct.
>>>>>>> acf60f06deb55ce0b8f731f259a210f629f71332
        ready->state = RUNNING;
        running_t = ready;
        uthread_ctx_switch(current_thread->context, running_t->context);
    }
    else{
<<<<<<< HEAD
        tcb_t ready = running_t; 
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**)ready);
        // store the running thread
        tcb_t yielding = running_t;
        // change their states respctively
        running_t->state = READY;
        ready->state = RUNNING;
        // enqueue running thread into ready
        queue_enqueue(ready_q, (void*)running_t);
        // replace running thread with ready thread
=======
        tcb_t yielding = (tcb_t)malloc(sizeof(tcb_t)); 
        // dequeue the next ready thread
        queue_dequeue(ready_q, (void**)ready);
        // store the running thread
        yielding = running_t;
        // change their states respct.
        running_t->state = READY;
        ready->state = RUNNING;
        // move the threads
        queue_enqueue(ready_q, (void*)running_t);
>>>>>>> acf60f06deb55ce0b8f731f259a210f629f71332
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
    printf("thread created\n");
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
<<<<<<< HEAD
    queue_enqueue(ready_q, (void*)new_tcb);
    return new_tcb->tid;
=======

    queue_enqueue(ready_q, (void*)tcb);
    return tcb->tid;
>>>>>>> acf60f06deb55ce0b8f731f259a210f629f71332
}

void uthread_exit(int retval)
{
<<<<<<< HEAD
    // create a TCB for the just-completed thread
    tcb_t completed;
    // set its state
    running_t->state = ZOMBIE;
    // store into new TCB
    completed = running_t;
    // enqueue into zombie queue
    queue_enqueue(zombie_q, (void*)completed);
    // yield to next thread
    uthread_join(completed->tid, &retval);
=======
    printf("thread exited\n");
    if(retval == 0){
        tcb_t completed;
        running_t->state = ZOMBIE;
        completed = running_t;
        queue_enqueue(zombie_q, (void*)completed);
        
/*      
        if(completed->block_tid != 0){
            uthread_join()
        }
*/
        uthread_yield();
    }
>>>>>>> acf60f06deb55ce0b8f731f259a210f629f71332
}

int uthread_join(uthread_t tid, int *retval)
{  
<<<<<<< HEAD
    // create a TCB for the blocked thread
=======
    printf("thread blocked\n");
>>>>>>> acf60f06deb55ce0b8f731f259a210f629f71332
    tcb_t blocked;
    running_t->state = BLOCKED;
    blocked = running_t;
<<<<<<< HEAD
=======
    queue_enqueue(blocked_q, (void*)blocked);
>>>>>>> acf60f06deb55ce0b8f731f259a210f629f71332
	while(1)
    {
        /*        
        if(join_tid != 0){
            break;
	    }
        else{
            uthread_yield();
<<<<<<< HEAD
        } */
        if(blocked->tid == tid){
            tcb_t zombie = running_t;
            queue_dequeue(zombie_q, (void**)zombie);
            uthread_ctx_destroy_stack(zombie->stack);
            free(zombie);
=======
        }
        */
        if(queue_length(ready_q) == 0){
>>>>>>> acf60f06deb55ce0b8f731f259a210f629f71332
            break;
        }
        else{
            queue_enqueue(blocked_q, (void*)blocked);
            uthread_yield();        
        }
    }
    tid += 1;
    tid -= 1;
    *retval += 1;
    *retval -= 1;
    return *retval;
}

