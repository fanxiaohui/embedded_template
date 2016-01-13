#ifndef __HCS12_GPIO_PLATFORM_H__
#define __HCS12_GPIO_PLATFORM_H__

#include "hcs12_gpio.h"
#include "gpio/gpio_platform.h"

struct hcs12_gpio {
    volatile uint8_t *in_reg;
    volatile uint8_t *out_reg;
    volatile uint8_t *dir_reg;
    volatile uint8_t bit;
};

extern const struct gpio_operations hcs12_gpio_ops;

#endif
