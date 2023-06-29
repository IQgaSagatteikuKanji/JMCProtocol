#pragma once

#include <stdint.h>

#include "packet_format.h"


// Event types
enum EVENT_TYPES{
    EVENT_NULL = 0,
    PACKET,
    SERVER_IS_SHUTTING_DOWN,
    CLIENT_SOCKET_HAS_DISCONNECTED
};

struct event{
    enum EVENT_TYPES type;
    struct packet *packet;
    uint8_t generated_by;
};

void event_init();
void event_destroy();

void event_get_type();
void event_get_packet();

