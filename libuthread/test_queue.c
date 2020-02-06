#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "queue.h"

/*
    Iterate Test Functions
*/

int print_q(void *data, void *arg)
{
    int *a= (int*)data;
    printf("%d\n", *a);
    return 0;
}

int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;
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

int test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
    return 0;
}

int test_create(void){
    queue_t q;
    q = queue_create();
    assert(q != NULL);
    return 0;
}

int test_queue_simple(void){
    int n;
    queue_t q;
    int data = 3, *ptr;
    q = queue_create();
    queue_enqueue(q, &data);
    n = queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
    return n;
}

int bad_destroy(void){
    int data[] = {1,2,3};
    queue_t q;
    q = queue_create();
    queue_enqueue(q, &data[0]);
    return (queue_destroy(q));
}

int test_delete(void)
{
    queue_t q;
    int* data[] = {(int*)1, (int*)2, (int*)3, (int*)4, (int*)5, (int*)6};
    int i;
    int rtn;
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);
    rtn = queue_delete(q, &data[4]);
    
    queue_iterate(q, print_q, (void*)0, NULL);
    return rtn;
}

int main(){
    int rtn;

    rtn = test_create();
    printf("test_create() completed with %d\n", rtn);

    rtn = test_queue_simple();
    printf("test_queue_simple() completed with %d\n", rtn);

    rtn = bad_destroy();
    printf("bad_destroy() completed with %d\n", rtn);

    rtn = test_iterator();
    printf("test_iterator() completed with %d\n", rtn);

    rtn = test_delete();
    printf("test_delete() completed with %d\n", rtn);

    printf("Test Completed\n"); 
    return 0;
};
