#ifndef __ASYNC_PLATFORM_FREERTOS_H__
#define __ASYNC_PLATFORM_FREERTOS_H__

#include <stdint.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

typedef SemaphoreHandle_t *async_sem_t;
typedef QueueHandle_t async_queue_t;
typedef SemaphoreHandle_t *async_mutex_t;
typedef uint32_t async_time_t;

#define ASYNC_TIME_FOREVER ((async_time_t)0xFFFFFFFFUL)

/// \brief async_assert_info �������.
///
/// \param file Դ�ļ�·��.
/// \param line �к�.
/// \param fmt ��Ϣ.
void async_assert_info(const char *file, int line, const char *fmt, ...);

//void async_platform_init(void);
#define async_platform_init() do { ; } while(0)

/// \brief async_create_sem ����һ���ź���.
///
/// \return �������ź���.
//async_sem_t async_create_sem(void);
static inline async_sem_t async_create_sem(void) {
    return xSemaphoreCreateCounting(1000, 0);
}

static inline void async_destroy_sem(async_sem_t sem) {
    vSemaphoreDelete(sem);
}

/// \brief async_post_sem ����һ���ź���.
///
/// \param sem �ź���.
///
/// \return ==0 ����ʧ��.
/// \return !=0 ���ͳɹ�.
//char async_post_sem(async_sem_t sem);
static inline char async_post_sem(async_sem_t sem) {
    return pdFALSE != xSemaphoreGive(sem);
}

/// \brief async_pend_sem �ȴ�һ���ź���.
///
/// \param sem �ź���.
/// \param timeout ��ʱʱ��, ��λms.
///
/// \return !=0 �ȴ��ɹ�.
/// \return ==0 �ȴ���ʱ.
//char async_pend_sem(async_sem_t sem, async_time_t timeout);
static inline char async_pend_sem(async_sem_t sem, async_time_t timeout) {
    return pdFALSE != xSemaphoreTake(sem, timeout * configTICK_RATE_HZ / 1000);
}

/// \brief async_clear_sem ����ź���.
///
/// \param sem �ź���.
//void async_clear_sem(async_sem_t sem);
static inline void async_clear_sem(async_sem_t sem) {
    while (pdTRUE == xSemaphoreTake(sem, 0));
}

/// \brief async_create_mutex ����һ��������.
///
/// \return ������.
//async_mutex_t async_create_mutex(void);
static inline async_mutex_t async_create_mutex(void) {
    return xSemaphoreCreateRecursiveMutex();
}

static inline void async_destroy_mutex(async_mutex_t mutex) {
    vSemaphoreDelete(mutex);
}


/// \brief async_lock_mutex ����������.
///
/// \param mutex ������.
//void async_lock_mutex(async_mutex_t mutex);
static inline void async_lock_mutex(async_mutex_t mutex) {
    xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
}

/// \brief async_unlock_mutex ����������.
///
/// \param mutex ������.
//void async_unlock_mutex(async_mutex_t mutex);
static inline void async_unlock_mutex(async_mutex_t mutex) {
    xSemaphoreGiveRecursive(mutex);
}

/// \brief async_get_time ��ȡϵͳʱ��.
///
/// \return ϵͳʱ��.
/// \note ���ֵ�þ���ֵ����ν, ������msΪ��λ����.
//async_time_t async_get_time(void);
static async_time_t async_get_time(void) {
    return xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
}

#endif

