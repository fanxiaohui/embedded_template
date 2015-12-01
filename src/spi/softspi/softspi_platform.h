#ifndef __SOFTSPI_PLATFORM_H__
#define __SOFTSPI_PLATFORM_H__

#include "softspi.h"

#include "spi/spi_platform.h"
#include "gpio/gpio_platform.h"

struct softspi_platform {
    const struct gpio_operations *__FAR gpio_ops;
    void *__FAR clk;
    void *__FAR mosi;
    void *__FAR miso;
    void *__FAR *__FAR cs_pin;
    uint8_t cs_num;
};

struct softspi {
    unsigned char flags;
    const struct softspi_platform *__FAR platform;
};


#endif

