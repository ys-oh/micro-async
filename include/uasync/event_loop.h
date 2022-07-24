#ifndef EVENT_LOOP_H_ 
#define EVENT_LOOP_H_ 

#include "uasync/callable.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_LOOP_DEFAULT NULL

#define EVENT_READ  1
#define EVENT_WRITE 2

extern void* event_loop_create();
extern void event_loop_release(void* ctx);

extern void event_loop_add_io_event(void* ctx, int fd, int event, callable_obj_base* callable);
extern void event_loop_add_timed_event(void* ctx, uint64_t timeout, callable_obj_base* callable);
extern void event_loop_run(void* ctx);

#ifdef __cplusplus
}
#endif
#endif // EVENT_LOOP_H_ 
