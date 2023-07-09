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

const char *sender_logfile = "sender_logfile.txt";
const char *recver_logfile = "recver_logfile.txt";

void handler(int signo){
    printf("Exiting\n");
    socket_destroy(&sock);
    exit(0);
}

void sender_routine(){
    struct packet packet;
    packet_init(&packet);
    packet.header.op_code = PRIVMSG;
    packet.header.receiver_id = 1;
    trctrl_send(&ctrl, &packet);
    char resp[] = "Message sent\n";
    log_fixed_length_str(&logger, resp, sizeof(resp));
}

void recver_routine(){
    struct packet packet;
    packet_init(&packet);

    packet.header.op_code = GREETING;
    packet.header.sender_id = 1;
    perror("Sending greetings\n");
    int error = trctrl_send(&ctrl, &packet);

    perror("Receiving data\n");
    int res = trctrl_receive(&ctrl, &packet);
    if(res >= 0){
        char resp[] = "Got message successfully\n"; 
        perror(resp);
        log_fixed_length_str(&logger, resp, sizeof(resp));
    } else{
        char resp[] = "Connection was obliterated\n";
        perror(resp); 
        log_fixed_length_str(&logger, resp, sizeof(resp));
    }
}

void test_routines(char *argv[]){
    struct logger_builder builder;
    if(!strcmp(argv[1], "sender")){
        builder.filename = sender_logfile;
        logger_init(&logger, &builder);
        sender_routine();
    } else{
        builder.filename = recver_logfile;
        logger_init(&logger, &builder);
        perror("logfile initialised\n");
        recver_routine();
    }
    logger_destroy(&logger);
}

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

    if(argc == 2){
        test_routines(argv);
    }
    
    
}

