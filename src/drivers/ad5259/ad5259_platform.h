#ifndef __AD5259_PLATFORM_H__
#define __AD5259_PLATFORM_H__

#include "ad5259.h"

struct ad5259_platform {
    void *__FAR platform_data;
    uint32_t nominal_resistance;
    char (*init)(struct ad5259_platform const *__FAR platform);
    char (*i2c_write)(struct ad5259_platform const *__FAR platform, uint8_t const *dat, char len);
    char (*i2c_read)(struct ad5259_platform const *__FAR platform, uint8_t *dat, char len);
};

struct ad5259 {
    struct ad5259_platform const *__FAR platform;
    uint32_t resistance_ab;
};

#endif

