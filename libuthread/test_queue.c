#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "queue.h"

/*
    Iterate Test Functions
*/
static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}

/*
    Queue Test Functions
*/
void test_create(void){
    queue_t q;
    q = queue_create();
    assert(q != NULL);
}

void test_queue_simple(void){
    queue_t q;
    int data = 3, *ptr;
    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

/*
    Main
*/
int main(){
    queue_t q;
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int i;
    int *ptr;
    
    int n=0;
/*
    queue_t new_q = queue_create();
    printf("queue created with return[%d]\n", n);
    n = queue_destroy(new_q);
    printf("queue destroyed with return[%d]\n", n);
    
    printf("======================\n");


    new_q = queue_create();
    printf("queue created with return[%d]\n", n);
    
    n = queue_enqueue(new_q, &data[0]);
    printf("(%d) added to queue with return[%d]\n", data[0], n);

    n = queue_destroy(new_q);
    printf("queue destroyed with return[%d]\n", n);
    
    printf("======================\n");

// create queue + enqueue + dequeue + destroy
    queue_t Q;
    int m=0;
    Q = queue_create();
    printf("queue created with return[%d]\n", m);
    printf("Number of items in queue: %d\n", queue_length(Q));
    
    m = queue_enqueue(Q, &data[3]);
    printf("(%d) added to queue with return[%d]\n", data[3], m);
    printf("Number of items in queue: %d\n", queue_length(Q));
    
    m = queue_dequeue(Q, (void*)&data[3]);
    printf("(%d) removed from queue with return[%d]\n", data[3], m);
    printf("Number of items in queue: %d\n", queue_length(Q));

    m = queue_destroy(Q);
    printf("queue destroyed with return[%d]\n", m);

    printf("======================\n");
*/

// create queue + enqueue + iterate
   
     q = queue_create();
    int p=0;
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++){
        queue_enqueue(q, &data[i]);
    }
        
    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);
   

    return 0;
};
