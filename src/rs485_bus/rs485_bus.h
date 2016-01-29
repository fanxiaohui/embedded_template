#ifndef __RS485_BUS_H__
#define __RS485_BUS_H__

#include "stdint.h"

typedef struct rs485_bus *rs485_bus_t;

inline uint8_t rs485_bus_init(rs485_bus_t bus, uint16_t baud, void *dat, void (*on_recv_byte)(void *, uint8_t));
inline uint8_t rs485_bus_tx_byte(rs485_bus_t bus, uint8_t b);
inline uint8_t rs485_bus_tx_string(rs485_bus_t bus, const uint8_t *dat);
inline uint8_t rs485_bus_tx_bytes(rs485_bus_t bus, const uint8_t *dat, uint8_t len);

#endif

