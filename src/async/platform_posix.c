#include "platform.h"
#include <math.h>
#include <time.h>

struct timespec otime;
void async_platform_init(void) {
    clock_gettime(CLOCK_REALTIME, &otime);
}

async_sem_t async_create_sem(void) {
    static sem_t sem;
    sem_init(&sem, 0, 0);
    return &sem;
}

char async_post_sem(async_sem_t sem) {
    return 0 == sem_post(sem);
}

char async_pend_sem(async_sem_t sem, async_timeout_t timeout) {
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


void async_clear_sem(async_sem_t sem) {
    sem_init(sem, 0, 0);
}


async_mutex_t async_create_mutex(void) {
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    return &mutex;
}

void async_lock_mutex(async_mutex_t mutex) {
    pthread_mutex_lock(mutex);
}

void async_unlock_mutex(async_mutex_t mutex) {
    pthread_mutex_unlock(mutex);
}

async_time_t async_get_time(void) {
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

