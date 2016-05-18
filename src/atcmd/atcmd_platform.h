#ifndef __ATCMD_PLATFORM_H__
#define __ATCMD_PLATFORM_H__

#include "atcmd.h"
#include "ringbuffer.h"
#include "platform_os.h"

struct at_QIRD_recv_info {
    uint8_t stage;
    uint16_t datlen;
    uint8_t *buffer;
    uint16_t recvlen;
};

struct atcmd {
    uint8_t auto_prefix[10];
    uint8_t auto_prefix_index;

    uint16_t ipd_length;
    uint16_t ipd_recv_index;
    struct ringbuffer ipd_rb;

    struct ringbuffer rb;
    union {
        struct at_QIRD_recv_info QIRD;
    } serial_recv_info;

    os_sem_t sem;

    os_time_t time_to_default;
    void (*serial_send)(const uint8_t *dat, uint16_t len);
    void (*serial_recv)(struct atcmd *atcmd, uint8_t b);
};

#endif
