/// \file async.c
/// \brief ASYNC��ʵ��.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#include "./private.h"


async_mutex_t async_g_lock;

void async_init(void) {
    async_g_lock = async_create_mutex();
    async_platform_init();
    async_looper_init();
    async_event_init();
}

