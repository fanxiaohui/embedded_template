/// \file event.c
/// \brief 异步事件调用实现.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#include "./private.h"
#include <list/list.h>

static LIST_HEAD(free_list);

struct async_event {
    struct list_head head;
    async_looper_t looper;
    async_event_callback_t cb;
    async_timeout_t timeout;
    async_timeout_t escaped;
    void *__FAR dat;
};

static struct async_event events_pool[ASYNC_EVENT_CALL_SIZE];

void async_event_init(void) {
    int i;
    for (i = 0; i < ASYNC_EVENT_CALL_SIZE; ++i) {
        INIT_LIST_HEAD(&events_pool[i].head);
        list_add(&events_pool[i].head, &free_list);
    }
}

async_event_t async_event_register(async_looper_t looper, async_event_callback_t cb, async_timeout_t timeout, void *__FAR dat) {
    async_event_t ret;
    struct async_sem_private priv;

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
    ret->looper = looper;
    ret->timeout = timeout;

    priv.type = PRIVATE_SEM_TYPE_ADD_EVENT;
    priv.data.event = ret;
    if (async_notify_loop(looper, &priv)) { // OK
        return ret;
    }

    // error put back to free_list.
    async_lock_mutex(async_g_lock);
    list_add(&ret->head, &free_list);
    async_unlock_mutex(async_g_lock);
    return ASYNC_EVENT_REGISTER_ERROR;
}

void async_event_exec(struct list_head *__FAR events, async_event_t event) {
    struct list_head *__FAR pos;

    list_for_each(pos, events) {
        if (pos != &event->head) {
            continue;
        }
        if (event->cb(event->dat)) {
            return;
        }

        // cancel the event call.
        async_lock_mutex(async_g_lock);
        list_move(&event->head, &free_list);
        async_unlock_mutex(async_g_lock);
    }
}

char async_event_trigger(async_event_t event) {
    struct async_sem_private priv;

    priv.type = PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL;
    priv.data.event = event;

    return async_notify_loop(event->looper, &priv);
}


