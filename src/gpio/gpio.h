#ifndef __GPIO_H__
#define __GPIO_H__

typedef void *gpio_t;
typedef struct gpio_operations const *gpio_ops_t;

enum gpio_mode {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT_PUSHPULL,
    GPIO_MODE_OUTPUT_OPENDRAIN,
};

inline char gpio_init(gpio_ops_t ops, gpio_t io, enum gpio_mode mode);
inline char gpio_input_is_high(gpio_ops_t ops, gpio_t io);
inline char gpio_output_is_high(gpio_ops_t ops, gpio_t io);
inline void gpio_set_output(gpio_ops_t ops, gpio_t io, char is_high);

#endif
