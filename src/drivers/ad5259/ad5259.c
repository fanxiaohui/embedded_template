#include "ad5259_platform.h"


#define RESISTANCE_W 0

static char init_get_resistance_ab(struct ad5259 *__FAR dev) {
    // read back Tolerance
    uint8_t buf[2];
    uint16_t tolerance;
    uint8_t addr = dev->platform->addr;
    i2c_bus_t bus = &dev->platform->bus;

    buf[0] = 0x3E; // read tolerance Consecutively
    if (1 != i2c_transmit(bus, addr, buf, 1, 0, 0)) {
        return 0;
    }
    if (2 != i2c_transmit(bus, addr, 0, 0, buf, 2)) {
        return 0;
    }

    tolerance = (uint16_t)(buf[0] & 0x7F) << 8;
    tolerance += buf[1];

    if (buf[0] & 0x80) {
        tolerance = 25600 - tolerance;
    } else {
        tolerance = 25600 + tolerance;
    }

    dev->resistance_ab = ((uint32_t)tolerance * dev->platform->nominal_resistance + 12800) / 25600;
    return 1;
}

char ad5259_init(struct ad5259 *__FAR dev) {
    i2c_bus_t bus = &dev->platform->bus;

    if (!i2c_init(bus)) {
        return 0;
    }
    if (!init_get_resistance_ab(dev)) {
        return 0;
    }
    return 1;
}

uint32_t ad5259_get_resistance_ab(struct ad5259 *__FAR dev) {
    return dev->resistance_ab;
}

char ad5259_set_resistance_wb(struct ad5259 *__FAR dev, uint32_t rwb) {
    uint8_t buf[2];
    uint32_t pos;
    uint8_t addr = dev->platform->addr;
    i2c_bus_t bus = &dev->platform->bus;

    if (rwb <= 2 * RESISTANCE_W) {
        rwb = 0;
    } else if (rwb > dev->resistance_ab + 2 * RESISTANCE_W) {
        rwb = dev->resistance_ab;
    } else {
        rwb = rwb - 2 * RESISTANCE_W;
    }

    pos = (rwb * 256 + dev->resistance_ab / 2) / dev->resistance_ab;
    buf[0] = 0x00;
    buf[1] = pos & 0xff;

    if (2 != i2c_transmit(bus, addr, buf, 2, 0, 0)) {
        return 0;
    }
    return 1;
}

char ad5259_get_resistance_wb(struct ad5259 *__FAR dev, uint32_t *__FAR rwb) {
    uint8_t buf;
    uint8_t addr = dev->platform->addr;
    i2c_bus_t bus = &dev->platform->bus;

    buf = 0x00;
    if (1 != i2c_transmit(bus, addr, &buf, 1, 0, 0)) {
        return 0;
    }
    if (1 != i2c_transmit(bus, addr, 0, 0, &buf, 1)) {
        return 0;
    }
    *rwb = (dev->resistance_ab * buf + 128) / 256 + 2 * RESISTANCE_W;
    return 1;
}


