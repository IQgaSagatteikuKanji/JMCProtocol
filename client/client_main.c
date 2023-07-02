#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "socket_proxy.h"
#include "internet_address.h"

struct address_v4 server;
struct socket_xpa sock;

void handler(int signo){
    printf("Exiting\n");
    socket_destroy(&sock);
    exit(0);
}

int main(){
    signal(SIGINT, handler);

    socket_init(&sock);
    socket_open(&sock);

    server.ip = "0.0.0.0";
    server.port = 5000;
    socket_connect(&sock, &server);

    while(1){

    }
}