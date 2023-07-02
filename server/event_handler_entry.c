#include "event_handler_entry.h"
#include <stdint.h>

#include "uconstants.h"
#include "server_parameters.h"
#include "packet_format.h"
#include "logger.h"
#include "server_control.h"
// this thing is temporary just to demonstrate a working scenario

struct chatter{
    uint32_t chatter_id;
};

// no synchronisation yet
struct chatter chatters[MAX_SERVED_CLIENTS_NUMBER];

struct logger logger;

void handle_greeting(struct event *event){
    printf("Greeting came from: %d\n", event->generated_by);
    chatters[event->generated_by].chatter_id = event->packet->header.sender_id;
}

void handle_start(struct event *event){
    struct logger_builder builder;
    builder.filename = "Server_logfile.txt";
    logger_init(&logger, &builder);
}

void handle_privmsg(struct event *event){
    printf("Got private message\n");
    for(int i = 0; i < MAX_SERVED_CLIENTS_NUMBER; i++){
        if(chatters[i].chatter_id == event->packet->header.receiver_id){
            printf("Found receiver of message: %d\n", i);
            log_fixed_length_str(&logger, "Message rerouted\n",sizeof("Message rerouted\n"));
            server_send_message(event->server, i, event->packet);
        }
    }
}

void handle_shutting_down(struct event *event){
    logger_destroy(&logger);
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
        if(event->packet->header.op_code == GREETING)
            handle_greeting(event);
        if(event->packet->header.op_code == PRIVMSG)
            handle_privmsg(event);
    }
};