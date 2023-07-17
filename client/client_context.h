#pragma once

#include "client_console.h"
#include "client_messages_renderer.h"
#include "packet_format.h"

struct ncscr_info{
    //I don't want to deal with resizes for now, so I expect you to never do such a thing
    uint32_t height, width;
    struct client_console ucon;
    struct messages messages;
    uint32_t username;
};

void ncscr_info_init(struct ncscr_info *info);
void ncscr_info_destroy(struct ncscr_info *info);

void ncscr_dedicate_thread_to_managing_user_input(struct ncscr_info *info);
void ncscr_refresh_chat(struct ncscr_info *info);

void ncscr_add_message(struct ncscr_info *info, struct packet *pack);
void ncscr_remove_message(struct ncscr_info *info, uint32_t message_id, uint32_t chat_id, int type_of_chat);

void ncscr_stop_serving_clients(struct ncscr_info *info);