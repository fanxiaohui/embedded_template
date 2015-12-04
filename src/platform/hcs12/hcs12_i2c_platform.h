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
    unsigned char freq_div;
};

struct hcs12_i2c {
    //struct hcs12_i2c_regs *regs;
    const struct hcs12_i2c_platform *platform;
    union {
        struct {
            unsigned char aquired: 1;
            unsigned char txing: 1;
            unsigned char rxing: 1;
            unsigned char txing_addr: 1;
            unsigned char txed_addr: 1;
        } bits;
        unsigned char byte;
    } flags;

    const unsigned char *__FAR tx_data;
    unsigned char *__FAR rx_data;
    unsigned char tx_length;
    unsigned char tx_index;
    unsigned char rx_length;
    unsigned char rx_index;
    unsigned char slave_addr;
};

void hcs12_i2c_isr(struct hcs12_i2c *__FAR i2c);

extern struct i2c_operations hcs12_i2c_ops;


#endif

