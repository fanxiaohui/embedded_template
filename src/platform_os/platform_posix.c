#include "platform_posix.h"

struct timespec otime;
os_mutex_t critical_mutex;

void os_platform_init(void) {
    clock_gettime(CLOCK_REALTIME, &otime);
    critical_mutex = os_create_mutex();
}

static sem_t sem_pool[100];
static char sem_use_flag[100];

os_sem_t os_create_sem(void) {

    int i;
    for (i = 0; i < sizeof(sem_pool)/sizeof(sem_pool[0]); ++i) {
        if (!sem_use_flag[i]) {
            sem_use_flag[i] = 1;
            sem_init(&sem_pool[i], 0, 0);
            return &sem_pool[i];
        }
    }
    return NULL;
}

void os_destroy_sem(os_sem_t sem) {
    int i;
    for (i = 0; i < sizeof(sem_pool)/sizeof(sem_pool[0]); ++i) {
        if (&sem_pool[i] == sem) {
            sem_use_flag[i] = 0;
            return;
        }
    }
}

