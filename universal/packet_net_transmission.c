#include <assert.h>
#include <stdlib.h>

#include "packet_net_transmission.h"


void trctrl_init(struct trctrl *ctrl, struct socket_xpa *sock){
    assert(ctrl != NULL);
    assert(sock != NULL);

    ctrl->sock = sock;
}

void trctrl_destroy(struct trctrl *ctrl){
    ctrl->sock = NULL;
}

void trctrl_send(struct trctrl *ctrl, struct packet *pack){
    assert(pack != NULL);
    assert(ctrl != NULL);
    assert(ctrl->sock != NULL);
    
    struct encoded_packet enc;
    encoded_packet_init(&enc);

    // Archiver_archive added here
    encode_packet(pack, &enc);

    uint32_t bytes_sent = 0;
    uint32_t step = 0;
    while(bytes_sent < enc.length){
        step = socket_send(ctrl->sock, enc.text + bytes_sent, enc.length - bytes_sent);
        if(step < 0){
            // signal about error in send
            assert(step > 0);
        }
        bytes_sent += step;
    }

    encoded_packet_destroy(&enc);
}

void trctrl_receive_header(struct trctrl *ctrl, struct packet *pack){
    struct encoded_packet header;
    encoded_packet_init(&header);
    
    uint8_t expected_length = HEADER_SIZE;
    encoded_packet_allocate_for(&header, expected_length);

    uint32_t bytes_received = 0;
    uint32_t step = 0;
    while( bytes_received < expected_length){
        step = socket_receive(ctrl->sock, header.text + bytes_received, header.length - bytes_received);
        if(step < 0){
            // signal about error in receive header
            assert(step > 0);
        }
        bytes_received += step;
    }

    decode_header(&header, &pack->header, 0);
    encoded_packet_destroy(&header);
}

void trctrl_receive(struct trctrl *ctrl, struct packet *pack){
    assert(pack != NULL);
    assert(ctrl != NULL);
    assert(ctrl->sock != NULL);
    
    trctrl_receive_header(ctrl, pack);

    struct encoded_packet payload;
    encoded_packet_init(&payload);
    encoded_packet_allocate_for(&payload, pack->header.payload_length);

    uint32_t bytes_received = 0;
    uint32_t step = 0;
    while( bytes_received < pack->header.payload_length){
        step = socket_receive(ctrl->sock, payload.text + bytes_received, payload.length - bytes_received);
        if(step < 0){
            // signal about error in receive header
            assert(step > 0);
        }
        bytes_received += step;
    }

    decode_payload(&payload, pack, 0);
    encoded_packet_destroy(&payload);
}