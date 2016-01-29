#ifndef __HCS12_RS485_BUS_PLATFORM_H__
#define __HCS12_RS485_BUS_PLATFORM_H__

#include "rs485_bus_platform.h"
#include "hcs12_rs485_bus.h"

struct sci_regs {
    uint8_t bdh_asr1;
    uint8_t bdl_acr1;
    uint8_t cr1_acr2;
    uint8_t cr2;
    uint8_t sr1;
    uint8_t sr2;
    uint8_t drh;
    uint8_t drl;
};

struct hcs12_rs485_bus {
    struct sci_regs *regs;
    void *dat;
    void (*on_recv_byte)(void *dat, uint8_t b);
};

void hcs12_rs485_bus_isr(struct hcs12_rs485_bus * bus);

extern struct rs485_bus rs485_bus_0;
extern struct rs485_bus rs485_bus_1;
extern struct rs485_bus rs485_bus_2;
extern struct rs485_bus rs485_bus_3;

#endif
