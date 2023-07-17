#include "event_handler_entry.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "uconstants.h"
#include "server_parameters.h"
#include "packet_format.h"
#include "logger.h"
#include "server_control.h"
#include "server_state.h"
#include "server_responses.h"

#include "connection_handler.h"
#include "chatting_handler.h"

#define MIN(x,y) (x < y ? x : y)

// The client_persistent_data will be a reference to a user profile client has signed in

void handle_start(struct event *event){
    struct logger_builder builder;
    builder.filename = "Server_logfile.txt";
    logger_init(&logger, &builder);

    user_collection_init(&users);

    pc_collection_init(&pcs);
    gc_collection_init(&gcs);
}

void handle_shutting_down(struct event *event){
    logger_destroy(&logger);
    user_collection_destroy(&users);

    pc_collection_destroy(&pcs);
    gc_collection_destroy(&gcs);
}

void packets_handler(struct event *event){
    //some checks for all the packets
    //add logic that requires loging in to start

    char *type_of_packet = "Undefined";

    switch(event->packet->header.op_code){
        case LOGIN:
            user_login(event);
            type_of_packet = "LOGIN";
            break;
            
        case PRIVMSG:
            private_message(event);
            type_of_packet = "PRIVMSG";
            break;

        case GROUPMSG:
            group_message(event);
            type_of_packet = "GROUPMSG";
            break;

        case CREATE_GROUP:
            create_group(event);
            type_of_packet = "CREATE_GROUP";
            break;
        
        case JOIN:
            join_group(event);
            type_of_packet = "JOIN";
            break; 

        case LEAVE:
            leave_group(event);
            type_of_packet = "LEAVE";
            break;
        
        case DELETEMSG_FROM_GROUP:
            delete_group_message(event);
            type_of_packet = "DELETEMSG_FROM_GROUP";
            break;

        default:
            response_NACK(event);
    }

    int str_length = 512;
    char *str = calloc(1, str_length);
    memset(str, 0, str_length);
    sprintf(str, "Client %u sent %s to %u with payload:", event->packet->header.sender_id, type_of_packet, event->packet->header.receiver_id);
    strncat(str, event->packet->payload, MIN(str_length - strlen(str), event->packet->header.payload_length));
    str[strlen(str)] = '\n';

    log_0_terminated_str(&logger, str);
}

void disconnect_handler(struct event *event){
    struct user *usr = event->client_persistent_data;

    if(usr != NULL){
        usr->is_logged_in = false;

        int str_length = 512;
        char *str = calloc(1, str_length);
        memset(str, 0, str_length);
        sprintf(str, "Client %u has disconnected\n", usr->id);
        log_0_terminated_str(&logger, str);
    } else{
        log_0_terminated_str(&logger, "Unknown user has disconnected\n");
    }
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