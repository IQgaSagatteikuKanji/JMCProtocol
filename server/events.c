#include "events.h"

#include <assert.h>
#include <stdlib.h>

void event_init(struct event *event){
    assert(event != NULL);

    event->client_persistent_data = NULL;
    event->packet = NULL;
    event->server = NULL;
    event->type = EVENT_NULL;
}
void event_destroy(struct event *event){}
