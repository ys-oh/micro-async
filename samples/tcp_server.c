#include "uasync/tcp_socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


char buffer[1500];

void on_send(int err, struct tcp_socket* socket, int bytes_transferred, void* obj)
{
    printf ("[on send] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transferred, strerror(-err));
}

void on_recv(int err, struct tcp_socket* socket, int bytes_transferred, void* obj)
{
    printf ("[on recv] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transferred, strerror(-err));
}

void on_accept(int err, struct tcp_acceptor* acceptor, struct tcp_socket* socket, void* obj)
{
    printf ("[on accept] fd: %d, fd: %d, err: %s\n", acceptor->socket, socket->socket, strerror(-err));
}

int main(int argc, char **argv)
{
    struct endpoint ep = endpoint_ipv4("0.0.0.", 12345);
    struct tcp_acceptor* acceptor = tcp_bind(NULL, ep);
    struct tcp_socket* socket = tcp_socket_new(NULL);

    if (tcp_accept(acceptor, socket, on_accept, NULL) < 0)
        exit(-1);

    tcp_receive(socket, buffer, 1500, on_recv, NULL);
    printf ("=> %s\n", buffer);

    sprintf(buffer, "hello client!");
    tcp_send(socket, buffer, strlen(buffer) + 1, on_send, NULL);

    tcp_socket_release(socket);
    tcp_acceptor_release(acceptor);
    return 0;
}

