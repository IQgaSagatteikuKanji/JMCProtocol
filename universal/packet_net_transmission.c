#include "packet_net_transmission.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "XDR_representation.h"
#include "archivation_proxy.h"

void trctrl_init(struct trctrl *ctrl, struct socket_xpa *sock){
    assert(ctrl != NULL);
    assert(sock != NULL);

    ctrl->sock = sock;
}

void trctrl_destroy(struct trctrl *ctrl){
    free(ctrl->sock);
    ctrl->sock = NULL;
}

int receive_exactly_n_bytes(struct trctrl *ctrl, struct encoded_packet *encmes, uint32_t length){
    assert(ctrl != NULL);
    assert(ctrl->sock != NULL);
    assert(encmes != NULL);
    assert(length > 0);

    encoded_packet_init(encmes);
    assert(ctrl != NULL);
    encoded_packet_allocate_for(encmes, length);
    assert(ctrl != NULL);

    uint32_t bytes_received = 0u;
    uint32_t step = 0u;

    while(bytes_received < length){
        step = socket_receive(ctrl->sock, encmes->text + bytes_received, length - bytes_received);
        if(step <= 0){
            encoded_packet_destroy(encmes);
            return -1;
        }
        bytes_received += step;
    }

    return 0;
}

int trctrl_send(struct trctrl *ctrl, struct packet *pack){
    assert(pack != NULL);
    assert(ctrl != NULL);
    assert(ctrl->sock != NULL);
    
    struct encoded_packet unarchived_enc;
    encoded_packet_init(&unarchived_enc);

    // Archiver_archive added here
    encode_packet(pack, &unarchived_enc);
    assert(unarchived_enc.length <= MAX_LENGTH_OF_NET_TRANSMITTED_PACKET - LENGTH_DEFINING_HEADER_LENGTH);


    struct encoded_packet enc;
    archive_string(unarchived_enc.text, unarchived_enc.length, &enc.text, &enc.length);
    encoded_packet_destroy(&unarchived_enc);
    
    assert(enc.length <= MAX_SIZE_ARCHIVED_PACKET);
    //converting to net transmittable

    struct encoded_packet nettr;
    encoded_packet_init(&nettr);
    encoded_packet_allocate_for(&nettr, LENGTH_DEFINING_HEADER_LENGTH + enc.length);
    nettr.length = translate_variable_length_opaque_to_XDR(enc.text, enc.length, nettr.text);
    encoded_packet_destroy(&enc);

    uint32_t bytes_sent = 0;
    uint32_t step = 0;
    while(bytes_sent < nettr.length){
        step = socket_send(ctrl->sock, nettr.text + bytes_sent, nettr.length - bytes_sent);
        if(step < 0){
            // signal about error in send
            encoded_packet_destroy(&nettr);
            return -1;
        }
        bytes_sent += step;
    }

    encoded_packet_destroy(&nettr);
    return 0;
}

int trctrl_receive(struct trctrl *ctrl, struct packet *pack){
    assert(pack != NULL);
    assert(ctrl != NULL);
    assert(ctrl->sock != NULL);

    uint32_t expected_length = sizeof(uint32_t);
    struct encoded_packet encmes;
    int error_code = receive_exactly_n_bytes(ctrl, &encmes, expected_length);
    if(error_code < 0){
        return error_code;
    }

    translate_XDR_to_uint32(encmes.text, &expected_length);
    encoded_packet_destroy(&encmes);
    
    if(error_code < 0){
        return error_code;
    }
    //check if the size of packet is smaller than defined by archiver
    if(expected_length > MAX_SIZE_ARCHIVED_PACKET || expected_length == 0){
        return -1;
    }
    
    //receive archive
    error_code = receive_exactly_n_bytes(ctrl, &encmes, expected_length);
    if(error_code < 0){
        return error_code;
    }
    //dearchive packet
    struct encoded_packet unarchived_enc;
    dearchive_string(encmes.text, encmes.length, &unarchived_enc.text, &unarchived_enc.length);
    encoded_packet_destroy(&encmes);

    //decode packet
    decode_packet(&unarchived_enc, pack, 0);
    encoded_packet_destroy(&unarchived_enc);

    return 0;
}