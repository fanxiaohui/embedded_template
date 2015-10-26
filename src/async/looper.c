#include "./private.h"
#include <ringbuffer/ringbuffer.h>


RINGBUFFER_DEFINE_STATIC(sem_data_buffer, 100);

char async_notify_loop(struct async_sem_private *priv) {
    RINGBUFFER_SIZE_TYPE ret;

    __lock();
    ret = ringbuffer_write(&sem_data_buffer, (unsigned char *__FAR)priv, sizeof(*priv));
    __unlock();

    if (ret != sizeof(*priv)) {
        return 0;
    }

    return async_post_sem(sem);
}

void async_looper_loop(void *nouse) {
    unsigned char tmp;
    async_time_t pre_time;
    unsigned short escaped;
    async_timeout_t wait_time = -1;

    (void) nouse;

    while (1) {
        pre_time = async_get_time();
        tmp = async_pend_sem(sem, wait_time);
        if (!tmp) {
            goto __update_wait_time;
        } else {
            struct async_sem_private priv;
            __lock();
            tmp = ringbuffer_read(&sem_data_buffer, (unsigned char *__FAR)&priv, sizeof(priv));
            __unlock();

            if (tmp != sizeof(priv)) {
                __lock();
                ringbuffer_clear(&sem_data_buffer);
                async_clear_sem(sem);
                __unlock();
                goto __update_wait_time;
            }

            if (priv.type == PRIVATE_SEM_TYPE_TRIGGER_EVENT_CALL) {
                async_event_call_exec(priv.data.event_call);
                goto __update_wait_time;
            } else if (priv.type == PRIVATE_SEM_TYPE_ADD_TIMER) {
                escaped = async_get_time() - pre_time;
                wait_time = async_timer_add_timer(priv.data.timer, escaped);
                continue;
            } else if (priv.type == PRIVATE_SEM_TYPE_EXIT) {
                return;
            }

        }
__update_wait_time:
        escaped = async_get_time() - pre_time;
        if (wait_time <= escaped) {
            wait_time = async_timer_exec(escaped);
        } else {
            wait_time -= escaped;
        }

    }
}

