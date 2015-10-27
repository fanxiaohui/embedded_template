#ifndef __ASYNC_TIMER_H__
#define __ASYNC_TIMER_H__

#include "./platform.h"

typedef struct async_timer async_timer_t;

#define ASYNC_TIMER_CANCEL 0
#define ASYNC_TIMER_ERROR ((async_timer_t *__FAR)0)

/// \brief timer_handler timer的处理函数.
/// \param timer: 对应的Timer数据, 可以用
/// \return 0(ASYNC_TIMER_CANCEL): 取消当前timer;
/// \return !=0 下次调用的延时;
typedef async_timeout_t (*async_timer_handler_t)(async_timer_t *__FAR timer);

/// 注册一个定时器.
/// \return ASYNC_TIMER_ERROR: 注册定时器失败;
/// \return 其他值, 注册到的定时器.
async_timer_t *__FAR async_timer_register(async_timer_handler_t func, async_timeout_t timeout, void *__FAR dat);

/// 设置定时器调用的函数.
void async_timer_set_handler(async_timer_t *__FAR timer, async_timer_handler_t func);
/// 设置定时器的附加数据.
void async_timer_set_data(async_timer_t *__FAR timer, void *__FAR dat);
/// 这个函数返回async_timer_register时传入的dat参数.
void *__FAR async_timer_get_data(async_timer_t *__FAR timer);
/// 删除一个定时器.
void async_timer_cancel(async_timer_t *__FAR timer);

#endif

