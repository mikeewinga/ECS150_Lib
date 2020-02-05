#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "queue.h"

/*
    Iterate Test Functions
*/
int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;
    //printf("new value: %d\n", *a);
    return 0;
}

int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
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
    int n;
    q = queue_create();
    n = queue_enqueue(q, &data);
    printf("(%d) added to queue with return[%d]\n", data, n);
    queue_dequeue(q, (void**)&ptr);
    printf("(%d) removed from queue with return[%d]\n", *((int*)ptr), n);
    assert(ptr == &data);
}

void bad_destroy(void){
    int n;
    int data[] = {1,2,3};
    queue_t q;
    q = queue_create();
    n = queue_enqueue(q, &data[0]);
    printf("(%d) added to queue with return[%d]\n", data[0], n);
    n = queue_destroy(q);
    printf("queue destroyed with return[%d]\n", n);
}

/*
    Main
*/
int main(){
    queue_t q;
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int i;
    int *ptr;

    printf("======================\n");
    
    test_queue_simple();
    printf("======================\n");

    bad_destroy();
    printf("======================\n");

// create queue + enqueue + iterate
    test_iterator();

    printf("Test Completed\n"); 
    return 0;
};
