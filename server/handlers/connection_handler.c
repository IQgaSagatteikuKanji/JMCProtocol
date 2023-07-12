#include "connection_handler.h"

#include <stdlib.h>

#include "server_state.h"
#include "server_responses.h"
#include "user_collection.h"

void user_login(struct event *event){
    if(event->client_persistent_data == NULL){
        //if the user isn't signed in already
        struct user *usr = ucol_find_user_by_id(&users, event->packet->header.sender_id);

        if(usr == NULL){
            //if the user is a new user, register the user and accept the login
            struct user user;
            user_init(&user);
            user.id = event->packet->header.sender_id;
            user.logged_in_from = event->generated_by;
            user.is_logged_in = true;
            event->client_persistent_data = ucol_add_user(&users, &user);

            response_ACK(event);
        } 
        //solve synchronisation here later, like maybe change is_logged_in to mutex and do try_lock?
        else if(!usr->is_logged_in && !usr->is_banned){
            //if user exists, but is not logged in or banned, accept it
            usr->is_logged_in = true;
            usr->logged_in_from = event->generated_by;
            event->client_persistent_data = usr;

            response_ACK(event);
        } else{
            //if the user exists and has already logged in or has been banned, deny the login
            response_NACK(event);
        }
    } else{
        //if the user is signed in already, deny the login
        response_NACK(event);
    }
   
}

bool is_introduction_correct(struct event *event){
    return ((struct user *)event->client_persistent_data)->id == event->packet->header.sender_id;
}


bool is_user_banned(struct event *event){
    return ((struct user *)event->client_persistent_data)->is_banned;
}