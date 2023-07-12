#include "moderation_handler.h"

#include "server_state.h"
#include "user.h"
#include "server_responses.h"

bool is_client_host_admin(struct event *event){
    struct user *usr = event->client_persistent_data;

    return usr->is_admin;
}

bool is_client_mod_in_group(struct event *event, struct group_chat *gc){
    struct user *usr = event->client_persistent_data;
    
    return icol_contains(&gc->mods, usr->id) != NULL;
}

void permaban_user_handler(struct event *event){
    struct user *usr = event->client_persistent_data;
    struct user *usr_to_be_banned = ucol_find_user_by_id(&users, event->packet->header.target);

    if(is_client_host_admin(event) && usr_to_be_banned != NULL){
        usr_to_be_banned->is_banned = true;

        response_ACK(event);
    } else{
        response_NACK(event);
    }
}


void permaban_group(struct event *event){
    struct user *usr = event->client_persistent_data;
    struct group_chat *gc = gccol_find_chat_by_group_id(&gcs, event->packet->header.target);

    if(is_client_host_admin(event) && gc != NULL){
        gc->is_chat_banned = true;

        response_ACK(event);
    } else{
        response_NACK(event);
    }

}

void ban_user_from_group(struct event *event){
    struct user *usr = event->client_persistent_data;
    struct user *usr_to_be_banned = ucol_find_user_by_id(&users, event->packet->header.target);
    struct group_chat *gc = gccol_find_chat_by_group_id(&gcs, event->packet->header.receiver_id);

    if(gc != NULL && usr_to_be_banned != NULL && is_client_mod_in_group(event, gc)){
        icol_add_user(&gc->banned, usr_to_be_banned->id);

        response_ACK(event);
    } else{
        response_NACK(event);
    }

}