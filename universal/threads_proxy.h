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

struct mutex{
    pthread_mutex_t mutex;
};


int thread_create(struct thread *thr, struct thread_attributes *attributes, void *(*routine)(void *), void *params);
void thread_exit(void *retval);

int thread_join(struct thread *thr, void ** data);
int thread_cancel(struct thread *thr);


int mutex_init(struct mutex *mutex);
void mutex_destroy(struct mutex *mutex);

int mutex_lock(struct mutex *mutex);
int mutex_unlock(struct mutex *mutex);

