#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "server_control.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void shutdown(int);
void arguments_parser(int argc, char *argv[]);

char server_started = 0;
struct server_context server;

int main(int argc, char *argv[]){
    if(signal(SIGINT, shutdown) == SIG_ERR){
        printf("Failed to set up SIGINT handler. Shutting down.\n");
        exit(EXIT_FAILURE);
    }

    printf("Initialising server...\n");
    server_init(&server);
    server_started = 1;

    arguments_parser(argc, argv);
    printf("Hosting at %s:%d\nMax served clients number: %d\n", server.address.ip, server.address.port, MAX_SERVED_CLIENTS_NUMBER);

    printf("Starting server...\n");
    server_start(&server);
}


void shutdown(int signum){
    printf("\nShutting down server. Please wait...\n");
    
    if(server_started != 0){
        server_shutdown(&server);
        printf("Server was shut down. Freeing up resources...\n");
        server_destroy(&server);
    }
    printf("Finalized. Exiting\n");


    exit(EXIT_SUCCESS);
}

extern char *optarg;
void arguments_parser(int argc, char *argv[]){
    int opt = 0;

    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
        switch (opt) {
            case 'i':
               server.address.ip = calloc(1, strlen(optarg));
               strcpy(server.address.ip, optarg);
               break;
            case 'p':
                server.address.port = atoi(optarg);
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-i IPv4_address] [-p port]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // parameters should be cleaned up, but alas    
}
