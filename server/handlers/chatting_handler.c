#include "chatting_handler.h"

#include <stdlib.h>
#include <string.h>

#include "server_state.h"
#include "user_collection.h"
#include "user.h"
#include "server_responses.h"
#include "pc_collection.h"
#include "gc_collection.h"
#include "chat_entry.h"
#include "packet_format.h"

//TODO: refactor this file, there are a lot of functions that shouldn't be here

//should probably move it to chat_entry later
struct chat_entry *build_chat_entry_from_packet(struct packet *pack){
    struct chat_entry *cent = calloc(1, sizeof(struct chat_entry));
    chat_entry_init(cent);
    
    cent->chatter_id = pack->header.sender_id;
    cent->type = pack->header.op_code;
    
    cent->load_length = pack->header.payload_length;
    if(pack->header.payload_length != 0){
        cent->load = calloc(1, cent->load_length);
    
        strncpy(cent->load, pack->payload, cent->load_length);
    }

    return cent;
}


//this should go to server_responses
//If I'll need to keep track of unsent messages, this could be a good point to do it
void send_message_to_user(struct server *server,struct packet *pack, uint32_t global_user_id){
    struct user *usr = ucol_find_user_by_id(&users, global_user_id);

    if(usr != NULL && usr->is_logged_in){
        server_send_message(server, usr->logged_in_from, pack);
    }
}


#define NUMBER_OF_USERS_IN_PRIVATE_CHAT 2
void relay_message_in_private_chat(struct server *server, struct private_chat *pc, struct packet *msg, uint32_t ignore){
    uint32_t users[NUMBER_OF_USERS_IN_PRIVATE_CHAT];
    pc_get_recipients(pc, users, users + 1);

    for(uint32_t i = 0; i < NUMBER_OF_USERS_IN_PRIVATE_CHAT; i++){
        if(users[i] != ignore){
            send_message_to_user(server, msg, users[i]);
        }
    }
}


// I don't think this should be moved anywhere, or I can create some new header
uint32_t privmsg_in_existing_private_chat(struct event *event, struct private_chat *pc){
    struct chat_entry *cent = build_chat_entry_from_packet(event->packet);
    pc_add_message(pc, cent);
    uint32_t message_id = event->packet->header.message_id = cent->message_id;

    relay_message_in_private_chat(event->server, pc, event->packet, event->packet->header.sender_id);

    free(cent);

    return message_id;
}


void private_message(struct event *event){
    struct user *usr = event->client_persistent_data;
    uint32_t sender = event->packet->header.sender_id;
    uint32_t receiver = event->packet->header.receiver_id;

    struct private_chat *pc = pccol_find_chat_by_two_users(&pcs, sender, receiver);
    if(pc != NULL){
        //if such a chat exists

        if(!pc_is_pc_blocked(pc)){
            uint32_t mid = privmsg_in_existing_private_chat(event, pc);
            response_ACK_set_message_id(event, mid);
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


void broadcast_to_group_chat(struct server *server,struct packet *pack, struct group_chat *gc, uint32_t sender){
    struct id_collection *members = gc_get_list_of_listeners(gc);
    
    if(members != NULL){
        for(uint32_t i = 0; i < members->number_of_ids; i++){
            if(members->ids[i] != sender)
                send_message_to_user(server, pack, members->ids[i]);
        }
    }
}


uint32_t send_message_in_existing_group_chat(struct event *event, struct group_chat *gc){
    struct chat_entry *cent = build_chat_entry_from_packet(event->packet);
    gc_add_message(gc, cent);
    uint32_t message_id = event->packet->header.message_id = cent->message_id;

    broadcast_to_group_chat(event->server, event->packet, gc, event->packet->header.sender_id);

    free(cent);

    return message_id;
}


void group_message(struct event *event){
    struct group_chat *gc = gccol_find_chat_by_group_id(&gcs, event->packet->header.receiver_id);

    if(gc != NULL){
        uint32_t mid = send_message_in_existing_group_chat(event, gc);
        response_ACK_set_message_id(event, mid);
    } else{
        response_NACK(event);
    }
}


bool delete_message_in_existing_group_chat(struct event *event, struct group_chat *gc){
    uint32_t message_id = event->packet->header.target;

    gc_delete_message(gc, message_id);
    broadcast_to_group_chat(event->server, event->packet, gc, event->packet->header.sender_id);

    return true;
}


void delete_group_message(struct event *event){
    struct group_chat *gc = gccol_find_chat_by_group_id(&gcs, event->packet->header.receiver_id);

    if(gc != NULL){
        bool success = delete_message_in_existing_group_chat(event, gc);
        if(success)
            response_ACK(event);
        else
            response_NACK(event);
    } else{
        response_NACK(event);
    }
}


bool delete_message_in_existing_private_chat(struct event *event, struct private_chat *pc){
    uint32_t message_id = event->packet->header.target;

    pc_delete_message(pc, message_id);

    relay_message_in_private_chat(event->server, pc, event->packet, event->packet->header.sender_id);

    return true;
}


void delete_private_message(struct event *event){
    uint32_t sender = event->packet->header.sender_id;
    uint32_t receiver = event->packet->header.receiver_id;

    struct private_chat *pc = pccol_find_chat_by_two_users(&pcs, sender, receiver);
    if(pc != NULL){
        bool success = delete_message_in_existing_private_chat(event, pc);
        if(success)
            response_ACK(event);
        else
            response_NACK(event);
    } else{
        response_NACK(event);
    }
}



void edit_group_message(struct event *event);
void edit_private_message(struct event *event);

void create_group(struct event *event){
    struct group_chat gc;
    group_chat_init(&gc);

    gccol_add_gchat(&gcs, &gc);
    response_ACK_set_message_id(event, gc.chat_id);
}

void join_group(struct event *event){
    struct group_chat *gc = gccol_find_chat_by_group_id(&gcs, event->packet->header.target);

    if(gc != NULL){
        gc_add_listener(gc, event->packet->header.sender_id);
        response_ACK(event);
    }else{
        response_NACK(event);
    }
}

void leave_group(struct event *event){
    struct group_chat *gc = gccol_find_chat_by_group_id(&gcs, event->packet->header.target);

    if(gc != NULL){
        gc_remove_listener(gc, event->packet->header.sender_id);
        response_ACK(event);
    }else{
        response_NACK(event);
    }
}