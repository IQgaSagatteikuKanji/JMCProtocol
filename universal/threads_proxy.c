#include "threads_proxy.h"

#include <assert.h>


int thread_create(struct thread *thr, struct thread_attributes *attributes, 
                    void *(*routine)(void *), void *params){
    assert(thr != NULL);
    assert(routine != NULL);
    
    if(attributes == NULL){
        return pthread_create(&thr->thread, NULL, routine, params);
    } else{
        return pthread_create(&thr->thread, attributes->attr, routine, params);
    }
}


void thread_exit(void *retval){
    pthread_exit(retval);
}

int thread_join(struct thread *thr, void ** data){
    assert(thr != NULL);

    return pthread_join(thr->thread, data);
}

int thread_cancel(struct thread *thr){
    assert(thr != NULL);

    return pthread_cancel(thr->thread);
}


int mutex_init(struct mutex *mutex){
    assert(mutex != NULL);

    return pthread_mutex_init(&mutex->mutex, NULL);
}

void mutex_destroy(struct mutex *mutex){
    assert(mutex != NULL);

    pthread_mutex_destroy(&mutex->mutex);
}

int mutex_lock(struct mutex *mutex){
    assert(mutex != NULL);

    return pthread_mutex_lock(&mutex->mutex);
}

int mutex_unlock(strict mutex *mutex){
    assert(mutex != NULL);

    return pthread_mutex_unlock(&mutex->mutex);
}