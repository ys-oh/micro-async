#include "uasync/udp_socket.h"
#include <stdio.h>
#include <string.h>

char buffer[1500];

void on_recv(int err, struct udp_socket* socket, struct endpoint* ep, int bytes, void* obj)
{
    printf ("[on recv] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes, strerror(-err));
    printf ("==> %s\n", buffer);
}

void on_send(int err, struct udp_socket* socket, struct endpoint* ep, int bytes, void* obj)
{
    printf ("[on send] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes, strerror(-err));
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

    struct udp_socket* cli = udp_socket(NULL, endpoint_ipv4(addr, port));

    sprintf(buffer, "hello server!");

    udp_send(cli, buffer, strlen(buffer) + 1, on_send, NULL);
    udp_receive(cli, buffer, 1500, on_recv, NULL);

    udp_socket_release(cli);
    return 0;
}
