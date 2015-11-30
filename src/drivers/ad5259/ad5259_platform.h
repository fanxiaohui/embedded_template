#ifndef __AD5259_PLATFORM_H__
#define __AD5259_PLATFORM_H__

#include "ad5259.h"
#include "i2c/i2c.h"

struct ad5259_platform {
    uint32_t nominal_resistance;
    uint8_t addr;
    i2c_t bus;
};

struct ad5259 {
    struct ad5259_platform const *__FAR platform;
    uint32_t resistance_ab;
};

#endif

