#ifndef __ASYNC_PRIVATE_H__
#define __ASYNC_PRIVATE_H__

#include "./platform.h"

#include "./async.h"
#include "./looper.h"
#include "./event.h"
#include "misc/list.h"

extern async_mutex_t async_g_lock;

struct async_looper_command {
    unsigned char type;
    union {
        async_event_t event;
    } data;
};

#define LOOPER_COMMAND_TYPE_CANCEL_EVENT       0
#define LOOPER_COMMAND_TYPE_TRIGGER_CALL       1
#define LOOPER_COMMAND_TYPE_ADD_EVENT          2
#define LOOPER_COMMAND_TYPE_EXIT               3

void async_looper_init(void);
#if ASYNC_LOOPER_SIZE>1
char async_notify_loop(async_looper_t looper, const struct async_looper_command *cmd);
#else
char async_notify_loop(const struct async_looper_command *cmd);
#endif

void async_event_init(void);
async_time_t async_event_exec_timeout(struct list_head *__FAR events);
void async_event_free_from_looper_event_list(async_event_t event, struct list_head *__FAR list);
void async_event_free_all(struct list_head *events);
async_time_t async_event_exec_trigger(async_event_t event, struct list_head *__FAR events);
async_time_t async_event_add_to_looper_event_list(async_event_t event, struct list_head *__FAR list);

#endif
