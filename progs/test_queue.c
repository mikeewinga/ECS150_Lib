#include <stdio.h>
#include <string.h>

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

    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++){
        queue_enqueue(q, &data[i]);
    }

    return 0;
};