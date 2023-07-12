#include "event_handler_entry.h"

#include <stdint.h>
#include <stdlib.h>

#include "uconstants.h"
#include "server_parameters.h"
#include "packet_format.h"
#include "logger.h"
#include "server_control.h"
#include "handlers/server_state.h"
#include "handlers/Users/user_collection.h"
#include "handlers/Chats/private_chat.h"

// The client_persistent_data will be a reference to a user profile client has signed in

void handle_start(struct event *event){
    struct logger_builder builder;
    builder.filename = "Server_logfile.txt";
    logger_init(&logger, &builder);

    user_collection_init(&users);
    private_chat_init(&pc);
}

void handle_shutting_down(struct event *event){
    logger_destroy(&logger);
    user_collection_destroy(&users);
    private_chat_destroy(&pc);
}

void ACK_response(struct server_context *sercon, uint32_t client, uint32_t id){
    struct packet pack;
    packet_init(&pack);
    pack.header.id = id;
    pack.header.op_code = ACK;

    server_send_message(sercon, client, &pack);
    packet_destroy(&pack);
}

void general_NACK_response(struct server_context *sercon, uint32_t client, uint32_t id){
    struct packet pack;
    packet_init(&pack);
    pack.header.id = id;
    pack.header.op_code = NACK;

    server_send_message(sercon, client, &pack);
    packet_destroy(&pack);
}

void NACK_response(struct server_context *sercon, uint32_t client, uint16_t response_code, uint32_t id){
    struct packet pack;
    packet_init(&pack);
    pack.header.id = id;
    pack.header.op_code = response_code;

    server_send_message(sercon, client, &pack);
    packet_destroy(&pack);
}


// this handlers will be moved to a different header file
// They are easy to write, so wouldnt take much time
void user_login(struct event *event){
    struct user *usr = ucol_find_user_by_id(&users, event->packet->header.sender_id);
    if(usr == NULL){
        struct user user;
        user_init(&user);
        user.id = event->packet->header.sender_id;
        user.logged_in_from = event->generated_by;
        user.is_logged_in = true;
        ucol_add_user(&users, &user);

        ACK_response(event->server, event->generated_by, event->packet->header.id);
    } 
    else if(!usr->is_logged_in){
        usr->is_logged_in = true;
        usr->logged_in_from = event->generated_by;

        ACK_response(event->server, event->generated_by, event->packet->header.id);
    } else{
        general_NACK_response(event->server, event->generated_by, event->packet->header.id);
    }
}

void privmsg(struct event *event){
    struct user *usr = ucol_find_user_by_id(&users, event->packet->header.receiver_id);
    if(usr != NULL && usr->is_logged_in){
        server_send_message(event->server, usr->logged_in_from, event->packet);
        ACK_response(event->server, event->generated_by, event->packet->header.id);
    }
    else{
        NACK_response(event->server, event->generated_by, NACK, event->packet->header.id);
    }
}

void packets_handler(struct event *event){
    switch(event->packet->header.op_code){
        case LOGIN:
            user_login(event);
            break;
        case PRIVMSG:
            privmsg(event);
            break;
    }
}

void disconnect_handler(struct event *event){
    ucol_find_user_by_logged_in_from(&users,event->generated_by)->is_logged_in = false;
}


//right now it just relays messages to destination
void event_handler_main(struct event *event, bool suppress_sending){

    switch (event->type)
    {
    case SERVER_STARTING:
        handle_start(event);
        break;
    
    case SERVER_IS_SHUTTING_DOWN:
        handle_shutting_down(event);
        break;
    
    case PACKET:
        packets_handler(event);
        break;

    case CLIENT_SOCKET_HAS_DISCONNECTED:
        disconnect_handler(event);
        break;
    }
};