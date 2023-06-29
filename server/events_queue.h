#pragma once
#include "queue.h"
#include "events.h"

// Looks like a bottleneck, will probably not use it
// Used anywhere: false

//event queue
struct event_queue{
    struct queue queue;
    struct event event;
};
//synchronization primitives for event queue
// Mutex to safeguard
// Semaphor to track size

void event_q_init();
void event_q_destroy();

void event_q_push();
void event_q_pop();
void event_q_peek();