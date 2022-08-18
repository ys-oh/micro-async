#ifndef UDP_SOCKET_H_
#define UDP_SOCKET_H_

#include <unistd.h>
#include <errno.h>

#include "uasync/endpoint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct udp_socket {
    int socket;
    struct endpoint ep;
    void* ctx;
};

static inline
struct udp_socket* udp_socket(void* ctx, struct endpoint ep)
{
    struct udp_socket* udp = (struct udp_socket*)malloc(sizeof(struct udp_socket));
    bzero(udp, sizeof(struct udp_socket));
    udp->socket = socket(ep.sa.sa_family, SOCK_DGRAM, 0);
    udp->ep = ep;

    return udp;
}

static inline
void udp_socket_release(struct udp_socket* s)
{
    if (s->socket >= 0)
        close(s->socket);

    free(s);
}

static inline
struct udp_socket* udp_bind(void* ctx, struct endpoint ep)
{
    struct udp_socket* udp = udp_socket(ctx, ep);
    if (!udp)
        return NULL;

#ifdef SO_REUSEPORT
    int opt = 1;
    setsockopt(udp->socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
#endif

    if (bind(udp->socket, &udp->ep.sa, udp->ep.sa_len) < 0)
        return NULL;

    return udp;
}

typedef void (*udp_recv_callback)(int err, struct udp_socket* s, struct endpoint* ep, int bytes_transfered, void* obj);
typedef void (*udp_send_callback)(int err, struct udp_socket* s, struct endpoint* ep, int bytes_transfered, void* obj);

static inline
int udp_recvfrom(struct udp_socket* s, void* buffer, int size, struct endpoint* ep, udp_recv_callback callback, void* obj)
{
    int err = 0;
    int r_size = recvfrom(s->socket, buffer, size, 0, &ep->sa, &ep->sa_len);
    if (r_size < 0)
        err = errno;
    else if (r_size == 0)
        err = -1;

    if (callback)
        callback(err, s, ep, r_size, obj);

    return r_size;
}

static inline 
int udp_receive(struct udp_socket* s, void* buffer, int size, udp_recv_callback callback, void* obj)
{
    return udp_recvfrom(s, buffer, size, &s->ep, callback, obj);
}

static inline
int udp_sendto(struct udp_socket* s, const void* buffer, int size, struct endpoint* ep, udp_send_callback callback, void* obj)
{
    int err = 0;
    int t_size = sendto(s->socket, buffer, size, 0, &ep->sa, ep->sa_len);
    if (t_size < 0)
        err = -errno;
    else if (t_size == 0)
        err = -1;

    if (callback)
        callback(err, s, ep, t_size, obj);

    return t_size;
}

static inline 
int udp_send(struct udp_socket* s, const void* buffer, int size, udp_send_callback callback, void* obj)
{
    return udp_sendto(s, buffer, size, &s->ep, callback, obj);
}

void udp_async_recvfrom(struct udp_socket* s, void* buffer, int size, struct endpoint* ep,
                       udp_recv_callback callback, void* obj);

void udp_async_sendto(struct udp_socket* s, const void* buffer, int size, struct endpoint* ep,
                    udp_send_callback callback, void* obj);


#ifdef __cplusplus
}
#endif
#endif // UDP_SOCKET_H_
