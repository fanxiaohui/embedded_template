#ifndef __PLATFORM_FREERTOS_H__
#define __PLATFORM_FREERTOS_H__

#include <stdint.h>
#include "ucos_ii.h"

typedef OS_EVENT *os_sem_t;
typedef OS_EVENT *os_queue_t;
typedef OS_EVENT *os_mutex_t;
typedef uint32_t os_time_t;

#define OS_TIME_FOREVER ((os_time_t)0)

/// \brief os_assert_info 断言输出.
///
/// \param file 源文件路径.
/// \param line 行号.
/// \param fmt 信息.
void os_assert_info(const char *file, int line, const char *fmt, ...);

//void os_platform_init(void);
#define os_platform_init() do { ; } while(0)

/// \brief os_create_sem 创建一个信号量.
///
/// \return 创建的信号量.
//os_sem_t os_create_sem(void);
static inline os_sem_t os_create_sem(void) {
    return OSSemCreate(0);
}

static inline void os_destroy_sem(os_sem_t sem) {
    INT8U err;
    (void)OSSemDel(sem, OS_DEL_ALWAYS, &err);
}

/// \brief os_post_sem 发送一个信号量.
///
/// \param sem 信号量.
///
/// \return ==0 发送失败.
/// \return !=0 发送成功.
//char os_post_sem(os_sem_t sem);
static inline char os_post_sem(os_sem_t sem) {
    return OS_ERR_NONE == OSSemPost(sem);
}

/// \brief os_pend_sem 等待一个信号量.
///
/// \param sem 信号量.
/// \param timeout 超时时间, 单位ms.
///
/// \return !=0 等待成功.
/// \return ==0 等待超时.
//char os_pend_sem(os_sem_t sem, os_time_t timeout);
static inline char os_pend_sem(os_sem_t sem, os_time_t timeout) {
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

/// \brief os_clear_sem 清空信号量.
///
/// \param sem 信号量.
//void os_clear_sem(os_sem_t sem);
static inline void os_clear_sem(os_sem_t sem) {
    INT8U err;
    OSSemSet(sem, 0, &err);
}

/// \brief os_create_mutex 创建一个互斥锁.
///
/// \return 互斥锁.
//os_mutex_t os_create_mutex(void);
static inline os_mutex_t os_create_mutex(void) {
    return OSSemCreate(1);
}

static inline void os_destroy_mutex(os_mutex_t mutex) {
    INT8U err;
    (void)OSSemDel(mutex, OS_DEL_ALWAYS, &err);
}


/// \brief os_lock_mutex 加锁互斥锁.
///
/// \param mutex 互斥锁.
//void os_lock_mutex(os_mutex_t mutex);
static inline void os_lock_mutex(os_mutex_t mutex) {
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

/// \brief os_unlock_mutex 解锁互斥锁.
///
/// \param mutex 互斥锁.
//void os_unlock_mutex(os_mutex_t mutex);
static inline void os_unlock_mutex(os_mutex_t mutex) {
    INT8U err;
    OSSemSet(mutex, 0, &err);
    OSSemPost(mutex);
}

/// \brief os_get_time 获取系统时间.
///
/// \return 系统时间.
/// \note 这个值得绝对值无所谓, 但是以ms为单位递增.
//os_time_t os_get_time(void);
static os_time_t os_get_time(void) {
    return OSTimeGet() * 1000 / OS_TICKS_PER_SEC;
}

#endif

