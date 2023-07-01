#include "threads_proxy.h"

#include <assert.h>


int thread_create(struct thread *thr, struct thread_attributes *attributes, 
                    void *(*routine)(void *), void *params){
    assert(thr != NULL);
    assert(routine != NULL);
    
    if(attributes == NULL){
        return pthread_create(thr->thread, NULL, routine, params);
    } else{
        return pthread_create(thr->thread, attributes->attr, routine, params);
    }
}


void thread_exit(void *retval){
    pthread_exit(retval);
}

int thread_join(struct thread *thr, void ** data){
    assert(thr != NULL);

    return pthread_join(thr, data);
}

int pthread_cancel(struct thread *thr){
    assert(thr != NULL);

    return pthread_cancel(thr->thread);
}
