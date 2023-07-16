#include "client_messages_renderer.h"

#include <assert.h>
#include <stdlib.h>

#define DEFAULT_MESSAGE_RENDER_HEIGHT 5
#define DEFAULT_MESSAGE_HEADER_DELIMETER " @ "

void messages_init(struct messages *msgs, uint32_t height, uint32_t width){
    assert(msgs != NULL);

    msgs->height = height;
    msgs->width = width;
    msgs->message_height = DEFAULT_MESSAGE_RENDER_HEIGHT;
    msgs->offset = 0;
    msgs->delimeter = DEFAULT_MESSAGE_HEADER_DELIMETER;

    msgs->field = newwin(msgs->height, msgs->width, 0, 0);
    client_dlist_init(&msgs->entries, msgs->height / msgs->message_height);
}

void messages_destroy(struct messages *msgs){
    assert(msgs != NULL);

    client_dlist_destroy(&msgs->entries);
}


int draw_chat_entry(struct messages *msgs, struct client_chat_entry *cent){
    // draws the bottom line and doesn't care about the top line
    uint32_t offset = msgs->offset;
    if(offset + msgs->message_height > msgs->height){
        return 0;
    }

    wmove(msgs->field, offset, 0);
    if(!cent->type == PRIVATE_MESSAGE){
        wprintw(msgs->field, "%u%s%u POSTED %u\n%s\n", cent->chatter_id, msgs->delimeter, cent->chat_id, cent->message_id, cent->load);
    } else{
        wprintw(msgs->field, "%u%sPRIVATE POSTED %u\n%s\n",cent->chatter_id, msgs->delimeter, cent->message_id, cent->load);
    }    
    
    whline(msgs->field, ACS_HLINE, msgs->width);
    msgs->offset = getcury(msgs->field) + 1;

    return 1;
}


void messages_render(struct messages *msgs){
    assert(msgs != NULL);

    wclear(msgs->field);
    msgs->offset = 0;
    struct client_dlist_node *cent = msgs->entries.head;

    for(int i = 0; cent != NULL && i < msgs->entries.render_depth; i++){
        draw_chat_entry(msgs, &cent->msg);
        cent = cent->next;
    }

    wrefresh(msgs->field);
}

void messages_add_message(struct messages *msgs, struct packet *pack){
    assert(msgs != NULL);
    assert(pack != NULL);

    struct client_chat_entry cent;
    client_chat_entry_init(&cent);
    ccent_build_from_packet(&cent, pack);
    client_dlist_add_new_chat_entry(&msgs->entries, &cent);
}
void messages_remove_message(struct messages *msgs, uint32_t message_id){
    assert(msgs != NULL);

    client_dlist_remove_chat_entry(&msgs->entries, message_id);
}