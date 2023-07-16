#pragma once

#include <stdbool.h>

#include "chat_entry.h"


//I tried using lists as in linux core, but it gets confusing really fast when chat_entry also has pointers to data on heap

struct dlist_node{
    struct dlist_node *next, *prev;
    struct chat_entry msg;
};

struct dlist{
    struct dlist_node *head, *tail;
};


void dlist_node_init(struct dlist_node *node);
void dlist_node_destroy(struct dlist_node *node);

void dlist_init(struct dlist *list);
void dlist_destroy(struct dlist *list);

void dlist_add_to_head(struct dlist *list, struct chat_entry *elem_to_add);
void dlist_add_to_tail(struct dlist *list, struct chat_entry *elem_to_add);

void dlist_remove_from_head(struct dlist *list);
void dlist_remove_from_tail(struct dlist *list);

int dlist_remove_id(struct dlist *list, uint32_t id);

void dlist_clear(struct dlist *list);
bool dlist_is_empty(struct dlist *list);

struct dlist_node *dlist_find_node_by_message_id(struct dlist *list, uint32_t id);