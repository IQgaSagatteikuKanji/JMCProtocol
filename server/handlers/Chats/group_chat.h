#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "chat_entry.h"
#include "id_collection.h"
#include "user.h"


//also needs mutex
struct group_chat{
    uint32_t chat_id;
    struct id_collection mods;
    struct id_collection banned;
    struct id_collection listening;
    struct dlist msgs;
    bool is_chat_banned;
};


void group_chat_init(struct group_chat *gc);
void group_chat_destroy(struct group_chat *gc);

void gc_set_id(struct group_chat *gc, uint32_t id);

void gc_add_message(struct group_chat *gc, struct chat_entry *msg);
void gc_delete_message(struct group_chat *gc, uint32_t id);
void gc_clear(struct group_chat *gc);

//false if banned
bool gc_add_listener(struct group_chat *gc, uint32_t user);
void gc_remove_listener(struct group_chat *gc, uint32_t user);
void gc_promote_user_to_moderator(struct group_chat *gc, uint32_t user);
void gc_ban_user(struct group_chat *chat, uint32_t user);
void gc_unban_user(struct group_chat *chat, uint32_t user);

bool gc_is_user_gc_moderator(struct group_chat *chat, uint32_t user);
bool gc_is_user_banned_in_gc(struct group_chat *chat, uint32_t user);
bool gc_is_user_listening_to_gc(struct group_chat *chat, uint32_t user);

struct id_collection *gc_get_list_of_listeners(struct group_chat *chat);