#ifndef __PLATFORM_POSIX_H__
#define __PLATFORM_POSIX_H__

#include <stdint.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef sem_t *async_sem_t;
typedef pthread_mutex_t *async_mutex_t;
typedef uint32_t async_time_t;

#define ASYNC_TIME_FOREVER ((async_time_t)(0xFFFFFFFF))

extern struct timespec otime;

static inline void async_platform_init(void) {
    clock_gettime(CLOCK_REALTIME, &otime);
}

static inline async_sem_t async_create_sem(void) {
    static sem_t sem;
    sem_init(&sem, 0, 0);
    return &sem;
}

static inline char async_post_sem(async_sem_t sem) {
    return 0 == sem_post(sem);
}

static inline char async_pend_sem(async_sem_t sem, async_time_t timeout) {
    struct timespec to;

    clock_gettime(CLOCK_REALTIME, &to);

    to.tv_nsec += (timeout % 1000) * 1e6;
    to.tv_sec += timeout / 1000;

    if (to.tv_nsec >= 1e9) {
        to.tv_nsec -= 1e9;
        to.tv_sec += 1;
    }

    return 0 == sem_timedwait(sem, &to);
}


static inline void async_clear_sem(async_sem_t sem) {
    sem_init(sem, 0, 0);
}


static inline async_mutex_t async_create_mutex(void) {
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    return &mutex;
}

static inline void async_lock_mutex(async_mutex_t mutex) {
    pthread_mutex_lock(mutex);
}

static inline void async_unlock_mutex(async_mutex_t mutex) {
    pthread_mutex_unlock(mutex);
}

static inline async_time_t async_get_time(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    time_t sec = spec.tv_sec - otime.tv_sec;
    long nsec = spec.tv_nsec - otime.tv_nsec;

    if (nsec < 0) {
        nsec += 1e9;
        sec -= 1;
    }

    long msec = round(spec.tv_nsec / 1.0e6);
    msec += sec * 1e3;

    return msec;
}

static inline void async_destroy_sem(async_sem_t sem) {
}


static inline void async_destroy_mutex(async_mutex_t mutex) {
}

#endif
