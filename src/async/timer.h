/// \file timer.h
/// \brief 异步定时器声明.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_TIMER_H__
#define __ASYNC_TIMER_H__

#include "./looper.h"

/// 异步定时器数据类型.
typedef struct async_timer *__FAR async_timer_t;

#define ASYNC_TIMER_REGISTER_ERROR ((async_timer_t)0)

/// \brief async_timer_callback_t timer的回调函数.
/// \param timer: 对应的timer.
/// \return ==0: 取消当前timer.
/// \return !=0: 下次调用的延时;
typedef async_timeout_t (*async_timer_callback_t)(async_timer_t timer);

/// \brief async_timer_register 注册一个定时器.
///
/// \param looper 注册定时器到这个LOOPER执行.
/// \param cb 时间到时需要执行的函数.
/// \param timeout 超时时间.
/// \param dat 用户数据, 这个值可以用async_timer_get_data返回, 也可以通过async_timer_set_data来设置.
///
/// \return 定时器.
async_timer_t async_timer_register(async_looper_t looper, async_timer_callback_t cb, async_timeout_t timeout, void *__FAR dat);

/// \brief async_timer_set_callback 设置定时器调用的函数.
///
/// \param timer 定时器.
/// \param cb 回调函数.
void async_timer_set_callback(async_timer_t timer, async_timer_callback_t cb);

/// \brief async_timer_set_data 设置定时器的用户数据.
///
/// \param timer 定时器.
/// \param dat 用户数据.
void async_timer_set_data(async_timer_t timer, void *__FAR dat);

/// \brief async_timer_get_data 获取定时器的用户数据.
///
/// \param timer 定时器.
///
/// \return 用户数据.
void *__FAR async_timer_get_data(async_timer_t timer);

/// \brief async_timer_cancel 删除一个定时器.
///
/// \param timer 定时器.
void async_timer_cancel(async_timer_t timer);

/// \brief async_timer_get_looper 获取async_timer_register注册所在的LOOPER
///
/// \param timer 定时器.
///
/// \return LOOPER
async_looper_t async_timer_get_looper(async_timer_t timer);

#endif

