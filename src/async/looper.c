/// \file looper.c
/// \brief 异步循环实现.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#include "./private.h"
#include <stdio.h>
#include <list/list.h>
#include <ringbuffer/ringbuffer.h>

struct async_looper {
    struct list_head head;
    async_sem_t sem;
    async_mutex_t lock;
    ringbuffer_t rb;
    unsigned char buf[100];
    struct list_head events;
};

#if ASYNC_LOOPER_SIZE>1
static LIST_HEAD(used_list);
static LIST_HEAD(free_list);
struct async_looper looper_pool[ASYNC_LOOPER_SIZE];
#else
#define looper_pool looper
struct async_looper looper[ASYNC_LOOPER_SIZE];
#endif

void async_looper_init(void) {
    int i;
    for (i = 0; i < ASYNC_LOOPER_SIZE; ++i) {
        INIT_LIST_HEAD(&looper_pool[i].head);
        INIT_LIST_HEAD(&looper_pool[i].events);
        ringbuffer_init(&looper_pool[i].rb, looper_pool[i].buf, sizeof(looper_pool[i].buf));
#if ASYNC_LOOPER_SIZE>1
        list_add(&looper_pool[i].head, &free_list);
#endif
    }
}

async_looper_t async_looper_create(void) {

#if ASYNC_LOOPER_SIZE>1
    async_looper_t ret;
    async_lock_mutex(async_g_lock);
    if (list_empty(&free_list)) {
        async_unlock_mutex(async_g_lock);
        return ASYNC_LOOPER_CREATE_ERROR;
    }

    ret = (async_looper_t)(free_list.next);
    list_del(&(ret->head));
    async_unlock_mutex(async_g_lock);

    ret->lock = async_create_mutex();
    ret->sem = async_create_sem();

    return ret;
#else
    if (looper->lock != NULL) { return NULL; }

    looper->lock = async_create_mutex();
    if (looper->lock == NULL) { goto __error; }
    looper->sem = async_create_sem();
    if (looper->sem == NULL) { goto __error; }
    return looper;

__error:
    if (looper->lock != NULL) {
        async_destroy_mutex(looper->lock);
        looper->lock = NULL;
    }
    if (looper->sem != NULL) {
        async_destroy_sem(looper->sem);
        looper->sem = NULL;
    }
    return NULL;
#endif
}

#if ASYNC_LOOPER_SIZE>1
char async_notify_loop(async_looper_t looper, const struct async_looper_command *priv) {
#else
char async_notify_loop(const struct async_looper_command *priv) {
#endif
    RINGBUFFER_SIZE_TYPE ret;

    async_lock_mutex(looper->lock);
    ret = ringbuffer_write(&(looper->rb), (unsigned char *__FAR)priv, sizeof(*priv));
    async_unlock_mutex(looper->lock);

    if (ret != sizeof(*priv)) {
        return 0;
    }

    return async_post_sem(looper->sem);
}

char async_looper_exit(async_looper_t looper) {
    struct async_looper_command priv;

    priv.type = LOOPER_COMMAND_TYPE_EXIT;

#if ASYNC_LOOPER_SIZE>1
    return async_notify_loop(looper, &priv);
#else
    return async_notify_loop(&priv);
#endif
}

#if ASYNC_LOOPER_SIZE>1
void async_looper_loop(async_looper_t looper) {
#else
void async_looper_loop(void) {
#endif
    async_timeout_t wait_time = ASYNC_TIMEOUT_FOREVER;
    struct async_looper_command cmd;

    async_time_t now = async_get_time();
    async_time_t last_time = now;
    async_time_t escaped;

    while (1) {
        if (wait_time == 0) {
            wait_time = async_event_exec_timeout(&looper->events, async_get_time() - last_time);
            last_time = async_get_time();
        }
        now = async_get_time();
        if (0 == async_pend_sem(looper->sem, wait_time)) {
            wait_time = 0;
            continue;
        }

        // 等待超时.
        escaped = async_get_time() - now;
        if (escaped >= wait_time) {
            wait_time = 0;
        } else {
            wait_time = wait_time - escaped;
        }

        async_lock_mutex(looper->lock);
        if (sizeof(cmd) != ringbuffer_read(&(looper->rb), (unsigned char *__FAR)&cmd, sizeof(cmd))) {
            ringbuffer_clear(&(looper->rb));
            async_clear_sem(looper->sem);
            async_unlock_mutex(looper->lock);
            continue;
        }
        async_unlock_mutex(looper->lock);

        if (cmd.type == LOOPER_COMMAND_TYPE_TRIGGER_CALL) {
            async_timeout_t this_wait_time = async_event_exec_trigger(cmd.data.event, &looper->events, async_get_time() - last_time);
            if (this_wait_time < wait_time) { wait_time = this_wait_time; }
            continue;
        }

        if (cmd.type == LOOPER_COMMAND_TYPE_ADD_EVENT) {
            async_timeout_t this_wait_time = async_event_add_to_looper_event_list(cmd.data.event, &looper->events, async_get_time() - last_time);
            if (this_wait_time < wait_time) { wait_time = this_wait_time; }
            continue;
        }

        if (cmd.type == LOOPER_COMMAND_TYPE_CANCEL_EVENT) {
            async_event_remove_frome_looper_event_list(cmd.data.event, &looper->events);
            continue;
        }
    }
}

