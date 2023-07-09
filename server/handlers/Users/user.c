#include "user.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>


void user_init(struct user *usr){
    assert(usr != NULL);

    usr->logged_in_from = 0;
    usr->id = 0;

    memset(usr->username, 0, USERNAME_MAX_LENGTH);
    usr->string_end_delimeter = 0;

    usr->is_admin = false;
    usr->is_banned = false;
    usr->is_logged_in = false;
}

void user_destroy(struct user *user){}

bool user_is_admin(struct user *user){
    assert(user != NULL);
    
    return user->is_admin;
}

bool user_is_logged_in(struct user *user){
    assert(user != NULL);

    return user->is_logged_in;
}
