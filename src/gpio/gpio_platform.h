#ifndef __GPIO_PLATFORM_H__
#define __GPIO_PLATFORM_H__

#include "gpio.h"

struct gpio_operations {
    char (*init)(void *io, char mode);
    char (*input_is_high)(void *io);
    char (*output_is_high)(void *io);
    char (*set_output)(void *io, char is_high);
};

#endif
