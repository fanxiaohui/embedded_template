#ifndef __PLATFORM_POSIX_H__
#define __PLATFORM_POSIX_H__

#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>

typedef sem_t *async_sem_t;
typedef uint32_t async_timeout_t;
typedef pthread_mutex_t *async_mutex_t;
typedef uint32_t async_time_t;

#define ASYNC_TIMEOUT_FOREVER ((async_timeout_t)(0xFFFFFFFF))


#endif
