#pragma once

#include <stdint.h>
#include <stdbool.h>

#define USERNAME_MAX_LENGTH 16
struct user{
    in32_t logged_in_from;
    uint32_t id;
    char username[USERNAME_MAX_LENGTH];
    char string_end_delimeter;
    bool is_admin;
    bool is_banned;
    bool is_logged_in;
};


void user_init(struct user *user);
void user_destroy(struct user *user);

bool user_is_admin(struct user *user);
bool user_is_logged_in(struct user *user);