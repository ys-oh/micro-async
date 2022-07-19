#include "uasync/tcp_socket.h"

#include <stdio.h>
#include <string.h>

char buffer[1500];

void on_recv(int err, struct tcp_socket* socket, int bytes, void* obj)
{
    printf ("[on recv] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes, strerror(-err));
    printf ("==> %s\n", buffer);
}

void on_send(int err, struct tcp_socket* socket, int bytes, void* obj)
{
    printf ("[on send] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes, strerror(-err));
}

void on_connect(int err, struct tcp_socket* socket, struct endpoint* ep, void* obj)
{
    printf ("[on connect] fd: %d, err: %s\n", socket->socket, strerror(-err));
}

int main(int argc, char **argv)
{
    char* addr = "127.0.0.1";
    int port = 12345;
    if (argc == 3)
    {
        addr = argv[1];
        port = atoi(argv[2]);
    }

    struct tcp_socket* cli = tcp_socket_new(NULL);

    if (tcp_connect(cli, endpoint_ipv4(addr, port), on_connect, NULL) < 0)
        exit(-1);

    sprintf(buffer, "hello server!");

    tcp_send(cli, buffer, strlen(buffer) + 1, on_send, NULL);
    tcp_receive(cli, buffer, 1500, on_recv, NULL);

    tcp_socket_release(cli);
    return 0;
}
