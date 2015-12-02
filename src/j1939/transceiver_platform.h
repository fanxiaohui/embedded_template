#ifndef __J1939_TRANSCEIVER_PLATFORM_H__
#define __J1939_TRANSCEIVER_PLATFORM_H__

#include "transceiver.h"


struct can_transceiver_operations {
    uint8_t (*init)(void *__FAR private_data, uint32_t baudrate);
    uint8_t (*recv)(void *__FAR private_data, struct can_extended_frame *__FAR frame);
    uint8_t (*send)(void *__FAR private_data, const struct can_extended_frame *__FAR frame);
    uint8_t (*set_recv_filter)(void *__FAR private_data, uint32_t filter, uint32_t mask);
};

struct can_transceiver {
    void *private_data;
    const struct can_transceiver_operations *ops;
};

#endif

