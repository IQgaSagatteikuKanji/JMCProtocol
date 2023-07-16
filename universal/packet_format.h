#pragma once
#include <stdint.h>

// Contracts:
// - init functions allocate memory and transfer ownership

struct packet_header{
    //server uses it to indicate a response to a specific packet
    uint32_t id;

    //user identifiers on a server
    uint32_t sender_id;
    uint32_t receiver_id;
    uint32_t target;
    uint32_t message_id;

    // alignment will be the same, in both cases there will be a Byte empty between packets
    // so Ill pack it to be more logical
    
    // signals operation type
    uint16_t op_code;

    uint16_t payload_length;
};

struct packet{
    struct packet_header header;
    char *payload;
};

struct encoded_packet{
    uint32_t length;
    char *text;
};

#define PAYLOAD_MAX_LENGTH 512
#define HEADER_SIZE sizeof(struct packet_header)

void encode_packet(struct packet *packet, struct encoded_packet *encoded);

// Expects a whole packet to be present in the encoded_packet
uint8_t decode_packet(struct encoded_packet *encoded, struct packet *packet, uint8_t offset);

// Expects a whole header to be present in the encoded_packet
uint8_t decode_header(struct encoded_packet *encoded, struct packet_header *header, uint8_t offset);

// Expects a whole payload to be present in the encoded_packet
uint8_t decode_payload(struct encoded_packet *encoded, struct packet *packet, uint8_t offset);

// transfers ownership
void packet_init(struct packet *packet);
void packet_destroy(struct packet *packet);

// transfers ownership
void encoded_packet_init(struct encoded_packet *encoded);
void encoded_packet_destroy(struct encoded_packet *encoded);
void encoded_packet_allocate_for(struct encoded_packet *encoded, uint8_t size);

