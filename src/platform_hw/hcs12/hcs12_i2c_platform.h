#ifndef __HCS12_I2C_PRIVATE_H__
#define __HCS12_I2C_PRIVATE_H__

#include "hcs12_i2c.h"

#include "derivative.h"

struct hcs12_i2c_regs {
    volatile IIC1_IBADSTR address;
    volatile IIC1_IBFDSTR freq_div;
    volatile IIC1_IBCRSTR control;
    volatile IIC1_IBSRSTR status;
    volatile IIC1_IBDRSTR dat;
    volatile IIC1_IBCR2STR control2;
};

struct hcs12_i2c_platform {
    struct hcs12_i2c_regs *regs;
    uint8_t freq_div;
};

struct hcs12_i2c {
    //struct hcs12_i2c_regs *regs;
    const struct hcs12_i2c_platform *platform;
    union {
        struct {
            uint8_t aquired: 1;
            uint8_t txing: 1;
            uint8_t rxing: 1;
            uint8_t txing_addr: 1;
            uint8_t txed_addr: 1;
        } bits;
        uint8_t byte;
    } flags;

    const uint8_t *__FAR tx_data;
    uint8_t *__FAR rx_data;
    uint8_t tx_length;
    uint8_t tx_index;
    uint8_t rx_length;
    uint8_t rx_index;
    uint8_t slave_addr;
};

void hcs12_i2c_isr(struct hcs12_i2c *__FAR i2c);

extern struct i2c_operations hcs12_i2c_ops;


#endif

