#include "user_collection.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAX(x, y) (x > y ? x : y)
#define NEXT_SIZE_OF_ARRAY_FOR(x) (2 * x)

// I have no idea why curly braces decided to open at the new line

void user_collection_init(struct user_collection *ucol){
    assert(ucol != NULL);

    ucol->number_of_users = 0;
    ucol->users = NULL;
    ucol->allocated_for = 0;
}

void user_collection_destroy(struct user_collection *ucol){
    assert(ucol != NULL);

    free(ucol->users);
    user_collection_init(ucol);
}

struct user *ucol_find_user_by_username(struct user_collection *ucol, char *username){
    assert(ucol != NULL);
    assert(username != NULL);

    for (uint32_t i = 0; i < ucol->number_of_users; i++){
        if (strcmp(ucol->users[i].username, username)){
            return ucol->users + i;
        }
    }

    return NULL;
}

struct user *ucol_find_user_by_id(struct user_collection *ucol, uint32_t id){
    assert(ucol != NULL);

    for (uint32_t i = 0; i < ucol->number_of_users; i++){
        if (ucol->users[i].id == id){
            return ucol->users + i;
        }
    }

    return NULL;
}

struct user *ucol_find_user_by_logged_in_from(struct user_collection *ucol, uint32_t logged_in_from){
    assert(ucol != NULL);

    for (uint32_t i = 0; i < ucol->number_of_users; i++){
        if (ucol->users[i].id == logged_in_from){
            return ucol->users + i;
        }
    }

    return NULL;
}

struct user *ucol_add_user(struct user_collection *ucol, struct user *user)
{
    assert(ucol != NULL);
    assert(user != NULL);

    // add more space if there isnt enough
    uint32_t required_space = ucol->number_of_users + 1u;
    if (required_space == 0){
        printf("Exceeded user storage capacity. This needs to be fixed. Please revisit the implementation of user_collection \n");
        exit(1);
    }

    if (required_space > ucol->allocated_for){
        ucol_allocate_for(ucol, required_space);
    }

    // add user to the collection
    ucol->users[ucol->number_of_users] = *user;
    ucol->number_of_users++;
    return ucol->users + (ucol->number_of_users - 1);
}

void ucol_fill_in_space(struct user_collection *ucol, uint32_t starting_from){

    for (uint32_t i = starting_from; i < ucol->number_of_users - 1; i++){
        ucol->users[i] = ucol->users[i + 1];
    }
    ucol->number_of_users--;
}

void ucol_remove_user(struct user_collection *ucol, struct user *user){
    assert(ucol != NULL);

    if (user != NULL){
        int64_t target = -1;

        for (uint32_t i = 0; i < ucol->number_of_users && target < 0; i++){
            if (ucol->users[i].id == user->id){
                target = i;
            }
        }

        if (target >= 0){
            user_destroy(ucol->users + target);
            ucol_fill_in_space(ucol, target);
        }
    }
}

void ucol_allocate_for(struct user_collection *ucol, uint32_t users){
    assert(ucol != NULL);
    //otherwise the memory might leak if incorrectly called
    assert(ucol->number_of_users <= users);

    // max is in case it overflows
    uint32_t new_size = NEXT_SIZE_OF_ARRAY_FOR(users);
    new_size = MAX(users, new_size);

    ucol->users = realloc(ucol->users, new_size * sizeof(struct user));
    ucol->allocated_for = new_size;
}
