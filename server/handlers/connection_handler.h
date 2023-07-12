#pragma once

#include <stdbool.h>

#include "events.h"

//another user-client specific functions will be added here, if needed

// ACCEPTS:
// - new user
// - existing user that isnt logged in
// DENIES otherwise
void user_login(struct event *event);

// checks if user introduced itself correctly in the packet
// it is an error to check it for a user that was not signed in yet
bool is_introduction_correct(struct event *event);
bool is_user_banned(struct event *event);