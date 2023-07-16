#include "client_chat_messages.h"

#include <assert.h>
#include <stdlib.h>


void client_dlist_node_init(struct client_dlist_node *node){
    assert(node != NULL);

    node->next = NULL;
    node->prev = NULL;

}

void client_dlist_node_destroy(struct client_dlist_node *node){
    assert(node != NULL);

    client_chat_entry_destroy(&node->msg);
}

void client_dlist_init(struct client_dlist *list, uint8_t render_depth){
    assert(list != NULL);

    list->head = NULL;
    list->tail = NULL;
    list->render_depth = render_depth;
}

void client_dlist_destroy(struct client_dlist *list){
    assert(list != NULL);
    
    client_dlist_clear(list);
    client_dlist_init(list, list->render_depth);
}

void client_dlist_add_to_head(struct client_dlist *list, struct client_chat_entry *elem){
    assert(list != NULL);

    struct client_dlist_node *buffer = calloc(1, sizeof(struct client_dlist_node));
    buffer->msg = *elem;
    buffer->next = list->head;
    buffer->prev = NULL;
    list->head = buffer;

    if(list->tail == NULL){
        list->tail = buffer;
    }
}

void client_dlist_add_to_tail(struct client_dlist *list, struct client_chat_entry *elem){
    assert(list != NULL);

    struct client_dlist_node *buffer = calloc(1, sizeof(struct client_dlist_node));
    buffer->msg = *elem;
    buffer->prev = list->tail;
    buffer->prev = NULL;
    list->tail = buffer;

    if(list->head == NULL){
        list->head = buffer;
    }
}

void client_dlist_remove_from_head(struct client_dlist *list){
    assert(list != NULL);
    assert(list->head != NULL);


    struct client_dlist_node *buffer = list->head;
    list->head = buffer->next;
    if(list->head != NULL){
        list->head->prev = NULL;
    }

    if(buffer->next == NULL){
        list->tail = NULL;
    }

    free(buffer);
}

void client_dlist_remove_from_tail(struct client_dlist *list){
    assert(list != NULL);
    assert(list->tail != NULL);


    struct client_dlist_node *buffer = list->tail;
    list->tail = buffer->prev;

    if(list->tail != NULL){
        list->tail->next = NULL;
    }

    if(buffer->prev == NULL){
        list->head = NULL;
    }

    free(buffer);
}

void client_dlist_remove_node(struct client_dlist_node **node){
    assert(node != NULL);
    assert(*node != NULL);

    struct client_dlist_node *to_delete = *node;
    *node = to_delete->next;
    if(to_delete->next != NULL){
        to_delete->next->prev = to_delete->prev;
    }

    client_dlist_node_destroy(to_delete);
    free(to_delete);
}

char client_dlist_remove_id(struct client_dlist *list, uint32_t id){
    assert(list != NULL);
    char deleted = 0;
    if(list->head != NULL){
        struct client_dlist_node **ptr = &list->head;

        while((*ptr) != NULL && !deleted){
            if((*ptr)->msg.message_id == id){
                deleted = 1;
                client_dlist_remove_node(ptr);
            } else{
                ptr = &(*ptr)->next;
            }
        }

    }
    return deleted;
}


void client_dlist_clear(struct client_dlist *list){
    assert(list != NULL);

    while(list->head->next != NULL){
        client_dlist_remove_node(&list->head);
    }
}


bool client_dlist_is_empty(struct client_dlist *list){
    return list->head == NULL;
}

struct client_dlist_node *client_dlist_find_node_by_message_id(struct client_dlist *list, uint32_t id){
    assert(list != NULL);

    struct client_dlist_node *node = list->head;

    while(node != NULL){
        if(node->msg.message_id == id){
            return node;
        }
    }

    return node;
}

void client_dlist_add_new_chat_entry(struct client_dlist *list, struct client_chat_entry *cent){
    client_dlist_add_to_head(list, cent);
}

void client_dlist_remove_chat_entry(struct client_dlist *list, uint32_t message_id){
    client_dlist_remove_id(list, message_id);
}