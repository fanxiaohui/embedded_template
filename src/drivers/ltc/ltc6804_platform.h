#ifndef __LTC6804_PLATFORM_H__
#define __LTC6804_PLATFORM_H__

#include "stdint.h"
#include "spi_platform.h"
#include "ltc6804.h"


#define VOL_REG_SIZE 6

struct ltc6804_vol_reg {
    uint8_t dat[VOL_REG_SIZE];
};

struct ltc6804_platform {
    const struct spi_bus bus;
    uint8_t cs_index;
};

struct ltc6804 {
    const struct ltc6804_platform *platform;
    struct ltc6804_vol_reg reg;
};

#endif
