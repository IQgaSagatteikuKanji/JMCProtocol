#include "private_chat.h"

#include <assert.h>
#include <stdlib.h>

void private_chat_init(struct private_chat *pc){
    assert(pc != NULL);

    pc->current_message_id = 0;
    pc->is_blocked_by = 0;
    pc->next_id = 0;

    dlist_init(&pc->msgs);
    pc->users[0] = 0;
    pc->users[1] = 0;
}

void private_chat_destroy(struct private_chat *pc){
    assert(pc != NULL);

    dlist_destroy(&pc->msgs);
}

void pc_set_recipients(struct private_chat *pc, uint32_t first, uint32_t second){
    assert(pc != NULL);

    pc->users[0] = first;
    pc->users[1] = second;
}

bool pc_add_message(struct private_chat *pc, struct chat_entry *msg){
    assert(pc != NULL);
    assert(msg != NULL);

    msg->message_id = pc->next_id++;
    dlist_add_to_head(&pc->msgs, msg);
}

bool pc_delete_message(struct private_chat *pc, uint32_t id){
    assert(pc != NULL);

    dlist_remove_id(&pc->msgs, id);
}

void pc_clear(struct private_chat *pc){
    assert(pc != NULL);

    dlist_clear(&pc->msgs);
}


//they return the same field for now, but if I would need to change it later, it might be better to distinguish
bool pc_is_pc_blocked(struct private_chat *pc){
    assert(pc != NULL);

    return pc->is_blocked_by;
}

unsigned char pc_chat_was_blocked_by_index(struct private_chat *pc){
    assert(pc != NULL);

    return pc->is_blocked_by;
}


void pc_get_recipients(struct private_chat *pc, uint32_t *first, uint32_t *second){
    assert(pc != NULL);

    if(first != NULL){
        *first = pc->users[0];
    }

    if(second != NULL){
        *second = pc->users[1];
    }
}