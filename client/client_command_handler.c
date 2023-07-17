#include "client_command_handler.h"

#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "client_interface.h"
#include "client_context.h"
#include "client_globals.h"
#include "uconstants.h"
#include "threads_proxy.h"


void login_handler();
void create_group_handler(char *command);
void join_group_handler(char *command);
void private_message_handler(char *command);
void group_message_delete_handler(char *command);
void group_message_handler(char *command);
void leave_group_handler(char *command);

void command_handler(char *command){
    enum COMMAND_CODES ccode = get_command_code_from_str(command);

    switch(ccode){
        case CC_EXIT:
            // At least its somewhat better, than what was here before
            ncscr_stop_serving_clients(&info);
            raise(SIGINT);
            break;
        
        case CC_LOGIN:
            login_handler();
            break;

        case CC_PRIVMSG:
            private_message_handler(command);
            break;
        
        case CC_GROUPMSG:
            group_message_handler(command);
            break;

        case CC_CREATE_GROUP:
            create_group_handler(command);
            break;
        
        case CC_JOIN_GROUP_CHAT:
            join_group_handler(command);
            break;

        case CC_LEAVE:
            leave_group_handler(command);
            break;
        
        case CC_DELETE_GROUPMSG:
            group_message_delete_handler(command);
            break;

        default:
        //should put some output to signalise its a wrong command later
            break;
    }
}

void login_handler(){
    struct packet pack;
    packet_init(&pack);
    pack.header.id = message_id++;
    pack.header.op_code = LOGIN;
    pack.header.sender_id = my_id;

    trctrl_send(&ctrl, &pack);

    packet_destroy(&pack);

    trctrl_receive(&ctrl, &pack);
    //its a bad practic, but I'm dire for time
    if(pack.header.op_code == ACK){
        printw("Successfully logged in\n");
    } else{
        printw("Failed to log in\n");
    }
}

void private_message_handler(char *command){
    uint32_t recipient;
    char *payload = NULL;
    parse_msg(command, &recipient, &payload);

    struct packet pack;
    packet_init(&pack);
    pack.header.id = message_id++;
    pack.header.op_code = PRIVMSG;
    pack.header.receiver_id = recipient;
    pack.header.sender_id = my_id;
    pack.header.payload_length = strlen(payload);
    pack.payload = payload;

    trctrl_send(&ctrl, &pack);
    

    //also frees payload string
    packet_destroy(&pack);
}

void group_message_handler(char *command){
    uint32_t recipient;
    char *payload = NULL;
    parse_msg(command, &recipient, &payload);

    struct packet pack;
    packet_init(&pack);
    pack.header.id = message_id++;
    pack.header.op_code = GROUPMSG;
    pack.header.receiver_id = recipient;
    pack.header.sender_id = my_id;
    pack.header.payload_length = strlen(payload);
    pack.payload = payload;

    trctrl_send(&ctrl, &pack);
    

    //also frees payload string
    packet_destroy(&pack);
}

void send_payload_free_package(uint32_t op_code, uint32_t target, uint32_t recipient){
    struct packet pack;
    packet_init(&pack);
    pack.header.id = message_id++;
    pack.header.op_code = op_code;
    pack.header.sender_id = my_id;
    pack.header.target = target;
    pack.header.receiver_id = recipient;

    trctrl_send(&ctrl, &pack);    

    //also frees payload string
    packet_destroy(&pack);
}

void create_group_handler(char *command){
    send_payload_free_package(CREATE_GROUP, 0, 0);
}
void join_group_handler(char *command){
    uint32_t recipient;
    parse_msg(command, &recipient, NULL);

    send_payload_free_package(JOIN, recipient, 0);
}

void leave_group_handler(char *command){
    uint32_t recipient;
    parse_msg(command, &recipient, NULL);

    send_payload_free_package(LEAVE, recipient, 0);
}

void group_message_delete_handler(char *command){
    char *target = NULL;
    uint32_t recipient;
    parse_msg(command, &recipient, &target);

    uint32_t target_id = atoi(target);
    free(target);

    send_payload_free_package(DELETEMSG_FROM_GROUP, target_id, recipient);
}