/// \file platform.h
/// \brief 异步框架的平台相关的声明.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_PLATFORM_H__
#define __ASYNC_PLATFORM_H__

#if defined(__FREERTOS__)
#include "./platform_freertos.h"
#endif

#if defined(__linux__) || defined(__WIN32__)
#include "./platform_posix.h"
#endif

/*
/// \brief async_assert_info 断言输出.
///
/// \param file 源文件路径.
/// \param line 行号.
/// \param msg 信息.
void async_assert_info(const char *file, int line, const char *msg);

/// \brief async_platform_init 平台相关的运行环境初始化.
void async_platform_init(void);

/// \brief async_create_sem 创建一个信号量.
///
/// \return 创建的信号量.
async_sem_t async_create_sem(void);

/// \brief async_post_sem 发送一个信号量.
///
/// \param sem 信号量.
///
/// \return ==0 发送失败.
/// \return !=0 发送成功.
char async_post_sem(async_sem_t sem);

/// \brief async_pend_sem 等待一个信号量.
///
/// \param sem 信号量.
/// \param timeout 超时时间, 单位ms.
///
/// \return !=0 等待成功.
/// \return ==0 等待超时.
char async_pend_sem(async_sem_t sem, async_timeout_t timeout);

/// \brief async_clear_sem 清空信号量.
///
/// \param sem 信号量.
void async_clear_sem(async_sem_t sem);

/// \brief async_create_mutex 创建一个互斥锁.
///
/// \return 互斥锁.
async_mutex_t async_create_mutex(void);

/// \brief async_lock_mutex 加锁互斥锁.
///
/// \param mutex 互斥锁.
void async_lock_mutex(async_mutex_t mutex);

/// \brief async_unlock_mutex 解锁互斥锁.
///
/// \param mutex 互斥锁.
void async_unlock_mutex(async_mutex_t mutex);

/// \brief async_get_time 获取系统时间.
///
/// \return 系统时间.
/// \note 这个值得绝对值无所谓, 但是以ms为单位递增.
async_time_t async_get_time(void);
*/


#ifndef ASYNC_LOOPER_SIZE
#define ASYNC_LOOPER_SIZE 20
#endif

#ifndef ASYNC_EVENT_SIZE
#define ASYNC_EVENT_SIZE 20
#endif

#ifndef __FAR
#define __FAR
#endif

#endif
