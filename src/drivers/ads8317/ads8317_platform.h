#ifndef __ADS8317_PLATFORM_H__
#define __ADS8317_PLATFORM_H__


#include "ads8317.h"
#include "spi/spi_platform.h"


struct ads8317_platform {
    const struct spi_bus bus;
};

#endif

