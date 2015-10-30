/// \file platform.h
/// \brief �첽��ܵ�ƽ̨��ص�����.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_PLATFORM_H__
#define __ASYNC_PLATFORM_H__

#if defined(__FREERTOS__)
#include "./platform_freertos.h"
#endif

#if defined(__linux__) || defined(__WIN32__)
#include "./platform_posix.h"
#endif

#ifndef __FAR
#define __FAR
#endif

#endif
