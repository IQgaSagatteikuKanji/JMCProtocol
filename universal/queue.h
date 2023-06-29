#pragma once

struct queue{
    struct queue *next;
};

void queue_init();
void queue_destroy();

void queue_push();
void queue_pop();
void queue_peek();

void queue_clear();