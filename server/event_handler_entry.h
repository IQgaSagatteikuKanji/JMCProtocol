#pragma once
#include <stdbool.h>
#include "events.h"

// If suppress_sending is set 
//  handle events as usual except for sending responses
//  sending any kind of messages in this mode will result in a hard error 
void event_handler_main(struct event *event, bool suppress_sending);