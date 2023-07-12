#pragma once

#include <stdint.h>

#include "packet_format.h"
#include "server_control.h"


// Event types
enum EVENT_TYPES{
    EVENT_NULL = 0,
    PACKET,
    SERVER_IS_SHUTTING_DOWN,
    SERVER_STARTING,
    CLIENT_SOCKET_HAS_DISCONNECTED
};

struct event{
    enum EVENT_TYPES type;
    struct packet *packet;
    uint16_t generated_by;
    struct server_context *server;

    //When event is generated by the same client, it keeps the previous value it had at the end of the previous call
    void *client_persistent_data;
};

void event_init(struct event *event);
void event_destroy(struct event *event);


