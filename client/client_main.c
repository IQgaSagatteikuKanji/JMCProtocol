#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "socket_proxy.h"
#include "internet_address.h"
#include "packet_format.h"
#include "packet_net_transmission.h"
#include "uconstants.h"
#include "logger.h"
#include "client_interface.h"

struct address_v4 server;
struct socket_xpa sock;
struct trctrl ctrl;
struct logger logger;
uint32_t my_id = 3;
uint32_t message_id = 0;

const char *sender_logfile = "sender_logfile.txt";
const char *recver_logfile = "recver_logfile.txt";

void handler(int signo){
    printf("Exiting\n");
    socket_destroy(&sock);
    exit(0);
}

const char *menu_text = "Placeholder: ";

void command_actions_delegate(enum COMMAND_CODES code, char *command);
void login(uint32_t name);
void privmsg(uint32_t name, char *command);

int main(int argc, char *argv[]){
    signal(SIGINT, handler);

    socket_init(&sock);
    socket_open(&sock);

    server.ip = "0.0.0.0";
    server.port = 5001;
    if(socket_connect(&sock, &server) < 0){
        printf("Fatal failure: Failed to connect to a server.\n");
        handler(SIGINT);
    }
    trctrl_init(&ctrl, &sock);

    char command[COMMAND_MAX_LENGTH + 1];
	bool infinite_loop = true;
	enum COMMAND_CODES ccode = CC_NOOP;

	while(infinite_loop){
		printf("%s", menu_text);
		memset(command, 0, COMMAND_MAX_LENGTH);
		fgets(command, COMMAND_MAX_LENGTH, stdin);

		ccode = get_command_code_from_str(command);

        command_actions_delegate(ccode, command);
		if(ccode == CC_EXIT){
			infinite_loop = false;
		}
	}   
    
}

void command_actions_delegate(enum COMMAND_CODES code, char *command){
    switch(code){
        case CC_LOGIN:
            login(my_id);
            break;
        case CC_PRIVMSG:
            break;
    }
}

void login(uint32_t name){
    struct packet pack;
    packet_init(&pack);
    pack.header.id = message_id++;
    pack.header.op_code = LOGIN;
    pack.header.sender_id = name;

    trctrl_send(&ctrl, &pack);

    packet_destroy(&pack);

    trctrl_receive(&ctrl, &pack);
    if(pack.header.op_code == ACK){
        printf("Successfully logged in\n");
    } else{
        printf("Failed to log in\n");
    }
}

void privmsg(uint32_t name, char *command){
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