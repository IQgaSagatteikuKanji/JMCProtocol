#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ncurses.h>

#include "internet_address.h"
#include "socket_proxy.h"
#include "packet_net_transmission.h"
#include "logger.h"
#include "client_context.h"
#include "threads_proxy.h"
#include "uconstants.h"
#include "client_globals.h"

void handler(int signo){
    socket_destroy(&sock);
    endwin();
    exit(0);
}

//it should be in a different file though 
void client_packet_handler();

void *user_input_thread_routine(void *v){
    //doesn't touch messages drawing, so it's safe for concurrency
    ncscr_dedicate_thread_to_managing_user_input(&info);
}

void arguments_parser(int argc, char *argv[]);

int main(int argc, char *argv[]){
    my_id = 3;
    message_id = 0;
    mutex_init(&message_queue);

    signal(SIGINT, handler);
    socket_init(&sock);
    trctrl_init(&ctrl, &sock);

    server.ip = "0.0.0.0";
    server.port = 5001;
    arguments_parser(argc, argv);

    socket_open(&sock);
    socket_connect(&sock, &server);

    initscr();
    noecho(); // I will have to make a custom one
    curs_set(0);
    
    ncscr_info_init(&info);

    ncscr_refresh_chat(&info);

    struct thread user_service;
    thread_create(&user_service, NULL, user_input_thread_routine, NULL);

    client_packet_handler();
}

extern char *optarg;
void arguments_parser(int argc, char *argv[]){
    int opt = 0;

    while ((opt = getopt(argc, argv, "i:p:u:")) != -1) {
        switch (opt) {
            case 'i':
               server.ip = calloc(1, strlen(optarg));
               strcpy(server.ip, optarg);
               break;
            case 'p':
                server.port = atoi(optarg);
                break;
            case 'u':
                my_id = atoi(optarg);
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-i IPv4_address] [-p port] [-u user_id]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // parameters should be cleaned up, but alas
}


void client_packet_handler(){
    struct packet pack;
    packet_init(&pack);

    //it's too late to change things
    while(serving){
        int error = trctrl_receive(&ctrl, &pack);

        if(error < 0){
            //exiting normally
            raise(SIGINT);
        }

        if(pack.header.op_code == PRIVMSG || pack.header.op_code == GROUPMSG){
            mutex_lock(&message_queue);
            ncscr_add_message(&info, &pack);
            ncscr_refresh_chat(&info);
            mutex_unlock(&message_queue);
        }

        if(pack.header.op_code == DELETEMSG_FROM_GROUP){
            mutex_lock(&message_queue);
            ncscr_remove_message(&info, pack.header.target, pack.header.receiver_id, GROUP_MESSAGE);
            ncscr_refresh_chat(&info);
            mutex_unlock(&message_queue);
        }

        packet_destroy(&pack);
    }
}