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
typedef uint32_t async_timeout_t;
typedef uint32_t async_time_t;

#define ASYNC_TIMEOUT_FOREVER ((async_timeout_t)0xFFFFFFFFUL)

/// \brief async_assert_info 断言输出.
///
/// \param file 源文件路径.
/// \param line 行号.
/// \param fmt 信息.
void async_assert_info(const char *file, int line, const char *fmt, ...);

//void async_platform_init(void);
#define async_platform_init() do { ; } while(0)

/// \brief async_create_sem 创建一个信号量.
///
/// \return 创建的信号量.
//async_sem_t async_create_sem(void);
static inline async_sem_t async_create_sem(void) {
    return xSemaphoreCreateCounting(1000, 0);
}

static inline void async_destroy_sem(async_sem_t sem) {
    vSemaphoreDelete(sem);
}

/// \brief async_post_sem 发送一个信号量.
///
/// \param sem 信号量.
///
/// \return ==0 发送失败.
/// \return !=0 发送成功.
//char async_post_sem(async_sem_t sem);
static inline char async_post_sem(async_sem_t sem) {
    return pdFALSE != xSemaphoreGive(sem);
}

/// \brief async_pend_sem 等待一个信号量.
///
/// \param sem 信号量.
/// \param timeout 超时时间, 单位ms.
///
/// \return !=0 等待成功.
/// \return ==0 等待超时.
//char async_pend_sem(async_sem_t sem, async_timeout_t timeout);
static inline char async_pend_sem(async_sem_t sem, async_timeout_t timeout) {
    return pdFALSE != xSemaphoreTake(sem, timeout * configTICK_RATE_HZ / 1000);
}

/// \brief async_clear_sem 清空信号量.
///
/// \param sem 信号量.
//void async_clear_sem(async_sem_t sem);
static inline void async_clear_sem(async_sem_t sem) {
    while (pdTRUE == xSemaphoreTake(sem, 0));
}

/// \brief async_create_mutex 创建一个互斥锁.
///
/// \return 互斥锁.
//async_mutex_t async_create_mutex(void);
static inline async_mutex_t async_create_mutex(void) {
    return xSemaphoreCreateRecursiveMutex();
}

static inline void async_destroy_mutex(async_mutex_t mutex) {
    vSemaphoreDelete(mutex);
}


/// \brief async_lock_mutex 加锁互斥锁.
///
/// \param mutex 互斥锁.
//void async_lock_mutex(async_mutex_t mutex);
static inline void async_lock_mutex(async_mutex_t mutex) {
    xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
}

/// \brief async_unlock_mutex 解锁互斥锁.
///
/// \param mutex 互斥锁.
//void async_unlock_mutex(async_mutex_t mutex);
static inline void async_unlock_mutex(async_mutex_t mutex) {
    xSemaphoreGiveRecursive(mutex);
}

/// \brief async_get_time 获取系统时间.
///
/// \return 系统时间.
/// \note 这个值得绝对值无所谓, 但是以ms为单位递增.
//async_time_t async_get_time(void);
static async_time_t async_get_time(void) {
    return xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
}

#endif

