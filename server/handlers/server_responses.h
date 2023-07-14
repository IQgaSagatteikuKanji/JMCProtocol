#pragma once

#include "events.h"
#include "uconstants.h"

void response_general_no_payload(struct event *event, enum MESSAGE_TYPES type);

void response_ACK(struct event *event);
void response_NACK(struct event *event);


//this will be sent to the sender of the PRIVMSG and GROUPMSG, to notify him about the message_id of his message
void response_ACK_set_message_id(struct event *event, uint32_t message_id);