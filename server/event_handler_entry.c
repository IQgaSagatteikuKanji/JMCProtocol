#include "event_handler_entry.h"

#include <stdint.h>
#include <stdlib.h>

#include "uconstants.h"
#include "server_parameters.h"
#include "packet_format.h"
#include "logger.h"
#include "server_control.h"
#include "server_state.h"
#include "server_responses.h"

#include "connection_handler.h"

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

    switch(event->packet->header.op_code){
        case LOGIN:
            user_login(event);
            break;
        default:
            response_NACK(event);
    }
}

void disconnect_handler(struct event *event){
    struct user *usr = event->client_persistent_data;

    if(usr != NULL){
        usr->is_logged_in = false;
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