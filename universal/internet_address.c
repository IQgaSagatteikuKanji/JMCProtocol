#include "internet_address.h"

#include <stddef.h>
#include <assert.h>
#include <arpa/inet.h>

#define BITS_IN_BYTE 8

uint32_t extract_ip_address(struct address_v4 *address){
    assert(address != NULL);

    return inet_addr(address->ip);
}

uint16_t extract_port_number(struct address_v4 *address){
    assert(address != NULL);

    return htons(address->port);
}