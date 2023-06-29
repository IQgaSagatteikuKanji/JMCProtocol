#include "internet_address.h"

#include <stddef.h>

#define BITS_IN_BYTE 8

uint32_t extract_ip_address(struct address_v4 *address){
    assert(address != NULL);

    //regardless of architecture the bytes here are given in Big Endian
    uint32_t ip = 0;
    for(int i = IPV4_LENGTH; i > 0; i--){
        ip = ip << BITS_IN_BYTE;
        ip += address->ip[i];
    }

    return ip;
}

uint16_t extract_port_number(struct address_v4 *address){
    assert(address != NULL);

    return htons(address->port);
}