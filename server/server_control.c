#include "server_control.h"

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "socket_proxy.h"
#include "packet_net_transmission.h"

void server_init(struct server_context *server){
    assert(server != NULL);

    for(int i = 0; i < MAX_SERVED_CLIENTS_NUMBER; i++){
        server->thread_available[i] = true;
        server->waiting_for_clean_up[i] = false;
    }

    server->address.ip = IP_ADDRESS;
    server->address.port = PORT_NUMBER;

    server->event_handler_main = EVENT_HANDLER;

    server->clients_number = -1; //hasn't started yet
    server->working = false;

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

struct thread_routine_parameters{
    struct server_context *server;
    uint16_t client;
};


void thread_routine(struct thread_routine_parameters *params){
    //unpacking params for easier use
    struct server_context *server = params->server;
    uint8_t client = params->client;

    int socket_error = false;
    struct trctrl *serving = &server->clients[client];
    struct event event;
    event_init(&event);
    event.generated_by = client;
    event.server = server;

    struct packet packet;
    packet_init(&packet);

    while(server->working){
        
        socket_error = trctrl_receive(serving, &packet);

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
            printf("Client disconnected. Finishing up thread...\n");
            event_destroy(&event);
            server->waiting_for_clean_up[client] = true;
            server->clients_number--;
            thread_exit(NULL);
        }
    }
}

bool server_can_serve_next_client(struct server_context *server){
    return (server->clients_number < MAX_SERVED_CLIENTS_NUMBER) && (server->working);
}

void server_clean_up_thread_if_finished(struct server_context *server, uint16_t thread){
    if(server->waiting_for_clean_up[thread]){
        //wait for the thread to finish execution then free the thread resources
        thread_join(server->threads + thread, NULL);
        socket_destroy(server->clients[thread].sock);
        trctrl_destroy(&server->clients[thread]);

        server->waiting_for_clean_up[thread] = false;
        server->thread_available[thread] = true;
    };
}

//expects that there is a free space
//will effectively block execution if there isn't
uint16_t server_find_space_for_thread(struct server_context *server, uint16_t prev){
    //try the next possible in a ring as naive strategy
    uint16_t next = (prev + 1) % MAX_SERVED_CLIENTS_NUMBER;

    server_clean_up_thread_if_finished(server, next);

    if(server->thread_available[next]){
        return next;
    }
    else{
        //if it fails search for any available spot from 0 
        for(uint16_t i = 0; i < MAX_SERVED_CLIENTS_NUMBER; i++){
            server_clean_up_thread_if_finished(server, i);
            if(server->thread_available[i]) return i;
        }
    }
}

uint16_t dispatch_resources_for_client(struct server_context *server, uint16_t available_space, 
                                        struct socket_xpa *client){
    available_space = server_find_space_for_thread(server, available_space);

    server->thread_available[available_space] = false;
    trctrl_init(&server->clients[available_space], client);
    
    struct thread_routine_parameters *params = calloc(1, sizeof(struct thread_routine_parameters));
    
    params->client = available_space;
    params->server = server;

    printf("Socket placed at: %d\n", available_space);

    thread_create(server->threads + available_space, NULL, (void *(*)(void *)) thread_routine, params);
    
    return available_space;
}

void server_start(struct server_context *server){
    assert(server != NULL);

    printf("Trying to bind server to address: ");
    //binds server
    int error_code = socket_bind(&server->host_sock, &server->address);
    if(error_code < 0){
        printf("\nFATAL ERROR: Failed to bind to the given address\n");
        server_destroy(server);
        exit(error_code);
    }
    printf("SUCCESS\n");

    printf("Trying to start listening: ");
    //start listening
    error_code = socket_listen(&server->host_sock, ACTIVE_QUEUE_SIZE);
    if(error_code < 0){
        perror("\nFATAL ERROR: Failed to start listening to the socket\n");
        server_destroy(server);
        exit(error_code);
    }
    printf("SUCCESS\n");

    server->clients_number = 0;
    server->working = true;

    //send event that server is starting
    struct event event;
    event_init(&event);
    event.type = SERVER_STARTING;
    server->event_handler_main(&event, false);
    event_destroy(&event);

    //listen for clients and dispatch one thread for each client
    uint16_t available_space = 0;
    struct socket_xpa *incoming_client = calloc(1, sizeof(struct socket_xpa));

    printf("Server ready to accept clients\n");
    while(server->working){
        socket_init(incoming_client);
        socket_accept(&server->host_sock, incoming_client);

        if(server_can_serve_next_client(server)){
            printf("Incoming client: ");
            printf("Accepted\n");

            server->clients_number++;
            available_space = dispatch_resources_for_client(server, available_space, incoming_client);
            incoming_client = calloc(1, sizeof(struct socket_xpa));
        } 
        else{
            //if client capacity reached deny connection
            socket_destroy(incoming_client);
            printf("Incoming client: ");
            printf("\nClient tried to connect, but the capacity is reached. Denied\n");
        }
    }
}


void server_shutdown(struct server_context *server){
    assert(server != NULL);
    if(server->working){
        //sends event that server shuts down
        struct event event;
        event_init(&event);
        event.type = SERVER_IS_SHUTTING_DOWN;
        event.server = server;
        server->event_handler_main(&event, false);
        event_destroy(&event);

    
        //stop all threads and close all clients
        server->working = false;

        for(int i = 0; i < server->clients_number; i++){
            if(!server->thread_available[i]){
                //once the socket is shut down, function to read a socket returns negative number for an error
                //we wait for a thread to shut down its functions and then destroy structures
                socket_shutdown(server->clients[i].sock);
                thread_join(server->threads + i, NULL);
                
                socket_destroy(server->clients[i].sock);
                trctrl_destroy(&server->clients[i]);
            }
        }

        server->clients_number = 0;
    }

    
}

// Performs server checks for sending/receiving packets
enum SERVOP_STATUS server_send_message(struct server_context *server, uint8_t client_id, struct packet *msg){
    assert(server != NULL);
    assert(client_id < MAX_SERVED_CLIENTS_NUMBER);

    if(!server->thread_available[client_id] && !server->waiting_for_clean_up[client_id] && msg != NULL){
        int code = trctrl_send(server->clients + client_id, msg);
        if(code < 0)
            return CONNECTION_TO_CLIENT_UNAVAILABLE;
    } else{
        if(msg != NULL)
            return NOT_EXISTING_CLIENT;
        else return MESSAGE_CANT_BE_NULL;
    }
    return SUCCESS;
}

//not recommended for use
enum SERVOP_STATUS server_receive_message(struct server_context *server, uint8_t client_id, struct packet *msg){
    assert(server != NULL);
    assert(client_id < MAX_SERVED_CLIENTS_NUMBER);

     if(!server->thread_available[client_id] && !server->waiting_for_clean_up[client_id] && msg != NULL){
        int code = trctrl_receive(server->clients + client_id, msg);
        
        return CONNECTION_TO_CLIENT_UNAVAILABLE;

    } else{
        if(msg != NULL)
            return NOT_EXISTING_CLIENT;
        else return MESSAGE_CANT_BE_NULL;
    }
    return SUCCESS;
}

void server_close_client(struct server_context *server, uint8_t client_id){
    assert(server != NULL);

    if(client_id < MAX_SERVED_CLIENTS_NUMBER && !server->thread_available[client_id]){
        socket_shutdown(server->clients[client_id].sock);
        thread_join(server->threads + client_id, NULL);
                
        socket_destroy(server->clients[client_id].sock);
        trctrl_destroy(&server->clients[client_id]);
        server->waiting_for_clean_up[client_id] = false;
        server->thread_available[client_id] = true;
        server->clients_number--;
    }
}