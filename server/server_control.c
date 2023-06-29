#include "server_control.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h>

#include "socket_proxy.h"
#include "packet_net_transmission.h"

void server_init(struct server_context *server){
    assert(server != NULL);

    uint8_t ip_address[] = IP_ADDRESS;
    for(int i = 0; i < IPV4_LENGTH; i++){
        server->address.ip[i] = ip_address[i];
    }
    server->address.port = PORT_NUMBER;

    server->event_handler_main = EVENT_HANDLER;

    server->clients_number = -1; //hasn't started yet

    socket_init(&server->host_sock);
    socket_open(&server->host_sock);
}

void server_destroy(struct server_context *server){
    assert(server != NULL);

    if(server->working){
        server_shutdown(server);
    }

    socket_destroy(&server->host_sock);
}


void thread_routine(struct server_context *server, uint8_t client){
    int socket_error = false;

    struct trctrl *serving = &server->clients[client];
    struct event event;
    event_init(&event);
    event.generated_by = client;

    struct packet packet;
    packet_init(&packet);

    while(server->working){
        
        socket_error = trctrl_receive(serving,&packet);

        if(socket_error < 0){
            event.type = CLIENT_SOCKET_HAS_DISCONNECTED;
            event.packet = NULL;
        } else{
            event.type = PACKET;
            event.packet = &packet;
        }

        server->event_handler_main(&event, false);
        packet_destroy(&packet);

        if(socket_error < 0){
            event_destroy(&event);
            return;
        }
    }
}

void server_start(struct server_context *server){
    assert(server != NULL);

    //binds server
    int error_code = socket_bind(&server->host_sock, &server->address);
    if(error_code < 0){
        perror("FATAL ERROR: Failed to bind to the given address\n");
        server_destroy(server);
        exit(error_code);
    }
    //starts listening
    error_code = socket_listen(&server->host_sock, ACTIVE_QUEUE_SIZE);
    if(error_code < 0){
        perror("FATAL ERROR: Failed to start listening to the socket\n");
        server_destroy(server);
        exit(error_code);
    }


    server->clients_number = 0;
    server->working = true;
    //dispatches one thread for each client
}


void server_shutdown(struct server_context *server){
    assert(server != NULL);

    //sends event that server shuts down
    struct event event;
    event_init(&event);
    event.type = SERVER_IS_SHUTTING_DOWN;
    server->event_handler_main(&event, false);
    event_destroy(&event);

    
    //stops all threads 
    server->working = false;

    //close all clients
    for(int i = 0; i < server->clients_number; i++){
        socket_destroy(server->clients[i].sock);
        trctrl_destroy(&server->clients[i]);
    }

    server->clients_number = 0;
}