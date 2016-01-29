#include "hcs12_rs485_bus_platform.h"
#include "hcs12_misc.h"
#include "derivative.h"

uint8_t hcs12_rs485_bus_init(struct hcs12_rs485_bus *bus, void *dat, void (*on_recv_byte)(void *dat, uint8_t)) {
    bus->dat = dat;
    bus->on_recv_byte = on_recv_byte;

    bus->regs->cr1_acr2 = 0x00;
    bus->regs->cr2 = (1<<3) | (1<<2) | (1<<5);
    
    return 1;
}

uint8_t hcs12_rs485_bus_set_baud(struct hcs12_rs485_bus *bus, uint16_t baud) {
    struct sci_regs *regs = bus->regs;
    uint32_t bus_clk = hcs12_get_bus_clock();
    uint16_t div = (uint16_t)((uint32_t)(bus_clk + (baud << 3)) / (baud << 4));
    regs->bdh_asr1 = div >> 8;
    regs->bdl_acr1 = (uint8_t)div;
    
    return 1;
}

uint8_t hcs12_rs485_bus_tx_byte(struct hcs12_rs485_bus *bus, uint8_t b) {
    struct sci_regs *regs = bus->regs;
    regs->drl = b;
    while ((regs->sr1 & (1<<7)) == 0);
    return 1;
}

void hcs12_rs485_bus_isr(struct hcs12_rs485_bus *bus) {
    struct sci_regs *regs = bus->regs;
    uint8_t status = regs->sr1;
    uint8_t d = regs->drl;
    if (status & (1<<5)) {
        if (bus->on_recv_byte) bus->on_recv_byte(bus->dat, d);
    }
}

static const struct rs485_bus_ops hcs12_rs485_bus_ops = {
    hcs12_rs485_bus_init,
    hcs12_rs485_bus_set_baud,
    hcs12_rs485_bus_tx_byte,
};

static struct hcs12_rs485_bus __hcs12_rs485_bus_0 = {
    (struct sci_regs *)&SCI0BDH,  
};

static struct hcs12_rs485_bus __hcs12_rs485_bus_1 = {
    (struct sci_regs *)&SCI1BDH,  
};

static struct hcs12_rs485_bus __hcs12_rs485_bus_2 = {
    (struct sci_regs *)&SCI2BDH,  
};

static struct hcs12_rs485_bus __hcs12_rs485_bus_3 = {
    (struct sci_regs *)&SCI3BDH,  
};

struct hcs12_rs485_bus *const hcs12_rs485_bus_0 = &__hcs12_rs485_bus_0;
struct hcs12_rs485_bus *const hcs12_rs485_bus_1 = &__hcs12_rs485_bus_1;
struct hcs12_rs485_bus *const hcs12_rs485_bus_2 = &__hcs12_rs485_bus_2;
struct hcs12_rs485_bus *const hcs12_rs485_bus_3 = &__hcs12_rs485_bus_3;

void SCI0_ISR_Handler(void) {
    //hcs12_rs485_bus_isr(hcs12_rs485_bus_0);
}

void SCI1_ISR_Handler(void) {
    //hcs12_rs485_bus_isr(hcs12_rs485_bus_1);
}

void SCI2_ISR_Handler(void) {
    //hcs12_rs485_bus_isr(hcs12_rs485_bus_2);
}

void SCI3_ISR_Handler(void) {
    hcs12_rs485_bus_isr(hcs12_rs485_bus_3);
}

struct rs485_bus rs485_bus_0 = {
    &__hcs12_rs485_bus_0,
    &hcs12_rs485_bus_ops,
};

struct rs485_bus rs485_bus_1 = {
    &__hcs12_rs485_bus_1,
    &hcs12_rs485_bus_ops,
};

struct rs485_bus rs485_bus_2 = {
    &__hcs12_rs485_bus_2,
    &hcs12_rs485_bus_ops,
};

struct rs485_bus rs485_bus_3 = {
    &__hcs12_rs485_bus_3,
    &hcs12_rs485_bus_ops,
};

