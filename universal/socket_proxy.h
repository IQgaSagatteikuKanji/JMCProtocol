#pragma once
#include <stdint.h>
#include <stdbool.h>

// NOT TESTED YET

//just an abstraction over sockets to easily change platforms later if needed

struct socket_xpa{
    int descr;
    bool is_open;
};

void socket_init(struct socket_xpa *);
void socket_destroy(struct socket_xpa *);

void socket_open(struct socket_xpa *sock);
int socket_connect(struct socket_xpa *, struct address_v4 *addr);
int socket_bind(struct socket_xpa *, struct address_v4 *addr);
int socket_listen(struct socket_xpa *, uint32_t active_queue_length);
void socket_accept(struct socket_xpa *server, struct socket_xpa *returns_client);

int socket_send(struct socket_xpa *, char *str, int length);
int socket_receive(struct socket_xpa *, char *str, int length);

int socket_shutdown(struct socket_xpa *);
int socket_close(struct socket_xpa *);