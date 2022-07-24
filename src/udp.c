#include "uasync/udp_socket.h"
#include "uasync/event_loop.h"

CALLABLE_DEFINE(udp_connect,
    CALLABLE_ARG(struct udp_socket*, socket),
    CALLABLE_ARG(struct endpoint*, ep),
    CALLABLE_ARG(udp_connect_callback, callback),
    CALLABLE_ARG(void*, obj))
{
    udp_connect(socket, *ep, callback, obj);
}

void udp_async_connect(struct udp_socket* s, struct endpoint ep,
                       udp_connect_callback callback, void* obj)
{
    s->ep = ep;
    CALLABLE_OBJ(udp_connect)* callable = (CALLABLE_OBJ(udp_connect)*)
                                          malloc(sizeof(CALLABLE_OBJ(udp_connect)));

    CALLABLE_INIT(udp_connect, callable, s, &s->ep, callback, obj);

    event_loop_add_timed_event(s->ctx, 1000000, (callable_obj_base*)callable);
}

CALLABLE_DEFINE(udp_recvfrom,
    CALLABLE_ARG(struct udp_socket*, socket),
    CALLABLE_ARG(void*, buffer),
    CALLABLE_ARG(int, size),
    CALLABLE_ARG(struct endpoint*, ep),
    CALLABLE_ARG(udp_recv_callback, callback),
    CALLABLE_ARG(void*, obj))
{
    udp_recvfrom(socket, buffer, size, ep, callback, obj);
}

void udp_async_recvfrom(struct udp_socket* s, void* buffer, int size, struct endpoint* ep,
                       udp_recv_callback callback, void* obj)
{
    CALLABLE_OBJ(udp_recvfrom)* callable = (CALLABLE_OBJ(udp_recvfrom)*)
                                          malloc(sizeof(CALLABLE_OBJ(udp_recvfrom)));

    CALLABLE_INIT(udp_recvfrom, callable, s, buffer, size, ep, callback, obj);

    event_loop_add_io_event(s->ctx, s->socket, EVENT_READ, (callable_obj_base*)callable);
}

CALLABLE_DEFINE(udp_sendto,
    CALLABLE_ARG(struct udp_socket*, socket),
    CALLABLE_ARG(const void*, buffer),
    CALLABLE_ARG(int, size),
    CALLABLE_ARG(struct endpoint*, ep),
    CALLABLE_ARG(udp_recv_callback, callback),
    CALLABLE_ARG(void*, obj))
{
    udp_sendto(socket, buffer, size, ep, callback, obj);
}

void udp_async_sendto(struct udp_socket* s, const void* buffer, int size, struct endpoint* ep,
                    udp_send_callback callback, void* obj)
{
    CALLABLE_OBJ(udp_sendto)* callable = (CALLABLE_OBJ(udp_sendto)*)
                                          malloc(sizeof(CALLABLE_OBJ(udp_sendto)));

    CALLABLE_INIT(udp_sendto, callable, s, buffer, size, ep, callback, obj);

    event_loop_add_io_event(s->ctx, s->socket, EVENT_WRITE, (callable_obj_base*)callable);
}
