#include "client_chat_entry.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "uconstants.h"

void client_chat_entry_init(struct client_chat_entry *cent){
    assert(cent != NULL);

    cent->chat_id = 0;
    cent->chatter_id = 0;
    cent->type = NULL_CHAT_ENTRY;
    cent->load_length = 0;
    cent->load = NULL;
    cent->message_id = 0;
}

void client_chat_entry_destroy(struct client_chat_entry *cent){
    assert(cent != NULL);
    
    free(cent->load);
    client_chat_entry_init(cent);
}

void ccent_build_from_packet(struct client_chat_entry *cent, struct packet *pack){
    assert(cent != NULL);
    assert(pack != NULL);

    client_chat_entry_destroy(cent);

    cent->chat_id = pack->header.receiver_id;
    cent->chatter_id = pack->header.sender_id;
    cent->load_length = pack->header.payload_length;
    if(cent->load_length != 0){
        cent->load = calloc(1, cent->load_length);
        strncpy(cent->load, pack->payload, cent->load_length);
    }
    cent->message_id = pack->header.message_id;

    if(pack->header.op_code == GROUPMSG){
        cent->type = GROUP_MESSAGE;
    } else if( pack->header.op_code == PRIVMSG){
        cent->type = PRIVATE_MESSAGE;
    }
}