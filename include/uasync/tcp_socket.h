#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_

#include <unistd.h>
#include <errno.h>

#include "uasync/endpoint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tcp_socket
{
    int socket;
    struct endpoint ep;
    void* ctx;
};

struct tcp_acceptor
{
    int socket;
    struct endpoint ep;
    void* ctx;
};

static inline
struct tcp_socket* tcp_socket_new(void* ctx)
{
    struct tcp_socket* s = (struct tcp_socket*)malloc(sizeof(struct tcp_socket));
    bzero(s, sizeof(struct tcp_socket));
    s->socket = -1;
    s->ctx = ctx;
    return s;
}

static inline
void tcp_socket_release(struct tcp_socket* s)
{
    if (s->socket > 0)
        close(s->socket);

    free(s);
}

static inline
struct tcp_acceptor* tcp_bind(void* ctx, struct endpoint ep)
{
    int s = socket(ep.sa.sa_family, SOCK_STREAM, 0);
    if (s < 0)
        return NULL;

#ifdef SO_REUSEPORT
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
#endif

    if (bind(s, &ep.sa, ep.sa_len) < 0)
        goto close_return;

    if (listen(s, 5) < 0)
        goto close_return;

    struct tcp_acceptor* acceptor = (struct tcp_acceptor*)malloc(sizeof(struct tcp_acceptor));
    acceptor->ep = ep;
    acceptor->socket = s;
    acceptor->ctx = ctx;

    return acceptor;

close_return:
    close(s);
    return NULL;
}

static inline
void tcp_acceptor_release(struct tcp_acceptor* acceptor)
{
    close (acceptor->socket);
    free(acceptor);
}

#include <fcntl.h>

static inline int make_nonblock(int fd)
{
    /*
        as socket nonblocking,
        socket syscall function would be
        return -1 immediatly with setting
        'errno' EWOULDBLOCK or EAGAIN.
    */
    int flag;
    flag = fcntl(fd, F_GETFL, 0 );
    if (flag & O_NONBLOCK)
        return flag;

    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);

    return flag;
}

static inline int make_block(int fd)
{
    int flag;
    flag = fcntl(fd, F_GETFL, 0 );
    flag &= ~(O_NONBLOCK);
    fcntl(fd, F_SETFL, flag);

    return flag;
}

typedef void (*tcp_accept_callback)(int err, struct tcp_acceptor* acceptor, struct tcp_socket* socket, void* obj);
typedef void (*tcp_connect_callback)(int err, struct tcp_socket* socket, struct endpoint* ep, void* obj);
typedef void (*tcp_recv_callback)(int err, struct tcp_socket* socket, int bytes_transfered, void* obj);
typedef void (*tcp_send_callback)(int err, struct tcp_socket* socket, int bytes_transfered, void* obj);

static inline
int tcp_accept(struct tcp_acceptor* acceptor, struct tcp_socket* socket,
               tcp_accept_callback callback, void* obj)
{
    int err = 0;
    int s = accept(acceptor->socket, &socket->ep.sa, &socket->ep.sa_len);
    if (s < 0)
        err = -errno;

    socket->socket = s;

    if (callback)
        callback(err, acceptor, socket, obj);

    return err;
}

static inline
int tcp_connect(struct tcp_socket* s, struct endpoint ep,
                tcp_connect_callback callback, void* obj)
{
    if (s->socket < 0)
        s->socket = socket(ep.sa.sa_family, SOCK_STREAM, 0);

    int err = 0;
    int ret = connect(s->socket, &ep.sa, ep.sa_len);
    if (ret < 0)
    {
        err = -errno;
        if (err != -EAGAIN && err != -EWOULDBLOCK)
        {
            close(s->socket);
            s->socket = -1;
        }
    }

    s->ep = ep;

    if (callback)
        callback(err, s, &s->ep, obj);

    return err;
}

static inline
int tcp_receive(struct tcp_socket* s, void* buffer, int size,
                tcp_recv_callback callback, void* obj)
{
    int err = 0;
    int r_size = recv(s->socket, buffer, size, 0);
    if (r_size < 0)
        err = -errno;
    else if(r_size == 0)
        err = -1;

    if (callback)
        callback(err, s, r_size, obj);

    return r_size;
}

static inline
int tcp_send(struct tcp_socket* s, const void* buffer, int size,
             tcp_send_callback callback, void* obj)
{
    int err = 0;
    int t_size = send(s->socket, buffer, size, 0);
    if (t_size < 0)
        err = -errno;

    if (callback)
        callback(err, s, t_size, obj);

    return t_size;
}


void tcp_async_accept(struct tcp_acceptor* acceptor, struct tcp_socket* socket,
                      tcp_accept_callback callback, void* obj);

void tcp_async_connect(struct tcp_socket* s, struct endpoint ep,
                       tcp_connect_callback callback, void* obj);

void tcp_async_receive(struct tcp_socket* s, void* buffer, int size,
                       tcp_recv_callback callback, void* obj);

void tcp_async_send(struct tcp_socket* s, const void* buffer, int size,
                    tcp_send_callback callback, void* obj);

#ifdef __cplusplus
}
#endif
#endif // TCP_SOCKET_H_
