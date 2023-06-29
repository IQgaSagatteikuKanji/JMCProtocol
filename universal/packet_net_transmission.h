#pragma once
#include "socket_proxy.h"
#include "packet_format.h"

// NOT TESTED YET

// Defines interface for sending (receiving) whole packets to (from) the network
// Created in order to abstract away from the way the packets are sent
// Also is managing that packets are sent/received fully
// With blocking sockets just continues to resend/receive until the operation is fully done 

// Relies that user opens and closes sockets with outside tools and only cares that packets are correctly
// exchanged

// Doesn't possess ownership over socket_xpa
// Possesses ownership over archiver

// transmission control
struct trctrl{
    struct socket_xpa *sock;
};

// Add archiver to the init and structure
void trctrl_init(struct trctrl *ctrl, struct socket_xpa *sock);
void trctrl_destroy(struct trctrl *ctrl);

int trctrl_send(struct trctrl *ctrl, struct packet *pack);
int trctrl_receive(struct trctrl *ctrl, struct packet *pack);