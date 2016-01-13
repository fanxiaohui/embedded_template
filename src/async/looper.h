/// \file looper.h
/// \brief �첽ѭ������.
/// \author Xi Qingping
/// \version
/// \date 2015-10-29

#ifndef __ASYNC_LOOPER_H__
#define __ASYNC_LOOPER_H__

#include "./platform.h"


#ifndef ASYNC_LOOPER_SIZE
#define ASYNC_LOOPER_SIZE 1
#endif

/// �첽ѭ������������.
typedef struct async_looper *__FAR async_looper_t;

#define ASYNC_LOOPER_CREATE_ERROR ((async_looper_t)0)

/// \brief async_looper_create ����һ���첽ѭ��.
///
/// \return �첽ѭ����������, ��������첽ѭ���Ĳ�������Ҫʹ���������.
async_looper_t async_looper_create(void);
/// \brief async_looper_loop ִ���첽ѭ��, ���������async_looper_exit֮ǰ���᷵��.
///
/// \param looper ��Ҫִ�е��첽ѭ��.
void async_looper_loop(async_looper_t looper);

/// \brief async_looper_exit �˳��첽ѭ��.
///
/// \param looper ��Ҫ�˳����첽ѭ��.
///
/// \return 0 �˳�ʧ��.
/// \return !=0 �˳��ɹ�.
char async_looper_exit(async_looper_t looper);

#endif

