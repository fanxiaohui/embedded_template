/// \file event.c
/// \brief 异步事件调用实现.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#include <stdio.h>
#include "./private.h"
#include "misc/list.h"

static LIST_HEAD(free_list);

struct async_event {
    struct list_head head;
    async_event_callback_t cb;
    async_time_t timeout;
    async_time_t next_timestamp;
#if ASYNC_LOOPER_SIZE>1
    async_looper_t looper;
#endif
    void *__FAR dat;
};

static struct async_event events_pool[ASYNC_EVENT_SIZE];

void async_event_init(void) {
    int i;
    for (i = 0; i < ASYNC_EVENT_SIZE; ++i) {
        INIT_LIST_HEAD(&events_pool[i].head);
        list_add(&events_pool[i].head, &free_list);
    }
}

#if ASYNC_LOOPER_SIZE>1
async_event_t async_event_register(async_looper_t looper, async_event_callback_t cb, async_time_t timeout, void *__FAR dat) {
#else
async_event_t async_event_register(async_event_callback_t cb, async_time_t timeout, void *__FAR dat) {
#endif
    async_event_t ret;
    struct async_looper_command priv;

    async_lock_mutex(async_g_lock);
    if (list_empty(&free_list)) {
        async_unlock_mutex(async_g_lock);
        return ASYNC_EVENT_REGISTER_ERROR;
    }

    ret = (async_event_t)free_list.next;
    list_del(&ret->head);
    async_unlock_mutex(async_g_lock);
    ret->cb = cb;
    ret->dat = dat;
#if ASYNC_LOOPER_SIZE>1
    ret->looper = looper;
#endif
    ret->timeout = timeout;

    priv.type = LOOPER_COMMAND_TYPE_ADD_EVENT;
    priv.event = ret;

#if ASYNC_LOOPER_SIZE>1
    if (async_notify_loop(looper, &priv)) { // OK
#else
    if (async_notify_loop(&priv)) { // OK
#endif
        return ret;
    }

    // error put back to free_list.
    async_lock_mutex(async_g_lock);
    list_add(&ret->head, &free_list);
    async_unlock_mutex(async_g_lock);
    return ASYNC_EVENT_REGISTER_ERROR;
}

async_time_t async_event_exec_timeout(struct list_head *__FAR events) {
    char rc;
    async_event_t event;
    struct list_head *__FAR pos;
    struct list_head *__FAR n;

    async_time_t now = async_get_time();
    async_time_t most_recent_time = ASYNC_TIME_FOREVER;

    list_for_each_safe(pos, n, events) {
        event = (async_event_t)pos;

        if (event->next_timestamp > now) {
            goto __check_timestamp;
        }

        rc = event->cb(event, ASYNC_EVENT_ADDITION_DATA_TIMEOUT);
        if (rc == 0) {
            async_lock_mutex(async_g_lock);
            list_move(pos, &free_list);
            async_unlock_mutex(async_g_lock);
            continue;

        }

        event->next_timestamp = now + event->timeout;

__check_timestamp:
        if (event->next_timestamp < most_recent_time) {
            most_recent_time = event->next_timestamp;
        }
    }

    return most_recent_time;
}

async_time_t async_event_exec_trigger(async_event_t event, void *__FAR addition_data, struct list_head *__FAR events) {
    char rc;
    struct list_head *__FAR pos;
    struct list_head *__FAR n;

    list_for_each_safe(pos, n, events) {
        if (pos != &event->head) {
            continue;
        }

        rc = event->cb(event, addition_data);
        if (rc == 0) {
            async_lock_mutex(async_g_lock);
            list_move(&event->head, &free_list);
            async_unlock_mutex(async_g_lock);
            return ASYNC_TIME_FOREVER;
        }

        event->next_timestamp = async_get_time() + event->timeout;
        return event->next_timestamp;
    }
    return ASYNC_TIME_FOREVER;
}

char async_event_trigger(async_event_t event, void *__FAR addition_data) {
    struct async_looper_command priv;

    priv.type = LOOPER_COMMAND_TYPE_TRIGGER_CALL;
    priv.event = event;

#if ASYNC_LOOPER_SIZE>1
    return async_notify_loop(event->looper, &priv);
#else
    return async_notify_loop(&priv);
#endif
}

async_time_t async_event_add_to_looper_event_list(async_event_t event, struct list_head *__FAR list) {
    char rc = 1;

    if (event->timeout == 0) {
        rc = event->cb(event, ASYNC_EVENT_ADDITION_DATA_TIMEOUT);
    }

    if (rc == 0) {
        async_lock_mutex(async_g_lock);
        list_add(&event->head, &free_list);
        async_unlock_mutex(async_g_lock);
        return ASYNC_TIME_FOREVER;
    }
    event->next_timestamp = async_get_time() + event->timeout;
    list_add(&event->head, list);
    return event->next_timestamp;
}

void async_event_free_from_looper_event_list(async_event_t event, struct list_head *__FAR list) {
    struct list_head *__FAR pos;
    struct list_head *__FAR n;

    list_for_each_safe(pos, n, list) {
        if (pos == &event->head) {
            async_lock_mutex(async_g_lock);
            list_move(&event->head, &free_list);
            async_unlock_mutex(async_g_lock);
            break;
        }
    }
}

void async_event_free_all(struct list_head *__FAR events) {
    async_lock_mutex(async_g_lock);
    list_splice_init(events, &free_list);
    async_unlock_mutex(async_g_lock);
}

void async_event_set_callback(async_event_t event, async_event_callback_t cb) {
    event->cb = cb;
}


void async_event_set_timeout(async_event_t event, async_time_t timeout) {
    event->timeout = timeout;
}

void async_event_set_data(async_event_t event, void *__FAR dat) {
    event->dat = dat;
}

void *__FAR async_event_get_data(async_event_t event) {
    return event->dat;
}

char async_event_is_timeout(async_event_t event) {
    return event->next_timestamp <= async_get_time();
}

char async_event_cancel(async_event_t event) {
    struct async_looper_command priv;

    priv.type = LOOPER_COMMAND_TYPE_CANCEL_EVENT;
    priv.event = event;

#if ASYNC_LOOPER_SIZE>1
    return async_notify_loop(event->looper, &priv);
#else
    return async_notify_loop(&priv);
#endif
}

#if ASYNC_LOOPER_SIZE>1
async_looper_t async_event_get_looper(async_event_t event) {
    return event->looper;
}
#endif

