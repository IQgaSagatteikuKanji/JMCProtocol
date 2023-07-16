#pragma once

#include <ncurses.h>

#include "client_chat_messages.h"
#include "packet_format.h"

struct messages{
    uint32_t height, width;
    uint32_t message_height, offset;
    WINDOW *field;
    const char *delimeter;
    struct client_dlist entries;
};


void messages_init(struct messages *msgs, uint32_t height, uint32_t width);
void messages_destroy(struct messages *msgs);

void messages_render(struct messages *msgs);

void messages_add_message(struct messages *msgs, struct packet *pack);
void messages_remove_message(struct messages *msgs, uint32_t message_id);