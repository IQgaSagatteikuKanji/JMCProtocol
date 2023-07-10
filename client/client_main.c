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

void command_actions_delegate(enum COMMAND_CODES code);
void login(uint32_t name);

int main(int argc, char *argv[]){
    signal(SIGINT, handler);

    socket_init(&sock);
    socket_open(&sock);

    server.ip = "0.0.0.0";
    server.port = 5000;
    if(socket_connect(&sock, &server) < 0){
        printf("Fatal failure: Failed to connect to a server.\n");
        handler(SIGINT);
    }
    trctrl_init(&ctrl, &sock);

    char command[COMMAND_MAX_LENGTH + 1];
	bool infinite_loop = true;
	enum COMMAND_CODES ccode = NOOP;

	while(infinite_loop){
		printf("%s", menu_text);
		memset(command, 0, COMMAND_MAX_LENGTH);
		fgets(command, COMMAND_MAX_LENGTH, stdin);

		ccode = get_command_code_from_str(command);

        command_actions_delegate(ccode);
		if(ccode == EXIT){
			infinite_loop = false;
		}
	}   
    
}

void command_actions_delegate(enum COMMAND_CODES code){
    switch(code){
        case LOGIN:
            login(my_id);
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