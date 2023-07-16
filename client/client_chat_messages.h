#pragma once

#include "client_chat_entry.h"


struct client_dlist_node{
    struct client_dlist_node *next, *prev;
    struct client_chat_entry msg;
};

struct client_dlist{
    struct client_dlist_node *head, *tail;
    uint8_t render_depth;    
};


void client_dlist_node_init(struct client_dlist_node *node);
void client_dlist_node_destroy(struct client_dlist_node *node);

void client_dlist_init(struct client_dlist *list, uint8_t render_depth);
void client_dlist_destroy(struct client_dlist *list);

void client_dlist_add_new_chat_entry(struct client_dlist *list, struct client_chat_entry *cent);
void client_dlist_remove_chat_entry(struct client_dlist *list, uint32_t message_id);

void client_dlist_clear(struct client_dlist *list);
bool client_dlist_is_empty(struct client_dlist *list);

struct client_dlist_node *client_dlist_find_node_by_message_id(struct client_dlist *list, uint32_t id);