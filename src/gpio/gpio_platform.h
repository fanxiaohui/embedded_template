#ifndef __GPIO_PLATFORM_H__
#define __GPIO_PLATFORM_H__

#include "gpio.h"

struct gpio_operations {
    uint8_t (*init)(void *__FAR io, enum gpio_mode mode);
    uint8_t (*input_is_high)(void *__FAR io);
    uint8_t (*output_is_high)(void *__FAR io);
    uint8_t (*set_output)(void *__FAR io, uint8_t is_high);
};

#endif
