#pragma once 

#include <stdint.h>
#include <stdbool.h>

#include "packet_format.h"

enum CHAT_ENTRY_TYPE{
    NULL_CHAT_ENTRY,
    PRIVATE_MESSAGE,
    GROUP_MESSAGE
};

// must be in heap
struct client_chat_entry{
    uint32_t message_id;
    uint32_t chatter_id;
    uint32_t chat_id;
    uint32_t load_length;
    char *load;
    enum CHAT_ENTRY_TYPE type;
};

void client_chat_entry_init(struct client_chat_entry *cent);
void client_chat_entry_destroy(struct client_chat_entry *cent);

void ccent_build_from_packet(struct client_chat_entry *cent, struct packet *pack);