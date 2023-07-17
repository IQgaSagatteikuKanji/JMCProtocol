#include "client_context.h"

#include <assert.h>
#include <stdlib.h>
#include <ncurses.h>

void ncscr_info_init(struct ncscr_info *info){
    assert(info != NULL);

    getmaxyx(stdscr, info->height, info->width);
    client_console_init(&info->ucon, info->width, info->height);

    messages_init(&info->messages, info->height - info->ucon.height - 1, info->width);

    refresh();
}

void ncscr_info_destroy(struct ncscr_info *info){
    assert(info != NULL);

    client_console_destroy(&info->ucon);
    messages_destroy(&info->messages);
}

void ncscr_dedicate_thread_to_managing_user_input(struct ncscr_info *info){
    assert(info != NULL);

    ccon_manage_input(&info->ucon);
}

void ncscr_refresh_chat(struct ncscr_info *info){
    assert(info != NULL);

    int y,x;
    getyx(info->ucon.border, y, x);

    messages_render(&info->messages);

    wmove(info->ucon.border, y, x);
}

void ncscr_add_message(struct ncscr_info *info, struct packet *pack){
    assert(info != NULL);
    assert(pack != NULL);

    messages_add_message(&info->messages, pack);
}

void ncscr_remove_message(struct ncscr_info *info, uint32_t id, uint32_t chat_id, int type_of_chat){
    assert(info != NULL);

    messages_remove_message(&info->messages, id, chat_id, type_of_chat);
}

void ncscr_stop_serving_clients(struct ncscr_info *info){
    //its ugly, but I don't have time to fix it
    serving = false;
}