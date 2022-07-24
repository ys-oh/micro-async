#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct endpoint
{
    struct sockaddr sa;
    socklen_t sa_len;
};

static inline struct endpoint endpoint_ipv6(const char* host, int port)
{
    struct endpoint ep;
    bzero(&ep, sizeof(ep));

    struct sockaddr_in6* sa6 = (struct sockaddr_in6*)&ep.sa;
    sa6->sin6_family = AF_INET6;
    sa6->sin6_port = htons(port);
    inet_pton(AF_INET6, host, &sa6->sin6_addr);

    ep.sa_len = sizeof (struct sockaddr_in6);

    return ep;
}

static inline struct endpoint endpoint_ipv4(const char* host, int port)
{
    struct endpoint ep;
    bzero(&ep, sizeof(ep));

    struct sockaddr_in* addr = (struct sockaddr_in*)&ep.sa;
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_pton(AF_INET, host, &addr->sin_addr);

    ep.sa_len = sizeof (struct sockaddr_in);

    return ep;
}

#ifdef __cplusplus
}
#endif
#endif // ENDPOINT_H_
