#ifndef __PLATFORM_FREERTOS_H__
#define __PLATFORM_FREERTOS_H__

#include "stdint.h"
#include "ucos_ii.h"

typedef OS_EVENT *os_sem_t;
typedef OS_EVENT *os_queue_t;
typedef OS_EVENT *os_mutex_t;
typedef uint32_t os_time_t;

#define OS_TIME_FOREVER ((os_time_t)0xFFFFFFFF)

/// \brief os_assert_info �������.
///
/// \param file Դ�ļ�·��.
/// \param line �к�.
/// \param fmt ��Ϣ.
void os_assert_info(const char *file, int line, const char *fmt, ...);

void os_platform_init(void);

/// \brief os_create_sem ����һ���ź���.
///
/// \return �������ź���.
//os_sem_t os_create_sem(void);
static inline os_sem_t os_create_sem(void) {
    return OSSemCreate(0);
}

static inline void os_destroy_sem(os_sem_t sem) {
    INT8U err;
    (void)OSSemDel(sem, OS_DEL_ALWAYS, &err);
}

/// \brief os_post_sem ����һ���ź���.
///
/// \param sem �ź���.
///
/// \return ==0 ����ʧ��.
/// \return !=0 ���ͳɹ�.
//char os_post_sem(os_sem_t sem);
static inline char os_post_sem(os_sem_t sem) {
    return OS_ERR_NONE == OSSemPost(sem);
}

/// \brief os_pend_sem �ȴ�һ���ź���.
///
/// \param sem �ź���.
/// \param timeout ��ʱʱ��, ��λms.
///
/// \return !=0 �ȴ��ɹ�.
/// \return ==0 �ȴ���ʱ.
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

/// \brief os_clear_sem ����ź���.
///
/// \param sem �ź���.
//void os_clear_sem(os_sem_t sem);
static inline void os_clear_sem(os_sem_t sem) {
    INT8U err;
    OSSemSet(sem, 0, &err);
}

/// \brief os_create_mutex ����һ��������.
///
/// \return ������.
//os_mutex_t os_create_mutex(void);
static inline os_mutex_t os_create_mutex(void) {
    return OSSemCreate(1);
}

static inline void os_destroy_mutex(os_mutex_t mutex) {
    INT8U err;
    (void)OSSemDel(mutex, OS_DEL_ALWAYS, &err);
}


/// \brief os_lock_mutex ����������.
///
/// \param mutex ������.
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

/// \brief os_unlock_mutex ����������.
///
/// \param mutex ������.
//void os_unlock_mutex(os_mutex_t mutex);
static inline void os_unlock_mutex(os_mutex_t mutex) {
    INT8U err;
    (void)OSSemSet(mutex, 0, &err);
    (void)OSSemPost(mutex);
}

/// \brief os_get_time ��ȡϵͳʱ��.
///
/// \return ϵͳʱ��.
/// \note ���ֵ�þ���ֵ����ν, ������msΪ��λ����.
//os_time_t os_get_time(void);
static inline os_time_t os_get_time(void) {
    return OSTimeGet() * (1000 / OS_TICKS_PER_SEC);
}

static inline void os_sleep(os_time_t ms) {
    os_time_t tick = ms * (1000 / OS_TICKS_PER_SEC);
    for (;;) {
        if (tick > 65535) {
            OSTimeDly(65535);
            tick -= 65535;
        } else if (tick != 0) {
            OSTimeDly((uint16_t)tick);
            return;
        } else {
            return;
        }
    }
}

#endif

