/// \file looper.c
/// \brief 异步循环实现.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#include "./private.h"
#include <stdio.h>
#include "misc/list.h"
#include "ringbuffer/ringbuffer.h"


#ifndef ASYNC_LOOPER_SIZE
#define ASYNC_LOOPER_SIZE 1
#endif

struct async_looper {
#if ASYNC_LOOPER_SIZE>1
    struct list_head head;
#endif
    os_sem_t sem;
    os_mutex_t lock;
    struct ringbuffer rb;
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
        INIT_LIST_HEAD(&looper_pool[i].events);
        ringbuffer_init(&looper_pool[i].rb, looper_pool[i].buf, sizeof(looper_pool[i].buf));
#if ASYNC_LOOPER_SIZE>1
        INIT_LIST_HEAD(&looper_pool[i].head);
        list_add(&looper_pool[i].head, &free_list);
#endif
    }
}

async_looper_t async_looper_create(void) {
#if ASYNC_LOOPER_SIZE>1
    async_looper_t ret;
    os_lock_mutex(async_g_lock);
    if (list_empty(&free_list)) {
        os_unlock_mutex(async_g_lock);
        return ASYNC_LOOPER_CREATE_ERROR;
    }

    ret = (async_looper_t)(free_list.next);
    list_del(&(ret->head));
    os_unlock_mutex(async_g_lock);

    ret->lock = async_create_mutex();
    ret->sem = async_create_sem();

    if (!ret->lock) {
        goto __error;
    }
    if (!ret->sem) {
        goto __error;
    }

    os_lock_mutex(async_g_lock);
    list_add(&(ret->head), &used_list);
    os_unlock_mutex(async_g_lock);
    return ret;

__error:
    if (ret->lock) {
        os_destroy_mutex(ret->lock);
        ret->lock = NULL;
    }

    if (ret->sem) {
        os_destroy_sem(ret->sem);
        ret->sem = NULL;
    }
    os_lock_mutex(async_g_lock);
    list_add(&(ret->head), &free_list);
    os_unlock_mutex(async_g_lock);
    return NULL;

#else
    if (looper->lock != NULL) {
        return NULL;
    }

    looper->lock = os_create_mutex();
    if (looper->lock == NULL) {
        goto __error;
    }
    looper->sem = os_create_sem();
    if (looper->sem == NULL) {
        goto __error;
    }
    return looper;

__error:
    if (looper->lock != NULL) {
        os_destroy_mutex(looper->lock);
        looper->lock = NULL;
    }
    if (looper->sem != NULL) {
        os_destroy_sem(looper->sem);
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

    os_lock_mutex(looper->lock);
    ret = ringbuffer_write(&(looper->rb), (unsigned char *__FAR)priv, sizeof(*priv));
    os_unlock_mutex(looper->lock);

    if (ret != sizeof(*priv)) {
        return 0;
    }

    return os_post_sem(looper->sem);
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

void async_looper_loop(async_looper_t looper) {
    os_time_t now;
    os_time_t diff;
    os_time_t next_timestamp;

    struct async_looper_command cmd;

    next_timestamp = OS_TIME_FOREVER;

    while (1) {
        now = os_get_time();
        if (next_timestamp <= now) {
            next_timestamp = async_event_exec_timeout(&looper->events);
            now = os_get_time();
        }

        if (next_timestamp <= now) {
            diff = 1;
        } else {
            diff = next_timestamp - now;
        }

        if (0 == os_pend_sem(looper->sem, diff)) {
            // 等待超时.
            continue;
        }


        os_lock_mutex(looper->lock);
        if (sizeof(cmd) != ringbuffer_read(&(looper->rb), (unsigned char *__FAR)&cmd, sizeof(cmd))) {
            ringbuffer_clear(&(looper->rb));
            os_clear_sem(looper->sem);
            os_unlock_mutex(looper->lock);
            continue;
        }
        os_unlock_mutex(looper->lock);

        if (cmd.type == LOOPER_COMMAND_TYPE_TRIGGER_CALL) {
            os_time_t timestamp = async_event_exec_trigger(cmd.event, cmd.addition_data, &looper->events);
            if (timestamp < next_timestamp) {
                next_timestamp = timestamp;
            }
            continue;
        }

        if (cmd.type == LOOPER_COMMAND_TYPE_ADD_EVENT) {
            os_time_t timestamp = async_event_add_to_looper_event_list(cmd.event, &looper->events);
            if (timestamp < next_timestamp) {
                next_timestamp = timestamp;
            }
            continue;
        }

        if (cmd.type == LOOPER_COMMAND_TYPE_CANCEL_EVENT) {
            async_event_free_from_looper_event_list(cmd.event, &looper->events);
            continue;
        }

        if (cmd.type == LOOPER_COMMAND_TYPE_EXIT) {
            async_event_free_all(&looper->events);
            os_destroy_mutex(looper->lock);
            looper->lock = NULL;
            os_destroy_sem(looper->sem);
            looper->sem = NULL;

#if ASYNC_LOOPER_SIZE>1
            os_lock_mutex(async_g_lock);
            list_move(&looper->head, &free_list);
            os_unlock_mutex(async_g_lock);
#endif
            return;
        }
    }
}

