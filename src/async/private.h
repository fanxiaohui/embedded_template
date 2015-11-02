#ifndef __ASYNC_PRIVATE_H__
#define __ASYNC_PRIVATE_H__

#include "./platform.h"

#ifndef ASYNC_LOOPER_SIZE
#define ASYNC_LOOPER_SIZE 20
#endif

#ifndef ASYNC_TIMER_SIZE
#define ASYNC_TIMER_SIZE 20
#endif


#ifndef ASYNC_EVENT_CALL_SIZE
#define ASYNC_EVENT_CALL_SIZE 20
#endif

#include "./async.h"
#include "./looper.h"
#include "./event.h"

#include <list/list.h>

extern async_mutex_t async_g_lock;

struct async_looper_command {
    unsigned char type;
    union {
        async_event_t event;
    } data;
};

//#define PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL 0
//#define PRIVATE_SEM_TYPE_ADD_EVENT_CALL 0
//#define PRIVATE_SEM_TYPE_ADD_TIMER 2
//#define PRIVATE_SEM_TYPE_EXIT 3

#define LOOPER_COMMAND_TYPE_TRIGGER_CALL 10
#define LOOPER_COMMAND_TYPE_ADD_EVENT 11
#define LOOPER_COMMAND_TYPE_CANCEL_EVENT 12
#define LOOPER_COMMAND_TYPE_EXIT 13

void async_looper_init(void);
#if ASYNC_LOOPER_SIZE>1
char async_notify_loop(async_looper_t looper, const struct async_looper_command *cmd);
#else
char async_notify_loop(const struct async_looper_command *cmd);
#endif

void async_event_init(void);
async_timeout_t async_event_add_to_looper_event_list(async_event_t event, struct list_head *__FAR list, async_timeout_t escaped_offset);
async_timeout_t async_event_exec_timeout(struct list_head *__FAR events, async_timeout_t escaped);
async_timeout_t async_event_exec_trigger(async_event_t event, struct list_head *__FAR events, async_timeout_t escaped_offset);

#endif
