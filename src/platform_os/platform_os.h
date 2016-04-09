/// \file platform.h
/// \brief 异步框架的平台相关的声明.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_PLATFORM_H__
#define __ASYNC_PLATFORM_H__

#if defined(__FREERTOS__)
#include "platform_freertos.h"
#endif

#if defined(__linux__) || defined(__WIN32__)
#include "platform_posix.h"
#endif

#if defined(__UCOS__)
#include "platform_ucos.h"
#endif


#endif
