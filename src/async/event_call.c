/// \file event_call.c
/// \brief 异步事件调用实现.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#include "./private.h"
#include <list/list.h>

static LIST_HEAD(free_list);

struct async_event_call {
    struct list_head head;
    async_event_callback_t cb;
    async_looper_t looper;
    void *__FAR dat;
};

static struct async_event_call event_calls[ASYNC_EVENT_CALL_SIZE];

void async_event_call_init(void) {
    int i;
    for (i = 0; i < ASYNC_EVENT_CALL_SIZE; ++i) {
        INIT_LIST_HEAD(&event_calls[i].head);
        list_add(&event_calls[i].head, &free_list);
    }
}

async_event_call_t async_event_call_register(async_looper_t looper, async_event_callback_t cb, void *__FAR dat) {
    async_event_call_t ret;
    struct async_sem_private priv;

    async_lock_mutex(async_g_lock);
    if (list_empty(&free_list)) {
        async_unlock_mutex(async_g_lock);
        return ASYNC_EVENT_CALL_REGISTER_ERROR;
    }

    ret = (async_event_call_t)free_list.next;
    list_del(&ret->head);
    async_unlock_mutex(async_g_lock);

    ret->cb = cb;
    ret->dat = dat;
    ret->looper = looper;

    priv.type = PRIVATE_SEM_TYPE_ADD_EVENT_CALL;
    priv.data.event_call = ret;
    if (async_notify_loop(looper, &priv)) { // OK
        return ret;
    }

    // error put back to free_list.
    async_lock_mutex(async_g_lock);
    list_add(&ret->head, &free_list);
    async_unlock_mutex(async_g_lock);
    return ASYNC_EVENT_CALL_REGISTER_ERROR;
}

void async_event_call_exec(struct list_head *__FAR event_calls, async_event_call_t event_call) {
    struct list_head *__FAR pos;

    list_for_each(pos, event_calls) {
        if (pos != &event_call->head) {
            continue;
        }
        if (event_call->cb(event_call->dat)) {

            return;
        }

        // cancel the event call.
        async_lock_mutex(async_g_lock);
        list_move(&event_call->head, &free_list);
        async_unlock_mutex(async_g_lock);
    }
}

char async_event_call_trigger(async_event_call_t event_call) {
    struct async_sem_private priv;

    priv.type = PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL;
    priv.data.event_call = event_call;

    return async_notify_loop(event_call->looper, &priv);
}

