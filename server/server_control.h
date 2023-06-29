#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>

#include "event_handler_entry.h"
#include "internet_address.h"
#include "socket_proxy.h"
#include "packet_net_transmission.h"
#include "packet_format.h"
#include "server_parameters.h"
#include "events.h"

//the only mutable shared resource threads will have is clients_number
//others will be considered either immutable to threads
//or unshared between threads at least considering the lifespan of a child thread
//so no further synchronization is needed

struct server_context{
    //immutable
    void (*event_handler_main)(struct event *, bool);
    //immutable
    struct address_v4 address;
    //immutable
    struct socket_xpa host_sock;
    //mutable shared
    atomic_char clients_number;
    //child threads only read
    bool working;
    //each individual socket is unshared for threads
    struct trctrl clients[MAX_SERVED_CLIENTS_NUMBER];
    
    //while making it mutable and non atomic definetely creates race conditions
    //they can be ignored because dispatcher changes value before creating a thread that can edit it
    //and then only checks the value
    //the thread will only change it to true when client is closing connection
    bool waiting_for_clean_up[MAX_SERVED_CLIENTS_NUMBER];
};

void server_init(struct server_context *server);
void server_destroy(struct server_context *server);

void server_start(struct server_context *server);
void server_shutdown(struct server_context *server);

// Performs server checks for sending/receiving packets
void server_send_message(struct server_context *server, uint8_t client_id, struct packet *msg);

//not recommended for use
void server_receive_message(struct server_context *server, uint8_t client_id, struct packet *msg);

void server_close_client(struct server_context *server, uint8_t client_id);