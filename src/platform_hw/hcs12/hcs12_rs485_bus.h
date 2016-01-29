#ifndef __HCS12_RS485_BUS_H__
#define __HCS12_RS485_BUS_H__

#include "stdint.h"

typedef struct hcs12_rs485_bus *hcs12_rs485_bus_t;

uint8_t hcs12_rs485_bus_init(hcs12_rs485_bus_t bus, void *dat, void (*on_recv_byte)(void *dat, uint8_t));
uint8_t hcs12_rs485_bus_set_baud(hcs12_rs485_bus_t bus, uint16_t baud);
uint8_t hcs12_rs485_bus_tx_byte(struct hcs12_rs485_bus *bus, uint8_t b);

extern const hcs12_rs485_bus_t hcs12_rs485_bus_0;
extern const hcs12_rs485_bus_t hcs12_rs485_bus_1;
extern const hcs12_rs485_bus_t hcs12_rs485_bus_2;
extern const hcs12_rs485_bus_t hcs12_rs485_bus_3;

#endif
