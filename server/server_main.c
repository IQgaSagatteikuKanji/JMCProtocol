#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "server_control.h"

void shutdown(int);

char server_started = 0;
struct server_context server;

int main(){
    if(signal(SIGINT, shutdown) == SIG_ERR){
        printf("Failed to set up SIGINT handler. Shutting down.\n");
        exit(1);
    }

    server_init(&server);
    server_started = 1;

    server_start(&server);
}


void shutdown(int signum){
    printf("Shutting down server. Please wait...\n");
    
    if(server_started != 0){
        server_shutdown(&server);
        printf("Server was shut down. Freeing up resources...\n");
        server_destroy(&server);
    }
    printf("Finalized. Exiting\n");


    exit(0);
}