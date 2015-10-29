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
#include "./timer.h"
#include "./event_call.h"

#include <list/list.h>

extern async_mutex_t async_g_lock;

struct async_sem_private {
    unsigned char type;
    union {
        async_timer_t timer;
        async_event_call_t event_call;
    } data;
};

#define PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL 0
#define PRIVATE_SEM_TYPE_ADD_EVENT_CALL 0
#define PRIVATE_SEM_TYPE_ADD_TIMER 2
#define PRIVATE_SEM_TYPE_EXIT 3

void async_looper_init(void);
char async_notify_loop(async_looper_t looper, const struct async_sem_private *priv);

void async_timer_init(void);
void async_event_call_init(void);
async_timeout_t async_timer_exec(struct list_head *__FAR timers, async_timeout_t escaped);
async_timeout_t aasync_timer_add_timer(struct list_head *__FAR timers, async_timer_t timer, async_timeout_t escaped);


void async_event_call_exec(struct list_head *__FAR event_calls, async_event_call_t event_call);


#pragma DATA_SEG __RPAGE_SEG PAGED_RAM

#endif
