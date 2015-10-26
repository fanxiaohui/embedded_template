#ifndef __ASYNC_PRIVATE_H__
#define __ASYNC_PRIVATE_H__

#include "./platform.h"

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


struct async_sem_private {
    unsigned char type;
    union {
        async_event_call_t *__FAR event_call;
        async_timer_t *__FAR timer;
    } data;
};

#define PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL 0
#define PRIVATE_SEM_TYPE_ADD_TIMER 1
#define PRIVATE_SEM_TYPE_EXIT 2

void async_looper_init(void);
char async_notify_loop(struct async_sem_private *priv);
extern async_mutex_t lock;
static inline void __lock() {
    async_lock_mutex(lock);
}

static inline void __unlock() {
    async_unlock_mutex(lock);
}

extern async_sem_t sem;

static inline void __post_sem() {
    async_post_sem(sem);
}


void async_timer_init(void);
void async_event_call_init(void);
async_timeout_t async_timer_exec(async_timeout_t escaped);
async_timeout_t async_timer_add_timer(async_timer_t *__FAR timer, async_timeout_t escaped);


void async_event_call_exec(async_event_call_t *__FAR event_call);



#pragma DATA_SEG __RPAGE_SEG PAGED_RAM

#endif
