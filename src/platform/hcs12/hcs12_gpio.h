#ifndef __HCS12_GPIO_H__
#define __HCS12_GPIO_H__

#include <stdint.h>

#include "gpio/gpio.h"

#ifndef __FAR
#define __FAR
#endif


typedef const struct hcs12_gpio *__FAR hcs12_gpio_t;

uint8_t hcs12_gpio_init(hcs12_gpio_t io, enum gpio_mode mode);
uint8_t hcs12_gpio_input_is_high(hcs12_gpio_t io);
uint8_t hcs12_gpio_output_is_high(hcs12_gpio_t io);
uint8_t hcs12_gpio_set_output(hcs12_gpio_t io, uint8_t is_high);

#endif

