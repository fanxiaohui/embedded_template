#include "./private.h"
#include <list/list.h>

static LIST_HEAD(free_list);
static LIST_HEAD(used_list);

struct async_event_call {
    struct list_head head;
    event_handler func;
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

async_event_call_t *__FAR async_event_call_post(event_handler func, void *__FAR dat) {
    async_event_call_t *__FAR ret = ASYNC_EVENT_CALL_ERROR;

    __lock();
    if (!list_empty(&free_list)) {
        ret = (async_event_call_t *__FAR)free_list.next;
        ret->func = func;
        ret->dat = dat;
        list_move(&ret->head, &used_list);
    }
    __unlock();
    return ret;
}

void async_event_call_exec(async_event_call_t *__FAR event_call) {
    struct list_head *__FAR pos;
    list_for_each(pos, &used_list) {
        if (pos == &event_call->head) {
            char ret = event_call->func(event_call->dat);
            if (!ret) {
                __lock();
                list_move(&event_call->head, &free_list);
                __unlock();
            }
            return;
        }
    }
}



char async_event_call_trigger(async_event_call_t *__FAR event_call) {
    struct async_sem_private priv;
    priv.type = PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL;
    priv.data.event_call = event_call;
    return async_notify_loop(&priv);
}

