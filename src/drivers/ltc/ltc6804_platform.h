#ifndef __LTC6804_PLATFORM_H__
#define __LTC6804_PLATFORM_H__

#include "stdint.h"
#include "spi_platform.h"
#include "ltc6804.h"

struct ltc6804_platform {
    const struct spi_bus bus;
    uint8_t cs_index;
};

//struct ltc6804 {
//    const struct ltc6804_platform *platform;
//};

#endif
