#pragma once

#include <stdbool.h>

#include "events.h"
#include "group_chat.h"

bool is_client_host_admin(struct event *event);
bool is_client_mod_in_group(struct event *event, struct group_chat *gc);


void permaban_user_handler(struct event *event);
void permaban_group_handler(struct event *event);
void ban_user_from_group_handler(struct event *event);