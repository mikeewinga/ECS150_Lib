# Project #2 - User-level thread library #  

We implemented a basic user-level thread library for Linux.

___

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
  * `queue_length` simply returns the number of nodes in the queue that have
    been incremented and decremented by `queue_enqueue` and `queue_dequeue`
    respectively. Returns an error if the queue is `NULL`

* ### Queue Test ###  

  * We adopted the approach of unit testing in order to provide a rigorous
    review of our queue implementation and to identify any edge cases we did not
    account for.
  * Test for any return values of -1 in all functions
  * TODO ADD TESTS  

___

## Phase 2: uthread API ##

* ### Thread Definition  ###

  * Implemented by setting the exit variable equal to 1 when the exit command is
    run
  * This prevents the shell from looping back  
  * Prints "Bye..."  

* ### Public API ###

  * cd is implemented by using the **chdir** system call that takes directory
    and changes to that directory  
  * Contains an error check in case that directory does not exist
  * pwd is implemented by using the getcwd system call and prints the output

* ### Thread Control Block ###  

  * When parsing the command the program attempts to open the file and on
    success the file descriptor is stored in the **command struct**
  * There is also a boolean variable in the **command struct** that reports
    whether or not there is output redirection  
  * If there is output redirection and the file descriptor is not **NULL** then
    when executing our shell will write the output to the file described  

## CHANGE AS NEEDED ##  

* ### Fasdasd ###  

  * When parsing the command the program attempts to open the file and on
    success the file descriptor is stored in the **command struct**
  * There is also a boolean variable in the **command struct** that reports
    whether or not there is output redirection  
  * If there is output redirection and the file descriptor is not **NULL** then
    when executing our shell will write the output to the file described  

## Piping ##  
  
* Each command is stored in an array of size 4 which is the maximum number of
  pipes possible within our shell plus 1  
* Each command is executed starting from the first command to the last command
  in order  
* For each command we use dup2 to duplicate the read portion of the pipe to
  STDIN_FILENO and then we close the pipe and open a new pipe  
* As long as the command is not the last command we use dup2 to duplicate the
  write portion of the pipe to STDOUT_FILENO. If the user specified piping error
  redirection then we also set the write portion of the pipe to STDERR_FILENO  
* If it is the last command, then we restore stdout to STDOUT_FILENO unless the
  user specified a file output redirection  
* This has the effect of passing the output of one program to the input of the
  following program  

## Error Management ##  

* ### Failure of Library Functions ###  

  * Library functions will fail due to inappropriate memory allocation using
    malloc() which has not occured... yet

* ### Standard Error Redirection ###  

  * While parsing the command in addition to checking for the output redirection
    symbol we also checked for the & symbol immediately after for standard error
    redirection
  * If this exists then during piping we will duplicate the pipe to the
    **STDERR_FILENO** in addition to the **STDOUT_FILENO**  
  * We also have to store a copy of the default stderr file descriptor so that
    it is not lost during piping  

## Directory Stack ##  

* ### Stack ###  

  * A linked list chosen to create a dynamically sized stack that will store
    remembered directories
  * dirstack employs two structs **struct node** and **struct stack** which use
    pointers to track directories going into the stack and the head of the
    stack, and an int variable to store the size of the stack.  
  * Debugging by tracing pointers and ensuring proper cstring manipulation  
    when dealing with directories
  * Errors handled during parse rather than each individual builtin
  * Errors involving popping from an empty stack utilized the size of the stack
    to determine when it is empty  

* ### pushd ###  

  * The command **pushd** and its directory argument are parsed in  
    **sshell.c**, the directory is pushed onto the stack, and then current
    working directory is changed using **chdir** to the directory pushed
  * Error handled when argument is a directory that does not exist by the output
    of **chdir**  

* ### popd ###  

  * The head of the stack is popped and **chdir** is used to change to the next
    directory which is now at the head of the stack  
  * Using the tester we found that we were changing to the wrong directory by  
    one element of the stack which was debugged by making sure the directory was
    changed to the new head of the stack, not the directory popped off the
    stack.

* ### dirs ###  

  * Simple loop printing out directories starting from the head and iterating
    through each directory pointed to by the **next** pointer until it points to
    **NULL**

## Reflection and Limitations ##  

* We found that queues were a suitable data structure when dealing with piping
  but lacked the time to properly incorporate them into the shell. We ended up
  using an array that has the side effect of only being able to include a fixed
  amount of pipes.  
* Pointers and programming language C's utilization of memory was a troublesome
  aspect of the assignment which took copious debugging
* Statically allocated memory to store important data was problematic due to
  overwrite problems and other memory problems when going out of scope for the
  corresponding variable  
* We got around this by dynamically allocating memory as much as possible
  instead of statically allocating
* However this caused memory leaks when not freeing the pointers which had to be
  meticulously found and fixed  
* The example program used in class for viewing the file descriptors was
  extremely helpful for the piping aspect of the assignment. What we thought was
  going to be a very difficult part turned out to be managable with this tool.

## Sources ##  

* [Linked List Stack](https://www.zentut.com/c-tutorial/c-stack-using-pointers/)
