/// \file event.h
/// \brief 异步事件调用声明.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_EVENT_H__
#define __ASYNC_EVENT_H__

#include "./looper.h"

/// 异步事件调用数据类型.
typedef struct async_event *__FAR async_event_t;


/// \brief 异步事件执行的函数类型.
/// return ==0 取消当前异步事件回调.
/// return !=0 继续等待事件.
typedef char (*async_event_callback_t)(async_event_t event);


#define ASYNC_EVENT_REGISTER_ERROR ((async_event_t)0)

/// \brief async_event_register 注册一个异步事件执行回调.
///
/// \param looper 注册到这个LOOPER中执行.
/// \param cb 事件发生时执行的回调函数.
/// \param dat 执行回调函数式传入的数据指针.
///
/// \return 异步调用数据类型, 这个值用于后面的trigger来触发这个时间的执行.
#if ASYNC_LOOPER_SIZE>1
async_event_t async_event_register(async_looper_t looper, async_event_callback_t cb, async_timeout_t timeout, void *__FAR dat);
#else
async_event_t async_event_register(async_event_callback_t cb, async_timeout_t timeout, void *__FAR dat);
#endif


/// \brief async_event_trigger 触发一个异步事件的执行.
///
/// \param event 需要触发的事件, 这个值是在注册时候返回的.
///
/// \return ==0 触发失败.
/// \return !=0 触发成功.
char async_event_trigger(async_event_t event);

/// \brief async_event_set_callback 设置定时器调用的函数.
///
/// \param event 定时器.
/// \param cb 回调函数.
void async_event_set_callback(async_event_t event, async_event_callback_t cb);

/// \brief async_event_set_data 设置定时器的用户数据.
///
/// \param event 定时器.
/// \param dat 用户数据.
void async_event_set_data(async_event_t event, void *__FAR dat);

/// \brief async_event_get_data 获取定时器的用户数据.
///
/// \param event 定时器.
///
/// \return 用户数据.
void *__FAR async_event_get_data(async_event_t event);

/// \brief async_event_cancel 删除一个定时器.
///
/// \param event 定时器.
void async_event_cancel(async_event_t event);

#endif

