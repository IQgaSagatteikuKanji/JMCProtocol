#pragma once

#include <stdint.h>

#include "logger.h"
#include "packet_net_transmission.h"
#include "socket_proxy.h"
#include "internet_address.h"
#include "client_context.h"

//there is a better way than this
struct ncscr_info info;
struct address_v4 server;
struct socket_xpa sock;
struct trctrl ctrl;
struct logger logger;
uint32_t my_id;
uint32_t message_id;
struct mutex message_queue;