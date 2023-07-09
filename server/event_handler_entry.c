#include "event_handler_entry.h"
#include <stdint.h>

#include "uconstants.h"
#include "server_parameters.h"
#include "packet_format.h"
#include "logger.h"
#include "server_control.h"
#include "handlers/server_state.h"
#include "handlers/Users/user_collection.h"
#include "handlers/Chats/private_chat.h"



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


// this handlers will be moved to a different header file
//right now no persistance will be implemented, but it will be added
void user_login(struct event *event){
    struct user *usr = ucol_find_user_by_logged_in_from(&users, event->generated_by);
}

void packets_handler(struct event *event){
    switch(event->packet->header.op_code){
        case LOGIN:
            
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