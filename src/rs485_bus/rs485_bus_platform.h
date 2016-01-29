#ifndef __RS485_BUS_PLATFOMR_H__
#define __RS485_BUS_PLATFOMR_H__

#include "rs485_bus.h"

struct rs485_bus_ops {
    uint8_t (*init)(void *private_data, void *bus, void (*on_recv_byte)(rs485_bus_t, uint8_t));
    uint8_t (*set_baud)(void *private_data, uint16_t baud);
    uint8_t (*tx_byte)(void *private_data, uint8_t byte);
};

struct rs485_bus {
    void *private_data;
    const struct rs485_bus_ops *ops;
    uint16_t baud;
};

#endif
