# Project #2 - User-level thread library #  

## Phase 1: queue API ##

* ### Data Structure: Doubly Linked List ###

  * A doubly linked list was chosen over a singly linked list to implement the
    queue within the design constraints of O(1) time for operations excluding
    iterate and delete.
  * **Advantages**:
    * Insertion and removal of nodes in O(1) time versus O(n) time in a singly
      linked list
    * Efficient iteration as a doubly linked list can be traversed forwards and
      backwards
  * **Disadvantages**:
    * Two pointers, next and previous, are required for all operations
    * This extra pointer requires more space and adds complexity to
      implementation

* ### Implementation ###

  * `struct node`  stores pointers to the previous and next nodes in the queue
  * `struct queue` stores pointers to the head and the tail of the queue as well
    as the number of nodes in the queue
  * The key to the queue operations was the proper use of the prev and next
    pointers which are vital to a doubly linked list  
  * `node_create` allocates space for a new node
  * `queue_create` allocates space for an empty queue and initalizes `struct
    queue`
  * `queue_destroy` deallocates memory to the queue if it is not `NULL` or it is
    empty.
  * `queue_enqueue` calls `node_create` to allocate a new node and increments
    the number of nodes in the queue.
    * If the queue is empty, the previous and next nodes point to `NULL` and the
      head and tail of the queue are set to be the new node
    * If the queue is not empty, the new node becomes the tail of the queue and
      the previous and next pointers of the new node and previous pointer of the
      tail node are adjusted to accommodate this.  
  * `queue_dequeue` removes the oldest element of a non-empty queue which will
    be at the head and decrements the number of nodes.
    * If there is only one node in the queue the head and tail will be set to
        `NULL`
    * Otherwise the next and previous pointers to the new head and old head will
        be adjusted to accommodate dequeueing.
  * `queue_delete` loops from the head of the queue to the tail and deletes the
    first node equal to the @data passed. The loop breaks after deletion or
    returns an error value upon reaching the tail
  * `queue_iterate` is used to apply a function, one that does not modify the
    queue itself, to each element in the queue from head to tail.
    * This function could be used to debug queues of threads through printing,
      but we were unable to use it to its fullest potential outside of queue
      testing due to time constraints.
  * `queue_length` simply returns the number of nodes in the queue that have
    been incremented and decremented by `queue_enqueue` and `queue_dequeue`
    respectively.

* ### Queue Test ###  

  * We adopted the approach of unit testing in order to provide a rigorous
    review of our queue implementation.
  * All operations should be checked for return values of -1 to indicate an
    error has occurred with the operation
  * There are currently 5 tests which involve creating, destroying, iterating,
    and deleting queues as well as performing enqueue and dequeue operations on
    these queues.
  * More testing should be done to properly test for edge cases and prevent
    problems when implementing these queues for the purpose of storing threads.

___

## Phase 2: uthread API ##

* ### Thread Control Block ###

  * `struct tcb` was created in order to store a thread identifier, a pointer to
    its context, the stack, the state, and a blocked thread id.
  * The queue API was used to create three queues which were implemented
    globally including a ready, blocked, and zombie queue which store threads in
    these respective states

* ### uthread_create()  ###

  * This function creates a thread and initializes the global queues if it is
    the first thread.
  * The thread properties are updated in the thread control block including a
    status change to READY and it is then enqueued into the ready queue

* ### uthread_yield() ###  

  * Thread yielding is done first by allocating memory to the next ready thread.
    * If the current running thread has already been enqueued then the next
      ready thread is dequeued. Its state is changed to running and the running
      thread is replaced with the ready one before a context switch to make sure
      that the context will be changed to the next thread in queue.
    * If both the running thread and ready thread need to be enqueued then the
      next ready thread is dequeued and the running thread is stored. Then their
      states are changed from ready to running and running to ready
      respectively. The running thread will be enqueued to the ready thread and
      then replaced with the ready thread. Finally the context will be switched.

* ### uthread_exit() ###  

  * Exiting threads would have a temporary TCB created where they would be set
    to zombie status and then enqueued into the zombie queue before yielding to
    the next thread

## Phase 2/3: uthread_join() ##  

* ### Initial (Phase 2) Design ###

  * Initial design of `uthread_join` involves an infinite loop which breaks and
    returns when no more threads are ready to run or yields to the next
    available thread.
  * In the current state of the function, a TCB is created for the blocked
    thread and its `block_tid` is set to the child tid. Its state is then
    changed and it is enqueued into the blocked queue before we enter the
    infinite loop.

* ### Phase 3 Design Goals ###  

  * Although we were unable to get to phase 3, the design goals included an
    implementation that would have `uthread_join` properly accommodate the two
    scenarios in which a thread T1 joins a thread T2 where:
    * If T2 is an active thread, T1 is blocked until T2 dies and T1 can collect it
    * If T2 is dead, T1 will not need to be blocked and can be collected right away
  * Collection involves freeing resources and would allow for proper memory deallocation
  * We would try to implement proper scheduling by allowing an unblocked T1 to
    run only after all other runnable threads have run

## uthread API Testing ##

* We were unable to test beyond `uthread_hello` and `uthread_yield` as we were
  unable to complete these tests themselves due to a seg fault.

___

## Reflection ##  

* We found that a singly linked list would be easier to implement, but would not
  perform insertion in O(1) time.
* Pointers and programming language C's utilization of memory was a troublesome
  aspect of the assignment which took copious debugging
* Statically allocated memory to store important data was problematic due to
  overwrite problems and other memory problems when going out of scope for the
  corresponding variable  
* We got around this by dynamically allocating memory as much as possible
  instead of statically allocating memory

## Limitations ##  

* Tests uthread_hello and uthread_yield were unable to be completed in Phase 2.
* We were unable to debug `uthread.c` enough to fix a seg fault. Further
  testing by debugging through gdb and properly tracing pointers and thread
  memory usage would be vital to fixing this.
* I (Navjot Mattu) was unable to provide proper support to my partner due to my
  own personal issues in the timeframe of this project. I think proper planning
  in the very beginning of a project and timelines on phase completion to allow
  enough time to fully understand and debug a program despite any personal
  setbacks is important and would be a priority if I were to have the chance to
  do this project again.

## References ##

[Make file](http://tldp.org/HOWTO/Program-Library-HOWTO/static-libraries.html)  
