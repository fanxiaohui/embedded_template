#include "i2c_platform.h"

uint8_t i2c_init(const struct i2c_bus *__FAR bus) {
    return bus->ops->init(bus->private_data);
}

uint8_t i2c_transmit(const struct i2c_bus *__FAR bus,
                            uint8_t addr,
                            const uint8_t *__FAR w,
                            uint8_t wlen,
                            uint8_t *__FAR r,
                            uint8_t rlen) {
    return bus->ops->transmit(bus->private_data, addr, w, wlen, r, rlen);
}


