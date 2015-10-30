/// \file event_call.h
/// \brief 异步事件调用声明.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_EVENT_CALL_H__
#define __ASYNC_EVENT_CALL_H__

#include "./looper.h"

/// 异步事件调用数据类型.
typedef struct async_event_call *__FAR async_event_call_t;


/// \brief 异步事件执行的函数类型.
/// return ==0 取消当前异步事件回调.
/// return !=0 继续等待事件.
typedef char (*async_event_callback_t)(void *__FAR dat);


#define ASYNC_EVENT_CALL_REGISTER_ERROR ((async_event_call_t)0)

/// \brief async_event_call_register 注册一个异步事件执行回调.
///
/// \param looper 注册到这个LOOPER中执行.
/// \param cb 事件发生时执行的回调函数.
/// \param dat 执行回调函数式传入的数据指针.
///
/// \return 异步调用数据类型, 这个值用于后面的trigger来触发这个时间的执行.
async_event_call_t async_event_call_register(async_looper_t looper, async_event_callback_t cb, void *__FAR dat);

/// \brief async_event_call_trigger 触发一个异步事件的执行.
///
/// \param event_call 需要触发的事件, 这个值是在注册时候返回的.
///
/// \return ==0 触发失败.
/// \return !=0 触发成功.
char async_event_call_trigger(async_event_call_t event_call);

#endif

