#ifndef __PLATFORM_POSIX_H__
#define __PLATFORM_POSIX_H__

#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>

typedef sem_t *async_sem_t;
typedef uint32_t async_timeout_t;
typedef pthread_mutex_t *async_mutex_t;
typedef uint32_t async_time_t;

/// \brief async_assert_info �������.
/// 
/// \param file Դ�ļ�·��.
/// \param line �к�.
/// \param msg ��Ϣ.
void async_assert_info(const char *file, int line, const char *msg);

/// \brief async_platform_init ƽ̨��ص����л�����ʼ��.
void async_platform_init(void);

/// \brief async_create_sem ����һ���ź���.
///
/// \return �������ź���.
async_sem_t async_create_sem(void);

/// \brief async_post_sem ����һ���ź���.
///
/// \param sem �ź���.
///
/// \return ==0 ����ʧ��.
/// \return !=0 ���ͳɹ�.
char async_post_sem(async_sem_t sem);

/// \brief async_pend_sem �ȴ�һ���ź���.
///
/// \param sem �ź���.
/// \param timeout ��ʱʱ��, ��λms.
///
/// \return !=0 �ȴ��ɹ�.
/// \return ==0 �ȴ���ʱ.
char async_pend_sem(async_sem_t sem, async_timeout_t timeout);

/// \brief async_clear_sem ����ź���.
///
/// \param sem �ź���.
void async_clear_sem(async_sem_t sem);

/// \brief async_create_mutex ����һ��������.
///
/// \return ������.
async_mutex_t async_create_mutex(void);

/// \brief async_lock_mutex ����������.
///
/// \param mutex ������.
void async_lock_mutex(async_mutex_t mutex);

/// \brief async_unlock_mutex ����������.
///
/// \param mutex ������.
void async_unlock_mutex(async_mutex_t mutex);

/// \brief async_get_time ��ȡϵͳʱ��.
///
/// \return ϵͳʱ��.
/// \note ���ֵ�þ���ֵ����ν, ������msΪ��λ����.
async_time_t async_get_time(void);

#endif
