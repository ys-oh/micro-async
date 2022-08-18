#include "uasync/udp_socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


char buffer[1500];

void on_send(int err, struct udp_socket* socket, struct endpoint* ep, int bytes_transferred, void* obj)
{
    printf ("[on send] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transferred, strerror(-err));
}

void on_recv(int err, struct udp_socket* socket, struct endpoint* ep, int bytes_transferred, void* obj)
{
    printf ("[on recv] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transferred, strerror(-err));
    printf ("==> %s\n", buffer);
}

int main(int argc, char **argv)
{
    struct udp_socket* socket = udp_bind(NULL, endpoint_ipv4("0.0.0.0", 12345));
    if (socket)
        printf ("udp_socket (%d), bind ok.\n", socket->socket);

    struct endpoint ep = endpoint_ipv4("0.0.0.0", 0);
    udp_recvfrom(socket, buffer, 1500, &ep, on_recv, NULL);

    sprintf(buffer, "hello client!");
    udp_sendto(socket, buffer, strlen(buffer) + 1, &ep, on_send, NULL);

    udp_socket_release(socket);
    return 0;
}

