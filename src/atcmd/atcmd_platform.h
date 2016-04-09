#ifndef __ATCMD_PLATFORM_H__
#define __ATCMD_PLATFORM_H__

#include "atcmd.h"
#include "ringbuffer.h"
#include "platform_os.h"

struct atcmd {
    const struct atcmd_interface *iface;
    struct ringbuffer rb;
    os_sem_t sem;
};

#endif