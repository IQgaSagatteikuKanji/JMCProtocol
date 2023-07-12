#pragma once

#include "events.h"
#include "uconstants.h"

void response_general_no_payload(struct event *event, enum MESSAGE_TYPES type);

void response_ACK(struct event *event);
void response_NACK(struct event *event);