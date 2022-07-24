#include "uasync/tcp_socket.h"
#include "uasync/event_loop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[1500];

void on_recv(int err, struct tcp_socket* socket, int bytes_transfered, void* obj)
{
    printf ("[on recv] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transfered, strerror(-err));
    if (err || bytes_transfered == 0)
    {
        tcp_socket_release(socket);
        return;
    }

    printf("==> %s\n", buffer);
}

void on_send(int err, struct tcp_socket* socket, int bytes_transfered, void* obj)
{
    printf ("[on send] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transfered, strerror(-err));
    if (err)
        return;

    tcp_async_receive(socket, buffer, 1500, on_recv, obj);
}

void on_connect(int err, struct tcp_socket* socket, struct endpoint* ep, void* obj)
{
    printf ("[on connect] fd: %d, err: %s\n", socket->socket, strerror(-err));

    if (err)
    {
        tcp_async_connect(socket, *ep, on_connect, obj);
    }
    else
    {
        sprintf(buffer, "hello async server");
        tcp_async_send(socket, buffer, strlen(buffer) + 1, on_send, obj);
    }
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


    tcp_async_connect(tcp_socket_new(EVENT_LOOP_DEFAULT),
                      endpoint_ipv4(addr, port),
                      on_connect, NULL);

    event_loop_run(EVENT_LOOP_DEFAULT);

    return 0;
}

