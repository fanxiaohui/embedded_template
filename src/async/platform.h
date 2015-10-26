#ifndef __ASYNC_PLATFORM_H__
#define __ASYNC_PLATFORM_H__

#if defined(__linux__) || defined(__WIN32__)
#include "./platform_posix.h"
#endif


void async_platform_init(void);

async_sem_t async_create_sem(void);
char async_post_sem(async_sem_t sem);
char async_pend_sem(async_sem_t sem, async_timeout_t timeout);
void async_clear_sem(async_sem_t sem);

async_mutex_t async_create_mutex(void);
void async_lock_mutex(async_mutex_t mutex);
void async_unlock_mutex(async_mutex_t mutex);
async_time_t async_get_time(void);


#ifndef __FAR
#define __FAR
#endif

#endif
