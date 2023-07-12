#include "server_responses.h"



void response_general_no_payload(struct event *event, enum MESSAGE_TYPES type){
    struct packet pack;
    packet_init(&pack);
    pack.header.id = event->packet->header.id;
    pack.header.op_code = type;
    pack.header.receiver_id = event->packet->header.sender_id;

    server_send_message(event->server, event->generated_by, &pack);
    packet_destroy(&pack);
}

void response_ACK(struct event *event){
    response_general_no_payload(event, ACK);
}

void response_NACK(struct event *event){
    response_general_no_payload(event, NACK);
}