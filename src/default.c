#include "uasync/event_loop.h"

#include <stddef.h>
#include <stdlib.h>

#include <ev.h>

#define event_entry_malloc()    (event_entry*)malloc(sizeof(event_entry))
#define event_entry_free(obj)   free((event_entry*)obj)

typedef struct event_entry
{
    union {
        ev_io io;
        ev_timer to;
    };
    callable_obj_base* callable;
} event_entry;

static void ev_io_cb(EV_P_ ev_io* io, short revents)
{
    struct event_entry* entry = (struct event_entry*)(((char*)io) - offsetof(struct event_entry, io));
    ev_io_stop(EV_A_ &entry->io);

    callable_invoke(entry->callable);
    callable_free(entry->callable);

    event_entry_free(entry);
}

static void ev_to_cb(EV_P_ ev_timer* w, short revents)
{
    struct event_entry* entry = (struct event_entry*)(((char*)w) - offsetof(struct event_entry, to));
    ev_timer_stop(EV_A_ &entry->to);

    callable_invoke(entry->callable);
    callable_free(entry->callable);

    event_entry_free(entry);
}

/* interface implementations */
static struct ev_loop* default_loop = NULL;

void* event_loop_create()
{
    if (default_loop)
        return ev_loop_new(EVFLAG_AUTO);

    default_loop = ev_default_loop(EVFLAG_AUTO);

    return default_loop;
}

void event_loop_release(void* ctx)
{
    if (!ev_is_default_loop (ctx))
        ev_loop_destroy (ctx);
}

void event_loop_add_io_event(void* ctx, int fd, int ev_flag, callable_obj_base* obj)
{
    struct ev_loop* loop = ctx ? (struct ev_loop*)ctx : ev_default_loop(0);

    event_entry* entry = event_entry_malloc();
    ev_init(&entry->io, (void*)ev_io_cb);

    entry->callable = obj;

    if (fd >= 0)
    {
        short events = 0;
        if (ev_flag & EVENT_READ)   events |= EV_READ;
        if (ev_flag & EVENT_WRITE)  events |= EV_WRITE;

        ev_io_set(&entry->io, fd, events);
        ev_io_start(loop, &entry->io);
    }
}

void event_loop_add_timed_event(void* ctx, uint64_t timeout, callable_obj_base* obj)
{
    struct ev_loop* loop = ctx ? (struct ev_loop*)ctx : ev_default_loop(0);

    event_entry* entry = event_entry_malloc();
    ev_init(&entry->to, (void*)ev_to_cb);

    entry->callable = obj;

    ev_tstamp evtimeout = (double)timeout * 1e-6;
    evtimeout = evtimeout ? evtimeout : 1e-6;

    if (evtimeout >= 0.)
    {
        ev_timer_set(&entry->to, evtimeout, 0.);
        ev_timer_start(loop, &entry->to);
    }
}

void event_loop_run(void* ctx)
{
    if (!ctx)
        ctx = ev_default_loop(0);

    ev_run(ctx, 0);
}
