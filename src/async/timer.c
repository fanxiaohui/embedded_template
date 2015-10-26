#include "./private.h"
#include <list/list.h>

static LIST_HEAD(free_list);
static LIST_HEAD(used_list);

struct async_timer {
    // 链表操作, 这个必须是第一个成员.
    struct list_head head;
    // 剩余时间.
    async_timeout_t timeout;
    // 执行的函数
    async_timer_handler_t func;
    // 用户附加数据.
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

async_timer_t *__FAR async_timer_register(async_timer_handler_t func, async_timeout_t timeout, void *__FAR dat) {
    async_timer_t *__FAR ret = ASYNC_TIMER_ERROR;

    __lock();
    if (!list_empty(&free_list)) {
        ret = (async_timer_t *__FAR)(free_list.next);
        ret->func = func;
        ret->dat = dat;
        ret->timeout = timeout;
        list_del(&ret->head);
    }
    __unlock();

    {
        struct async_sem_private priv;
        priv.type = PRIVATE_SEM_TYPE_ADD_TIMER;
        priv.data.timer = ret;
        if (!async_notify_loop(&priv)) {
            __lock();
            list_add(&ret->head, &free_list);
            __unlock();
            ret = ASYNC_TIMER_ERROR;
        }
    }

    return ret;
}

async_timeout_t async_timer_exec(async_timeout_t escaped) {
    struct list_head *__FAR pos;
    struct list_head *__FAR n;

    async_timeout_t mini_timeout = -1;

    list_for_each_safe(pos, n, &used_list) {
        struct async_timer *__FAR timer = (struct async_timer * __FAR)pos;
        if (timer->timeout > escaped) {
            timer->timeout -= escaped;
        } else {
            timer->timeout = timer->func(timer);
            if (timer->timeout == 0) {
                __lock();
                list_move(pos, &free_list);
                __unlock();
            }
        }

        if (mini_timeout > timer->timeout) {
            mini_timeout = timer->timeout;
        }
    }
    return mini_timeout;
}

async_timeout_t async_timer_add_timer(async_timer_t *__FAR timer, async_timeout_t escaped) {
    async_timeout_t mini_timeout = async_timer_exec(escaped);
    list_add(&(timer->head), &used_list);

    if (mini_timeout < timer->timeout) return mini_timeout;
    if (timer->timeout == 0) return 1;
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

void async_timer_cancel(async_timer_t *__FAR timer) {
    __lock();
    list_del(&timer->head);
    __unlock();
}

