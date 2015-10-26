#include "./private.h"

async_sem_t sem;
async_mutex_t lock;

void async_init(void) {
    sem = async_create_sem();
    lock = async_create_mutex();
    async_platform_init();
    async_timer_init();
    async_event_call_init();
}

char async_exit(void) {
    struct async_sem_private priv;
    priv.type = PRIVATE_SEM_TYPE_EXIT;
    return async_notify_loop(&priv);
}


