#ifndef __GPIO_PLATFORM_H__
#define __GPIO_PLATFORM_H__

#include "gpio.h"

struct gpio_operations {
    char (*init)(void *__FAR io, enum gpio_mode mode);
    char (*input_is_high)(void *__FAR io);
    char (*output_is_high)(void *__FAR io);
    char (*set_output)(void *__FAR io, uint8_t is_high);
};

#endif
