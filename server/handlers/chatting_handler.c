#include "chatting_handler.h"

#include "server_state.h"
#include "user_collection.h"
#include "user.h"
#include "server_responses.h"
#include "pc_collection.h"
#include "gc_collection.h"

void message_in_existing_private_chat(struct event *event, struct private_chat *pc){

}

void private_message(struct event *event){
    struct user *usr = event->client_persistent_data;
    uint32_t sender = event->packet->header.sender_id;
    uint32_t receiver = event->packet->header.receiver_id;

    struct private_chat *pc = pccol_find_chat_by_two_users(&pcs, sender, receiver);
    if(pc != NULL){
        //if such a chat exists

        if(!pc_is_pc_blocked(pc)){
            message_in_existing_private_chat(event, pc);
            response_ACK(event);
        } else{
            response_NACK(event);
        }

    } else if(ucol_find_user_by_id(&users, receiver) != NULL){
        //if such a chat doesn't exist and the user exists
        pc = create_new_private_chat_between_users(&pcs, sender, receiver);
        message_in_existing_private_chat(event, pc);
        response_ACK(event);
    } else{
        //if chat doesn't exist and the user doesn't exist deny
        response_NACK(event);
    }
}

void send_message_in_existing_group_chat(struct event *event, struct group_chat *gc){
    
}

void group_message(struct event *event){
    struct group_chat *gc = gccol_find_chat_by_group_id(&gcs, event->packet->header.receiver_id);

    if(gc != NULL){
        send_message_in_existing_group_chat(event, gc);
        response_ACK(event);
    } else{
        response_NACK(event);
    }
}

void delete_group_message(struct event *event);
void delete_private_message(struct event *event);

void edit_group_message(struct event *event);
void edit_private_message(struct event *event);

void create_group(struct event *event);
void join_group(struct event *event);
void leave_group(struct event *event);