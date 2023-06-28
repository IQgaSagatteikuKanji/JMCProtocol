#pragma once
#include <stdint.h>


struct socket;

struct address{
    uint8_t ip[4];
    uint16_t port;
};