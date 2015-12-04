#ifndef __ASYNC_PLATFORM_FREERTOS_H__
#define __ASYNC_PLATFORM_FREERTOS_H__

#include <stdint.h>
#include "ucos_ii.h"

typedef OS_EVENT *async_sem_t;
typedef OS_EVENT *async_queue_t;
typedef OS_EVENT *async_mutex_t;
typedef uint32_t async_time_t;

#define ASYNC_TIME_FOREVER ((async_time_t)0)

#ifndef ASYNC_LOOPER_SIZE
#define ASYNC_LOOPER_SIZE 1
#endif

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
    return OSSemCreate(0);
}

static inline void async_destroy_sem(async_sem_t sem) {
    INT8U err;
    (void)OSSemDel(sem, OS_DEL_ALWAYS, &err);
}

/// \brief async_post_sem 发送一个信号量.
///
/// \param sem 信号量.
///
/// \return ==0 发送失败.
/// \return !=0 发送成功.
//char async_post_sem(async_sem_t sem);
static inline char async_post_sem(async_sem_t sem) {
    return OS_ERR_NONE == OSSemPost(sem);
}

/// \brief async_pend_sem 等待一个信号量.
///
/// \param sem 信号量.
/// \param timeout 超时时间, 单位ms.
///
/// \return !=0 等待成功.
/// \return ==0 等待超时.
//char async_pend_sem(async_sem_t sem, async_time_t timeout);
static inline char async_pend_sem(async_sem_t sem, async_time_t timeout) {
    uint32_t ticks = (uint32_t)timeout * OS_TICKS_PER_SEC / 1000;
    INT8U err;
    
    while (ticks > 65535) {
        OSSemPend(sem, 65535, &err);
        if (OS_ERR_NONE == err) {
            return 1;
        }
        
        if (OS_ERR_TIMEOUT != err) { // wait error
            return 0;    
        }
        // wait timeout
        ticks -= 65535;
    }
    
    OSSemPend(sem, (INT16U)ticks, &err);    
    return OS_ERR_NONE == err;
}

/// \brief async_clear_sem 清空信号量.
///
/// \param sem 信号量.
//void async_clear_sem(async_sem_t sem);
static inline void async_clear_sem(async_sem_t sem) {
    INT8U err;
    OSSemSet(sem, 0, &err);
}

/// \brief async_create_mutex 创建一个互斥锁.
///
/// \return 互斥锁.
//async_mutex_t async_create_mutex(void);
static inline async_mutex_t async_create_mutex(void) {
    return OSSemCreate(1);
}

static inline void async_destroy_mutex(async_mutex_t mutex) {
    INT8U err;
    (void)OSSemDel(mutex, OS_DEL_ALWAYS, &err);
}


/// \brief async_lock_mutex 加锁互斥锁.
///
/// \param mutex 互斥锁.
//void async_lock_mutex(async_mutex_t mutex);
static inline void async_lock_mutex(async_mutex_t mutex) {
    INT8U err;
    
    for(;;) {
        OSSemPend(mutex, 65535, &err);
        if (OS_ERR_NONE == err) {
            return;
        }
        
        if (OS_ERR_TIMEOUT != err) { // wait error
            return;    
        }
    }
}

/// \brief async_unlock_mutex 解锁互斥锁.
///
/// \param mutex 互斥锁.
//void async_unlock_mutex(async_mutex_t mutex);
static inline void async_unlock_mutex(async_mutex_t mutex) {
    INT8U err;
    OSSemSet(mutex, 0, &err);
    OSSemPost(mutex);
}

/// \brief async_get_time 获取系统时间.
///
/// \return 系统时间.
/// \note 这个值得绝对值无所谓, 但是以ms为单位递增.
//async_time_t async_get_time(void);
static async_time_t async_get_time(void) {
    return OSTimeGet() * 1000 / OS_TICKS_PER_SEC;
}

#endif

