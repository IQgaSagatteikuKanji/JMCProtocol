#include "socket_proxy.h"

#include <assert.h>
#include <stdlib.h>

// Ill write implementation for Linux only but it should be trivial to add others
// SOCK_STREAM

#include<sys/socket.h>
#include<arpa/inet.h>	
#include <unistd.h>


uint16_t extract_port_address(struct address_v4 *address){
    assert(address != NULL);

    return htons(address->port);
}

void socket_init(struct socket_xpa *sock){
    sock->descr = -1;
    sock->is_open = false;
}

void socket_open(struct socket_xpa *sock){
    sock->descr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock->descr > 0){
        sock->is_open = true;
    }
}

void socket_destroy(struct socket_xpa *sock){
    if(sock->is_open){
        socket_close(sock);
    }
    sock->is_open = false;
    sock->descr = -1;
}

int socket_connect(struct socket_xpa *sock, struct address_v4 *addr){
    assert(sock->is_open);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;

    server_address.sin_addr.s_addr = (in_addr_t) extract_ip_address(addr);
    server_address.sin_port = extract_port_number(addr);

    return connect(sock->descr, (struct sockaddr *) &server_address, sizeof(server_address)); 
}

int socket_bind(struct socket_xpa *sock, struct address_v4 *addr){
    assert(sock->is_open);

    struct sockaddr_in address;
    address.sin_family = AF_INET;

    address.sin_addr.s_addr = extract_ip_address(addr);
    address.sin_port = extract_port_number(addr);

    return bind(sock->descr,(struct sockaddr *) &address, sizeof(address)); 
}

int socket_listen(struct socket_xpa *sock, uint32_t active_queue_length){
    assert(sock->is_open);
    return listen(sock->descr, active_queue_length);
}


void socket_accept(struct socket_xpa *server, struct socket_xpa *client){
    assert(server->is_open);
    assert(client != NULL);
    assert(client->descr == -1);

    client->descr = accept(server->descr, NULL, NULL);
    client->is_open = true;
}

int socket_shutdown(struct socket_xpa *sock){
    assert(sock->is_open);

    return shutdown(sock->descr, SHUT_RDWR);
}

int socket_close(struct socket_xpa *sock){
    assert(sock->is_open);

    sock->is_open = false;
    return close(sock->descr);
}


int socket_send(struct socket_xpa *sock, char *str, int length){
    assert(sock->is_open);

    return send(sock->descr, str, length, 0);
}

int socket_receive(struct socket_xpa *sock, char *str, int length){
    assert(sock != NULL);
    assert(sock->is_open);

    return recv(sock->descr, str, length, 0);
}