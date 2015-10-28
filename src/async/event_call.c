#include "./private.h"
#include <list/list.h>

static LIST_HEAD(free_list);

struct async_event_call {
    struct list_head head;
    event_handler func;
    async_looper_t *looper;
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

async_event_call_t *__FAR async_event_call_post(async_looper_t *__FAR looper, event_handler func, void *__FAR dat) {
    async_event_call_t *__FAR ret = ASYNC_EVENT_CALL_ERROR;

    async_lock_mutex(async_g_lock);
    if (list_empty(&free_list)) {
        async_unlock_mutex(async_g_lock);
        return ret;
    }
    ret = (async_event_call_t *__FAR)free_list.next;
    list_del(&ret->head);
    ret->func = func;
    ret->dat = dat;
    ret->looper = looper;

    {
        struct async_sem_private priv;
        priv.type = PRIVATE_SEM_TYPE_ADD_EVENT_CALL;
        priv.data.event_call = ret;
        if (!async_notify_loop(looper, &priv)) {
            async_lock_mutex(async_g_lock);
            list_add(&ret->head, &free_list);
            async_unlock_mutex(async_g_lock);
            ret = ASYNC_EVENT_CALL_CANCEL;
        }
    }

    return ret;
}

void async_event_call_exec(struct list_head *__FAR event_calls, async_event_call_t *__FAR event_call) {
    struct list_head *__FAR pos;
    list_for_each(pos, event_calls) {
        if (pos != &event_call->head) {
            continue;
        }
        if (!event_call->func(event_call->dat)) {
            async_lock_mutex(async_g_lock);
            list_move(&event_call->head, &free_list);
            async_unlock_mutex(async_g_lock);
        }
        return;
    }
}



char async_event_call_trigger(async_event_call_t *__FAR event_call) {
    struct async_sem_private priv;
    priv.type = PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL;
    priv.data.event_call = event_call;
    return async_notify_loop(event_call->looper, &priv);
}

