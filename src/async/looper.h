/// \file looper.h
/// \brief 异步循环声明.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_LOOPER_H__
#define __ASYNC_LOOPER_H__

#include "./platform.h"


/// 异步循环的数据类型.
typedef struct async_looper *__FAR async_looper_t;

#define ASYNC_LOOPER_CREATE_ERROR ((async_looper_t)0)

/// \brief async_looper_create 创建一个异步循环.
///
/// \return 异步循环数据类型, 后面对于异步循环的操作都需要使用这个数据.
async_looper_t async_looper_create(void);
/// \brief async_looper_loop 执行异步循环, 这个函数在async_looper_exit之前不会返回.
///
/// \param looper 需要执行的异步循环.
#if ASYNC_LOOPER_SIZE>1
void async_looper_loop(async_looper_t looper);
#else
void async_looper_loop(void);
#endif

/// \brief async_looper_exit 退出异步循环.
///
/// \param looper 需要退出的异步循环.
///
/// \return 0 退出失败.
/// \return !=0 退出成功.
char async_looper_exit(async_looper_t looper);

#endif

