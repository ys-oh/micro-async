#include "uasync/tcp_socket.h"
#include "uasync/event_loop.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char buffer[1500];

void on_accept(int err, struct tcp_acceptor* acceptor, struct tcp_socket* socket, void* obj);
void on_send(int err, struct tcp_socket* socket, int bytes_transferred, void* obj)
{
    printf ("[on send] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transferred, strerror(-err));
    tcp_socket_release(socket);
}

void on_recv(int err, struct tcp_socket* socket, int bytes_transferred, void* obj)
{
    printf ("[on recv] fd: %d, bytes: %d, err: %s\n", socket->socket, bytes_transferred, strerror(-err));
    if (err || bytes_transferred == 0)
    {
        tcp_socket_release(socket);
        return;
    }

    printf("==> %s\n", buffer);

    sprintf(buffer, "hello async client!");
    tcp_async_send(socket, buffer, strlen(buffer) + 1, on_send, obj);
}

void on_accept(int err, struct tcp_acceptor* acceptor, struct tcp_socket* socket, void* obj)
{
    printf ("[on accept] fd: %d, fd: %d, err(%d): %s\n", acceptor->socket, socket->socket, err, strerror(-err));
    if (err)
    {
        if (err == -EAGAIN || err == -EWOULDBLOCK)
        {
            tcp_async_accept(acceptor, socket, on_accept, obj);
        }
        else
            tcp_socket_release(socket);
        return;
    }

    tcp_async_receive(socket, buffer, 1500, on_recv, acceptor);
    tcp_async_accept(acceptor, tcp_socket_new(acceptor->ctx), on_accept, obj);
}


int main(int argc, char **argv)
{
    tcp_async_accept(
        tcp_bind(EVENT_LOOP_DEFAULT,
            endpoint_ipv4("0.0.0.0", 12345)),
        tcp_socket_new(EVENT_LOOP_DEFAULT),
        on_accept, NULL);

    event_loop_run(EVENT_LOOP_DEFAULT);

    return 0;
}
