#include "client_command_handler.h"

#include <signal.h>
#include <string.h>

#include "client_interface.h"
#include "client_context.h"
#include "client_globals.h"
#include "uconstants.h"
#include "threads_proxy.h"


void login_handler();
void private_message_handler(char *command);

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