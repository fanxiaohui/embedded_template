#include "i2c_platform.h"

inline char i2c_init(const struct i2c_bus *bus) {
    return bus->ops->init(bus->private_data);
}

inline uint8_t i2c_transmit(const struct i2c_bus *bus,
                     uint8_t addr,
                     const uint8_t *w,
                     uint8_t wlen,
                     uint8_t *r,
                     uint8_t rlen) {
    return bus->ops->transmit(bus->private_data, addr, w, wlen, r, rlen);
}

