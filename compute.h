#ifndef __COMPUTE_H
#define __COMPUTE_H
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef unsigned long long int ull;

struct ThreadTable {
    pthread_t* thread;
    ull lower_bound;
    ull upper_bound;
    bool* finished_element;
    ull* primes;
    long long* count_element;
};

/* 
* _ThreadTable is a pointer to the thread info typecasted
* as a void*.
*/

void* computeBounds(void* _ThreadTable); // (void*)(ThreadTable*)

#endif