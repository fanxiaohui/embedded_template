#ifndef __PLATFORM_POSIX_H__
#define __PLATFORM_POSIX_H__

#include <stdint.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef sem_t *os_sem_t;
typedef pthread_mutex_t *os_mutex_t;
typedef uint32_t os_time_t;
#define OS_TIME_FOREVER ((os_time_t)0xFFFFFFFF)

#define ASYNC_TIME_FOREVER ((os_time_t)(0xFFFFFFFF))

extern struct timespec otime;
extern os_mutex_t critical_mutex;

static inline os_sem_t os_create_sem(void) {
    static sem_t sem[100];
    static char used[100];

    int i;
    for (i = 0; i < sizeof(sem)/sizeof(sem[0]); ++i) {
        if (!used[i]) {
            used[i] = 1;
            sem_init(&sem[i], 0, 0);
            return &sem[i];
        }
    }
    return NULL;
}

static inline char os_post_sem(os_sem_t sem) {
    return 0 == sem_post(sem);
}

static inline char os_pend_sem(os_sem_t sem, os_time_t timeout) {
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


static inline void os_clear_sem(os_sem_t sem) {
    sem_init(sem, 0, 0);
}


static inline os_mutex_t os_create_mutex(void) {
    static pthread_mutex_t mutex[100];
    static char used[100];
    int i;

    for (i = 0; i < sizeof(mutex)/ sizeof(mutex[0]); ++i) {
        if (!used[i]) {
            used[i] = 1;
            mutex[i] = PTHREAD_MUTEX_INITIALIZER;
            return &mutex[i];
        }
    }
    return NULL;
}

static inline void os_lock_mutex(os_mutex_t mutex) {
    pthread_mutex_lock(mutex);
}

static inline void os_unlock_mutex(os_mutex_t mutex) {
    pthread_mutex_unlock(mutex);
}
#if defined(__WIN32__)
#include "Windows.h"
#endif
static inline void os_sleep(os_time_t t) {
#if defined(__WIN32__)
    Sleep(t);
#else
    sleep(t);
#endif
}

static inline os_time_t os_get_time(void) {
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

static inline void os_destroy_sem(os_sem_t sem) {
}

static inline void os_destroy_mutex(os_mutex_t mutex) {
}


static inline void os_platform_init(void) {
    clock_gettime(CLOCK_REALTIME, &otime);
    critical_mutex = os_create_mutex();
}


#define OS_CRITICAL(block) do { \
    os_lock_mutex(critical_mutex); \
    do { block } while(0); \
    os_unlock_mutex(critical_mutex); \
} while(0)


#endif
