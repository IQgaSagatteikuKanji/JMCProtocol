#include "chat_entry.h"

#include <assert.h>
#include <stdlib.h>

void chat_entry_init(struct chat_entry *entry){
    assert(entry != NULL);

    entry->chatter_id = 0;
    entry->message_id = 0;
    entry->type = ERROR_ENTRY;
    entry->load_length = 0;
    entry->load = NULL;
}


void chat_entry_destroy(struct chat_entry *entry){
    assert(entry != NULL);


    free(entry->load);
    entry->load_length = 0u;
}
