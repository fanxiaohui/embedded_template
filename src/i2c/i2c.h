#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>


typedef const struct i2c *i2c_t;

struct i2c {
    void *private_data;
    char (*init)(const struct i2c *bus);
    uint8_t (*transmit)(const struct i2c *bus,
                              uint8_t addr,
                              const uint8_t *w,
                              uint8_t wlen,
                              uint8_t *r,
                              uint8_t rlen);

};

static inline char i2c_init(const struct i2c *bus) {
    return bus->init(bus);
}

static uint8_t i2c_transmit(const struct i2c *bus,
                              uint8_t addr,
                              const uint8_t *w,
                              uint8_t wlen,
                              uint8_t *r,
                              uint8_t rlen) {
    return bus->transmit(bus, addr, w, wlen, r, rlen);
}


#endif

