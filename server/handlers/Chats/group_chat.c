#include "group_chat.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void group_chat_init(struct group_chat *gc){
    assert(gc != NULL);

    gc->chat_id = 0;
    gc->is_chat_banned = false;
    gc->next_id = 0;
    
    dlist_init(&gc->msgs);

    id_collection_init(&gc->banned);
    id_collection_init(&gc->mods);
    id_collection_init(&gc->listening);
}

void group_chat_destroy(struct group_chat *gc){
    assert(gc != NULL);

    dlist_destroy(&gc->msgs);

    id_collection_destroy(&gc->banned);
    id_collection_destroy(&gc->mods);
    id_collection_destroy(&gc->listening);
}

void gc_set_id(struct group_chat *gc, uint32_t id){
    assert(gc != NULL);

    gc->chat_id = id;
}

void gc_add_message(struct group_chat *gc, struct chat_entry *msg){
    assert(gc != NULL);

    msg->message_id = gc->next_id++;
    dlist_add_to_head(&gc->msgs, msg);
}

void gc_delete_message(struct group_chat *gc, uint32_t id){
    assert(gc != NULL);

    dlist_remove_id(&gc->msgs, id);
}

void gc_clear(struct group_chat *gc){
    assert(gc != NULL);

    dlist_clear(&gc->msgs);
}

//false if banned
bool gc_add_listener(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    icol_add_user(&gc->listening, user);
}

void gc_remove_listener(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    icol_remove_user(&gc->listening, user);
}

void gc_promote_user_to_moderator(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    icol_add_user(&gc->mods, user);
}

void gc_ban_user(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    icol_add_user(&gc->banned, user);
    icol_remove_user(&gc->listening, user);
    icol_remove_user(&gc->mods, user);
}

void gc_unban_user(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    icol_remove_user(&gc->banned, user);
}

bool gc_is_user_gc_moderator(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    return icol_contains(&gc->mods, user);
}

bool gc_is_user_banned_in_gc(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    return icol_contains(&gc->banned, user);
}

bool gc_is_user_listening_to_gc(struct group_chat *gc, uint32_t user){
    assert(gc != NULL);

    return icol_contains(&gc->listening, user);
}

struct id_collection *gc_get_list_of_listeners(struct group_chat *gc){
    assert(gc != NULL);


    //if you change logic here and allocate memory, then go and free it everywhere this function is used!
    return &gc->listening;
}