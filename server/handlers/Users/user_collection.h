#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "user.h"


// It should be changed to something more performant
// It needs mutex
struct user_collection{
    struct user *users;
    uint32_t number_of_users;
    uint32_t allocated_for;
};


void user_collection_init(struct user_collection *ucol);
void user_collection_destroy(struct user_collection *ucol);

struct user *ucol_find_user_by_username(struct user_collection *ucol, char *username);
struct user *ucol_find_user_by_id(struct user_collection *ucol, uint32_t id);
struct user *ucol_find_user_by_logged_in_from(struct user_collection *ucol, uint32_t logged_in_from);

//returns the pointer to the newly added user
struct user *ucol_add_user(struct user_collection *ucol, struct user *user);
void ucol_remove_user(struct user_collection *ucol, struct user *user);

void ucol_allocate_for(struct user_collection *ucol, uint32_t users);
