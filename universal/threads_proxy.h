#pragma once
#include <pthread.h>

// it will follow POSIX standard 
// Not full functionality of threads is present here, will be added if needed
// All functions declared MUST be implemented


struct thread_attributes{
    pthread_attr_t *attr;
};

struct thread{
    pthread_t thread;
};


int thread_create(struct thread *thr, struct thread_attributes *attributes, void *(*routine)(void *), void *params);
void thread_exit(void *retval);

int thread_join(struct thread *thr, void ** data);
int pthread_cancel (struct thread *thr);
