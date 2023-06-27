#pragma once
#include <stdint.h>

// Contracts:
// - init functions allocate memory and transfer ownership

struct packet{
    //server uses it to indicate a response to a specific packet
    uint32_t id;
    //user identifiers on a server
    uint32_t sender_id;
    uint32_t receiver_id;

    // alignment will be the same, in both cases there will be a Byte empty between packets
    // so Ill pack it to be more logical
    
    // signals operation type
    uint8_t op_code;

    uint16_t payload_length;
    char *payload;
};

struct encoded_packet{
    uint16_t length;
    char *text;
};

void encode_packet(struct packet *packet, struct encoded_packet *encoded);
void decode_packet(struct encoded_packet *encoded, struct packet *packet);

// transfers ownership
void packet_init(struct packet *packet);
void packet_destroy(struct packet *packet);

// transfers ownership
void encoded_packet_init(struct encoded_packet *encoded);
void encoded_packet_destroy(struct encoded_packet *encoded);

