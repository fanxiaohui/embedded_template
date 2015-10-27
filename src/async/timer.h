#ifndef __ASYNC_TIMER_H__
#define __ASYNC_TIMER_H__

#include "./platform.h"

typedef struct async_timer async_timer_t;

#define ASYNC_TIMER_CANCEL 0
#define ASYNC_TIMER_ERROR ((async_timer_t *__FAR)0)

/// \brief timer_handler timer�Ĵ�����.
/// \param timer: ��Ӧ��Timer����, ������
/// \return 0(ASYNC_TIMER_CANCEL): ȡ����ǰtimer;
/// \return !=0 �´ε��õ���ʱ;
typedef async_timeout_t (*async_timer_handler_t)(async_timer_t *__FAR timer);

/// ע��һ����ʱ��.
/// \return ASYNC_TIMER_ERROR: ע�ᶨʱ��ʧ��;
/// \return ����ֵ, ע�ᵽ�Ķ�ʱ��.
async_timer_t *__FAR async_timer_register(async_timer_handler_t func, async_timeout_t timeout, void *__FAR dat);

/// ���ö�ʱ�����õĺ���.
void async_timer_set_handler(async_timer_t *__FAR timer, async_timer_handler_t func);
/// ���ö�ʱ���ĸ�������.
void async_timer_set_data(async_timer_t *__FAR timer, void *__FAR dat);
/// �����������async_timer_registerʱ�����dat����.
void *__FAR async_timer_get_data(async_timer_t *__FAR timer);
/// ɾ��һ����ʱ��.
void async_timer_cancel(async_timer_t *__FAR timer);

#endif

