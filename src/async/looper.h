#ifndef __ASYNC_LOOPER_H__
#define __ASYNC_LOOPER_H__

#include "./platform.h"

typedef struct async_looper async_looper_t;

#define ASYNC_LOOPER_CREATE_ERROR ((async_looper_t *__FAR)0)

async_looper_t *__FAR async_looper_create(void);
void async_looper_loop(async_looper_t *__FAR looper);
char async_looper_exit(async_looper_t *__FAR looper);

#endif

