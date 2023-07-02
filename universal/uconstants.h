#pragma once

// Packet-related constants
#define PAYLOAD_MAX_LENGTH 512

// Command codes
enum MESSAGE_TYPES{
    NULL_MSG = 0,
    GREETING,
    PRIVMSG
};