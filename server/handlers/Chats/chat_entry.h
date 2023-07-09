#pragma once

#include <stdint.h>

#include "double_linked_list.h"

enum CHAT_ENTRY_TYPES{
    ERROR_ENTRY = 0,
    MESSAGE
};

// must be in heap
struct chat_entry{
    uint32_t message_id;
    uint32_t chatter_id;
    enum CHAT_ENTRY_TYPES type;
    uint32_t load_length;
    char *load;
};

void chat_entry_init(struct chat_entry *entry);
void chat_entry_destroy(struct chat_entry *entry);

