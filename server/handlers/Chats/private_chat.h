#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "double_linked_list.h"


struct private_chat{
    uint32_t users[2];
    uint32_t current_message_id;
    uint32_t next_id;
    struct dlist msgs;
    //STORED: 1 if the first user blocked it, 2 if the second user blocked it
    //RETURNS as an index
    unsigned char is_blocked_by;
};

void private_chat_init(struct private_chat *pc);
void private_chat_destroy(struct private_chat *pc);

void pc_set_recipients(struct private_chat *pc, uint32_t first, uint32_t second);
void pc_get_recipients(struct private_chat *pc, uint32_t *first, uint32_t *second);


bool pc_add_message(struct private_chat *pc, struct chat_entry *msg);
bool pc_delete_message(struct private_chat *pc, uint32_t id);
void pc_clear(struct private_chat *pc);

bool pc_is_pc_blocked(struct private_chat *pc);
unsigned char pc_chat_was_blocked_by_index(struct private_chat *pc);

