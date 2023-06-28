#include <assert.h>

#include "packet_format.h"
#include "uconstants.h"
#include "stdlib.h"
#include "XDR_representation.h"



void encode_header(struct packet_header *header, struct encoded_packet *encoded, uint8_t offset){
    assert(header != NULL);
    
    uint32_t c_offset = offset;
    c_offset += translate_uint32_to_XDR(&header->id, encoded->text + c_offset);
    c_offset += translate_uint32_to_XDR(&header->sender_id, encoded->text + c_offset);
    c_offset += translate_uint32_to_XDR(&header->receiver_id, encoded->text + c_offset);
    c_offset += translate_uint16_to_XDR(&header->op_code, encoded->text + c_offset);
    c_offset += translate_uint16_to_XDR(&header->payload_length, encoded->text + c_offset);
}

void encode_payload(struct packet *packet, struct encoded_packet *encoded, uint8_t offset){

    translate_fixed_length_opaque_to_XDR(packet->payload, packet->header.payload_length, encoded->text + offset);
}


void encode_packet(struct packet *packet, struct encoded_packet *encoded){
    assert(packet != NULL);
    assert(encoded != NULL);
    assert(encoded->length == 0);
    assert(packet->header.payload_length <= PAYLOAD_MAX_LENGTH);

    encoded->length = HEADER_SIZE + packet->header.payload_length;
    encoded->text = calloc(1, encoded->length);

    encode_header(&packet->header, encoded, 0);
    encode_payload(packet, encoded, HEADER_SIZE);
}


uint8_t decode_packet(struct encoded_packet *encoded, struct packet *packet, uint8_t offset){
    assert(packet != NULL);
    
    uint8_t bytes = decode_header(encoded, &packet->header, offset);
    bytes += decode_payload(encoded, packet, offset + bytes);
    return bytes;
}

// Expects a whole header to be present in the encoded_packet
uint8_t decode_header(struct encoded_packet *encoded, struct packet_header *header, uint8_t offset){
    assert(encoded != NULL);
    assert(header != NULL);
    assert(header->payload_length == 0);
    assert(encoded->length > HEADER_SIZE + offset);
    
    uint32_t c_offset = offset;
    c_offset += translate_XDR_to_uint32(encoded->text + c_offset, &header->id);
    c_offset += translate_XDR_to_uint32(encoded->text + c_offset, &header->sender_id);
    c_offset += translate_XDR_to_uint32(encoded->text + c_offset, &header->receiver_id);
    c_offset += translate_XDR_to_uint16(encoded->text + c_offset, &header->op_code);
    c_offset += translate_XDR_to_uint16(encoded->text + c_offset, &header->payload_length);

    return c_offset - offset;
}

// Expects a whole payload to be present in the encoded_packet
uint8_t decode_payload(struct encoded_packet *encoded, struct packet *packet, uint8_t offset){
    assert(packet->payload == NULL);

    packet->payload = calloc(1, packet->header.payload_length);
    translate_XDR_to_fixed_length_opaque(encoded->text + offset, packet->header.payload_length, packet->payload);
}


void header_init(struct packet_header *header){
    header->id = 0;
    header->op_code = OPCODE_NULL;
    header->sender_id = 0;
    header->receiver_id = 0;
    header->payload_length = 0;
}

void packet_init(struct packet *packet){
    header_init(&packet->header);
    packet->payload = NULL;
}
void packet_destroy(struct packet *packet){
    free(packet->payload);
    packet_init(packet);
}

// transfers ownership
void encoded_packet_init(struct encoded_packet *encoded){
    encoded->length = 0;
    encoded->text = NULL;
}
void encoded_packet_destroy(struct encoded_packet *encoded){
    free(encoded->text);
    encoded_packet_init(encoded);
}

void encoded_packet_allocate_for(struct encoded_packet *encoded, uint8_t size){    
    assert(encoded != NULL);
    assert(encoded->length == 0);

    encoded->length = size;
    encoded->text = calloc(1, encoded->length);
}