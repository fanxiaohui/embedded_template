/// \file timer.h
/// \brief �첽��ʱ������.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_TIMER_H__
#define __ASYNC_TIMER_H__

#include "./looper.h"

/// �첽��ʱ����������.
typedef struct async_timer *__FAR async_timer_t;

#define ASYNC_TIMER_REGISTER_ERROR ((async_timer_t)0)

/// \brief async_timer_callback_t timer�Ļص�����.
/// \param timer: ��Ӧ��timer.
/// \return ==0: ȡ����ǰtimer.
/// \return !=0: �´ε��õ���ʱ;
typedef async_timeout_t (*async_timer_callback_t)(async_timer_t timer);

/// \brief async_timer_register ע��һ����ʱ��.
///
/// \param looper ע�ᶨʱ�������LOOPERִ��.
/// \param cb ʱ�䵽ʱ��Ҫִ�еĺ���.
/// \param timeout ��ʱʱ��.
/// \param dat �û�����, ���ֵ������async_timer_get_data����, Ҳ����ͨ��async_timer_set_data������.
///
/// \return ��ʱ��.
async_timer_t async_timer_register(async_looper_t looper, async_timer_callback_t cb, async_timeout_t timeout, void *__FAR dat);

/// \brief async_timer_set_callback ���ö�ʱ�����õĺ���.
///
/// \param timer ��ʱ��.
/// \param cb �ص�����.
void async_timer_set_callback(async_timer_t timer, async_timer_callback_t cb);

/// \brief async_timer_set_data ���ö�ʱ�����û�����.
///
/// \param timer ��ʱ��.
/// \param dat �û�����.
void async_timer_set_data(async_timer_t timer, void *__FAR dat);

/// \brief async_timer_get_data ��ȡ��ʱ�����û�����.
///
/// \param timer ��ʱ��.
///
/// \return �û�����.
void *__FAR async_timer_get_data(async_timer_t timer);

/// \brief async_timer_cancel ɾ��һ����ʱ��.
///
/// \param timer ��ʱ��.
void async_timer_cancel(async_timer_t timer);

/// \brief async_timer_get_looper ��ȡasync_timer_registerע�����ڵ�LOOPER
///
/// \param timer ��ʱ��.
///
/// \return LOOPER
async_looper_t async_timer_get_looper(async_timer_t timer);

#endif

