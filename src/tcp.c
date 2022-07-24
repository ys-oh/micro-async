#include "uasync/tcp_socket.h"
#include "uasync/event_loop.h"

#include <stdlib.h>

CALLABLE_DEFINE(tcp_accept,
    CALLABLE_ARG(struct tcp_acceptor*, acceptor),
    CALLABLE_ARG(struct tcp_socket*, socket),
    CALLABLE_ARG(tcp_accept_callback, callback),
    CALLABLE_ARG(void*, obj))
{
    tcp_accept(acceptor, socket, callback, obj);
}

void tcp_async_accept(struct tcp_acceptor* acceptor, struct tcp_socket* socket,
                      tcp_accept_callback callback, void* obj)
{
    CALLABLE_OBJ(tcp_accept)* callable = (CALLABLE_OBJ(tcp_accept)*)
                                        mem_alloc(sizeof(CALLABLE_OBJ(tcp_accept)));

    CALLABLE_INIT(tcp_accept, callable, acceptor, socket, callback, obj);

    event_loop_add_io_event(acceptor->ctx, acceptor->socket, EVENT_READ, (callable_obj_base*)callable);
}

CALLABLE_DEFINE(tcp_connect,
    CALLABLE_ARG(struct tcp_socket*, socket),
    CALLABLE_ARG(struct endpoint*, ep),
    CALLABLE_ARG(tcp_connect_callback, callback),
    CALLABLE_ARG(void*, obj))
{
    tcp_connect(socket, *ep, callback, obj);
}

void tcp_async_connect(struct tcp_socket* s, struct endpoint ep,
                       tcp_connect_callback callback, void* obj)
{
    s->ep = ep;
    CALLABLE_OBJ(tcp_connect)* callable = (CALLABLE_OBJ(tcp_connect)*)
                                          malloc(sizeof(CALLABLE_OBJ(tcp_connect)));

    CALLABLE_INIT(tcp_connect, callable, s, &s->ep, callback, obj);

    event_loop_add_timed_event(s->ctx, 1000000, (callable_obj_base*)callable);
}

CALLABLE_DEFINE(tcp_receive,
    CALLABLE_ARG(struct tcp_socket*, socket),
    CALLABLE_ARG(void*, buffer),
    CALLABLE_ARG(int, size),
    CALLABLE_ARG(tcp_recv_callback, callback),
    CALLABLE_ARG(void*, obj))
{
    tcp_receive(socket, buffer, size, callback, obj);
}

void tcp_async_receive(struct tcp_socket* s, void* buffer, int size,
                       tcp_recv_callback callback, void* obj)
{
    CALLABLE_OBJ(tcp_receive)* callable = (CALLABLE_OBJ(tcp_receive)*)
                                          malloc(sizeof(CALLABLE_OBJ(tcp_receive)));

    CALLABLE_INIT(tcp_receive, callable, s, buffer, size, callback, obj);

    event_loop_add_io_event(s->ctx, s->socket, EVENT_READ, (callable_obj_base*)callable);
}

CALLABLE_DEFINE(tcp_send,
    CALLABLE_ARG(struct tcp_socket*, socket),
    CALLABLE_ARG(const void*, buffer),
    CALLABLE_ARG(int, size),
    CALLABLE_ARG(tcp_send_callback, callback),
    CALLABLE_ARG(void*, obj))
{
    tcp_send(socket, buffer, size, callback, obj);
}

void tcp_async_send(struct tcp_socket* s, const void* buffer, int size,
                    tcp_send_callback callback, void* obj)
{
    CALLABLE_OBJ(tcp_send)* callable = (CALLABLE_OBJ(tcp_send)*)
                                      malloc(sizeof(CALLABLE_OBJ(tcp_send)));

    CALLABLE_INIT(tcp_send, callable, s, buffer, size, callback, obj);

    event_loop_add_io_event(s->ctx, s->socket, EVENT_WRITE, (callable_obj_base*)callable);
}


