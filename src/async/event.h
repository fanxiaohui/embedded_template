/// \file event.h
/// \brief �첽�¼���������.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_EVENT_H__
#define __ASYNC_EVENT_H__

#include "./looper.h"

/// �첽�¼�������������.
typedef struct async_event *__FAR async_event_t;


/// \brief �첽�¼�ִ�еĺ�������.
/// return ==0 ȡ����ǰ�첽�¼��ص�.
/// return !=0 �����ȴ��¼�.
typedef char (*async_event_callback_t)(async_event_t event);


#define ASYNC_EVENT_REGISTER_ERROR ((async_event_t)0)

/// \brief async_event_register ע��һ���첽�¼�ִ�лص�.
///
/// \param looper ע�ᵽ���LOOPER��ִ��.
/// \param cb �¼�����ʱִ�еĻص�����.
/// \param dat ���ûص�����ʽ������û�����ָ��.
///
/// \return �첽������������, ���ֵ���ں����trigger����������¼���ִ��.
#if ASYNC_LOOPER_SIZE>1
async_event_t async_event_register(async_looper_t looper, async_event_callback_t cb, async_timeout_t timeout, void *__FAR dat);
#else
async_event_t async_event_register(async_event_callback_t cb, async_timeout_t timeout, void *__FAR dat);
#endif


/// \brief async_event_trigger ����һ���첽�¼���ִ��.
///
/// \param event ��Ҫ�������¼�, ���ֵ����ע��ʱ�򷵻ص�.
///
/// \return ==0 ����ʧ��.
/// \return !=0 �����ɹ�.
char async_event_trigger(async_event_t event);

/// \brief async_event_set_callback �����¼��ĺ���.
///
/// \param event �¼�.
/// \param cb �ص�����.
void async_event_set_callback(async_event_t event, async_event_callback_t cb);

/// \brief async_event_set_data �����¼����û�����.
///
/// \param event �¼�.
/// \param dat �û�����.
void async_event_set_data(async_event_t event, void *__FAR dat);

/// \brief async_event_get_data ��ȡ�¼����û�����.
///
/// \param event �¼�.
///
/// \return �û�����.
void *__FAR async_event_get_data(async_event_t event);

/// \brief async_event_cancel ɾ��һ���¼�.
///
/// \param event �¼�.
/// \return ==0 ɾ��ʧ��.
/// \return !=0 ɾ���ɹ�.
char async_event_cancel(async_event_t event);

#endif

