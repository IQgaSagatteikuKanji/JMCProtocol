#pragma once

#include "events.h"

void private_message(struct event *event);
void group_message(struct event *event);

void delete_group_message(struct event *event);
void delete_private_message(struct event *event);

void edit_group_message(struct event *event);
void edit_private_message(struct event *event);

void create_group(struct event *event);
void join_group(struct event *event);
void leave_group(struct event *event);

bool is_user_banned_in_group_chat(struct event *event, struct group_chat *gc);
bool is_user_banned_in_private_chat(struct event *event, struct private_chat *pc);