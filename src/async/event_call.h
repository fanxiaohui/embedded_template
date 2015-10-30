/// \file event_call.h
/// \brief �첽�¼���������.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_EVENT_CALL_H__
#define __ASYNC_EVENT_CALL_H__

#include "./looper.h"

/// �첽�¼�������������.
typedef struct async_event_call *__FAR async_event_call_t;


/// \brief �첽�¼�ִ�еĺ�������.
/// return ==0 ȡ����ǰ�첽�¼��ص�.
/// return !=0 �����ȴ��¼�.
typedef char (*async_event_callback_t)(void *__FAR dat);


#define ASYNC_EVENT_CALL_REGISTER_ERROR ((async_event_call_t)0)

/// \brief async_event_call_register ע��һ���첽�¼�ִ�лص�.
///
/// \param looper ע�ᵽ���LOOPER��ִ��.
/// \param cb �¼�����ʱִ�еĻص�����.
/// \param dat ִ�лص�����ʽ���������ָ��.
///
/// \return �첽������������, ���ֵ���ں����trigger���������ʱ���ִ��.
async_event_call_t async_event_call_register(async_looper_t looper, async_event_callback_t cb, void *__FAR dat);

/// \brief async_event_call_trigger ����һ���첽�¼���ִ��.
///
/// \param event_call ��Ҫ�������¼�, ���ֵ����ע��ʱ�򷵻ص�.
///
/// \return ==0 ����ʧ��.
/// \return !=0 �����ɹ�.
char async_event_call_trigger(async_event_call_t event_call);

#endif

