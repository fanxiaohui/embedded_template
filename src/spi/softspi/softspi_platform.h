#ifndef __SOFTSPI_PLATFORM_H__
#define __SOFTSPI_PLATFORM_H__

#include "spi/spi.h"
#include "gpio/gpio_platform.h"

struct softspi_platform {
    const struct gpio_operations *gpio_ops;
    void *clk;
    void *dat;
    void **cs_pin;
    uint8_t cs_num;
};

struct softspi {
    unsigned char flags;
    const struct softspi_platform *platform;
};


#endif

