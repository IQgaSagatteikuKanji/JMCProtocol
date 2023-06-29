#pragma once
#include <stdint.h>


#define IPV4_LENGTH 4

struct address_v4{
    uint8_t ip[IPV4_LENGTH];
    uint16_t port;
};

uint32_t extract_ip_address(struct address_v4 *address);
uint16_t extract_port_number(struct address_v4 *address);
