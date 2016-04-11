#ifndef __ATCMD_PLATFORM_H__
#define __ATCMD_PLATFORM_H__

#include "atcmd.h"
#include "ringbuffer.h"
#include "platform_os.h"

struct atcmd {
    void (*send)(const uint8_t *dat, uint16_t size);
    struct ringbuffer rb;
    os_sem_t sem;
};

#endif