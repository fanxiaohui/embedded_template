#include "./private.h"
#include <list/list.h>

static LIST_HEAD(free_list);

struct async_timer {
    // �������, ��������ǵ�һ����Ա.
    struct list_head head;
    // ʣ��ʱ��.
    async_timeout_t timeout;
    // ִ�еĺ���
    async_timer_handler_t func;
    // ����LOOPER
    async_looper_t *looper;
    // �û���������.
    void *__FAR dat;
};

struct async_timer timers[ASYNC_TIMER_SIZE];

void async_timer_init(void) {
    int i;
    for (i = 0; i < ASYNC_TIMER_SIZE; ++i) {
        INIT_LIST_HEAD(&timers[i].head);
        list_add(&timers[i].head, &free_list);
    }
}

async_timer_t *__FAR async_timer_register(async_looper_t *__FAR looper, async_timer_handler_t func, async_timeout_t timeout, void *__FAR dat) {
    async_timer_t *__FAR ret = ASYNC_TIMER_ERROR;

    async_lock_mutex(async_g_lock);
    if (list_empty(&free_list)) {
        async_unlock_mutex(async_g_lock);
        return ret;
    }
    ret = (async_timer_t *__FAR)(free_list.next);
    list_del(&ret->head);
    async_unlock_mutex(async_g_lock);

    ret->func = func;
    ret->looper = looper;
    ret->dat = dat;
    ret->timeout = timeout;

    {
        struct async_sem_private priv;
        priv.type = PRIVATE_SEM_TYPE_ADD_TIMER;
        priv.data.timer = ret;
        if (!async_notify_loop(looper, &priv)) {
            async_lock_mutex(async_g_lock);
            list_add(&ret->head, &free_list);
            async_unlock_mutex(async_g_lock);
            ret = ASYNC_TIMER_ERROR;
        }
    }

    return ret;
}

async_timeout_t async_timer_exec(struct list_head *__FAR timers, async_timeout_t escaped) {
    struct list_head *__FAR pos;
    struct list_head *__FAR n;

    async_timeout_t mini_timeout = -1;

    list_for_each_safe(pos, n, timers) {
        struct async_timer *__FAR timer = (struct async_timer * __FAR)pos;
        if (timer->timeout > escaped) {
            timer->timeout -= escaped;
        } else {
            timer->timeout = timer->func(timer);
            if (timer->timeout == 0) {
                async_lock_mutex(async_g_lock);
                list_move(pos, &free_list);
                async_unlock_mutex(async_g_lock);
            }
        }

        if (mini_timeout > timer->timeout) {
            mini_timeout = timer->timeout;
        }
    }
    return mini_timeout;
}

async_timeout_t aasync_timer_add_timer(struct list_head *__FAR timers, async_timer_t *__FAR timer, async_timeout_t escaped) {
    async_timeout_t mini_timeout = async_timer_exec(timers, escaped);
    if (timer->timeout <= 0) {
        timer->timeout = timer->func(timer);
        if (timer->timeout == 0) {
            async_lock_mutex(async_g_lock);
            list_move(&timer->head, &free_list);
            async_unlock_mutex(async_g_lock);
            return mini_timeout;
        }
    }

    list_add(&(timer->head), timers);
    if (mini_timeout < timer->timeout) return mini_timeout;
    return timer->timeout;
}


void async_timer_set_handler(async_timer_t *__FAR timer, async_timer_handler_t func) {
    timer->func = func;
}

void async_timer_set_data(async_timer_t *__FAR timer, void *__FAR dat) {
    timer->dat = dat;
}

void *__FAR async_timer_get_data(async_timer_t *__FAR timer) {
    return timer->dat;
}

async_looper_t *__FAR async_timer_get_looper(async_timer_t *__FAR timer) {
    return timer->looper;
}

