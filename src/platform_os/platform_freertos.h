#ifndef __PLATFORM_FREERTOS_H__
#define __PLATFORM_FREERTOS_H__

#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


typedef SemaphoreHandle_t os_sem_t;
typedef SemaphoreHandle_t os_mutex_t;
typedef uint32_t os_time_t;

#define OS_TIME_FOREVER ((os_time_t)0xFFFFFFFF)

void os_assert_info(const char *file, int line, const char *fmt, ...);

#define os_platform_init() do { ; } while(0)

static inline os_sem_t os_create_sem(void) {
    return xSemaphoreCreateCounting(65535, 0);
}

static inline void os_destroy_sem(os_sem_t sem) {
    vSemaphoreDelete(sem);
}

static inline char os_post_sem(os_sem_t sem) {
    return pdFALSE != xSemaphoreGive(sem);
}

static inline char os_pend_sem(os_sem_t sem, os_time_t timeout) {
    return pdFALSE != xSemaphoreTake(sem, timeout * configTICK_RATE_HZ / 1000);
}

static inline void os_clear_sem(os_sem_t sem) {
    while (pdTRUE == xSemaphoreTake(sem, 0));
}

static inline os_mutex_t os_create_mutex(void) {
    return xSemaphoreCreateRecursiveMutex();
}

static inline void os_destroy_mutex(os_mutex_t mutex) {
    vSemaphoreDelete(mutex);
}


static inline void os_lock_mutex(os_mutex_t mutex) {
    xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
}

static inline void os_unlock_mutex(os_mutex_t mutex) {
    xSemaphoreGiveRecursive(mutex);
}

static inline os_time_t os_get_time(void) {
    return xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
}

static inline void os_sleep(os_time_t ms) {
    vTaskDelay(ms * (1000 / configTICK_RATE_HZ));
}

#endif

