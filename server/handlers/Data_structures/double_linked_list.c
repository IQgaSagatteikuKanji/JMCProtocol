#include "double_linked_list.h"

#include <stdlib.h>
#include <assert.h>


void dlist_node_init(struct dlist_node *node){
    assert(node != NULL);

    node->next = NULL;
    node->prev = NULL;

    chat_entry_init(&node->msg);
}

void dlist_node_destroy(struct dlist_node *node){
    assert(node != NULL);

    chat_entry_destroy(&node->msg);
}

void dlist_init(struct dlist *list){
    assert(list != NULL);

    list->head = NULL;
    list->tail = NULL;
}

void dlist_destroy(struct dlist *list){
    assert(list != NULL);
    
    dlist_clear(list);
    dlist_init(list);
}

void dlist_add_to_head(struct dlist *list, struct chat_entry *elem){
    assert(list != NULL);

    struct dlist_node *buffer = calloc(1, sizeof(struct dlist_node));
    buffer->msg = *elem;
    buffer->next = list->head;
    buffer->prev = NULL;
    list->head = buffer;

    if(list->tail == NULL){
        list->tail = buffer;
    }
}

void dlist_add_to_tail(struct dlist *list, struct chat_entry *elem){
    assert(list != NULL);

    struct dlist_node *buffer = calloc(1, sizeof(struct dlist_node));
    buffer->msg = *elem;
    buffer->prev = list->tail;
    buffer->prev = NULL;
    list->tail = buffer;

    if(list->head == NULL){
        list->head = buffer;
    }
}

void dlist_remove_from_head(struct dlist *list){
    assert(list != NULL);
    assert(list->head != NULL);


    struct dlist_node *buffer = list->head;
    list->head = buffer->next;
    if(list->head != NULL){
        list->head->prev = NULL;
    }

    if(buffer->next == NULL){
        list->tail = NULL;
    }

    free(buffer);
}

void dlist_remove_from_tail(struct dlist *list){
    assert(list != NULL);
    assert(list->tail != NULL);


    struct dlist_node *buffer = list->tail;
    list->tail = buffer->prev;

    if(list->tail != NULL){
        list->tail->next = NULL;
    }

    if(buffer->prev == NULL){
        list->head = NULL;
    }

    free(buffer);
}

void dlist_remove_node(struct dlist_node **node){
    assert(node != NULL);
    assert(*node != NULL);

    struct dlist_node *to_delete = *node;
    *node = to_delete->next;
    if(to_delete->next != NULL){
        to_delete->next->prev = to_delete->prev;
    }

    dlist_node_destroy(to_delete);
    free(to_delete);
}

void dlist_remove_id(struct dlist *list, uint32_t id){
    assert(list != NULL);
    if(list->head != NULL){
        struct dlist_node **ptr = &list->head;
        char deleted = 0;

        while((*ptr)->next != NULL && !deleted){
            if((*ptr)->next->msg.message_id == id){
                deleted = 1;
                dlist_remove_node(ptr);
            } else{
                ptr = &(*ptr)->next;
            }
        }

    }
}


void dlist_clear(struct dlist *list){
    assert(list != NULL);

    while(list->head->next != NULL){
        dlist_remove_after(list->head);
    }
}


void dlist_is_empty(struct dlist *list){
    return list->head == NULL;
}