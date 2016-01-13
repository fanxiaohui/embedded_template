/// \file async.c
/// \brief ASYNCµÄÊµÏÖ.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#include "./private.h"

os_mutex_t async_g_lock;

void async_init(void) {
    if (async_g_lock == 0) {
        async_g_lock = os_create_mutex();
        os_platform_init();
        async_looper_init();
        async_event_init();
    }
}

