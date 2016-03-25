#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef void *__FAR gpio_t;
typedef struct gpio_operations const *__FAR gpio_ops_t;

enum gpio_mode {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT_PUSHPULL,
    GPIO_MODE_OUTPUT_OPENDRAIN,
};

uint8_t gpio_init(gpio_ops_t ops, gpio_t io, enum gpio_mode mode);
enum gpio_mode gpio_get_mode(gpio_ops_t ops, gpio_t io);
const char *gpio_mode_to_string(enum gpio_mode moe);
uint8_t gpio_input_is_high(gpio_ops_t ops, gpio_t io);
uint8_t gpio_output_is_high(gpio_ops_t ops, gpio_t io);
uint8_t gpio_set_output(gpio_ops_t ops, gpio_t io, uint8_t is_high);
void gpio_dump(struct gpio_operations const *__FAR ops, void *__FAR io, const char *prefix);

#endif
