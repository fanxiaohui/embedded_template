#include <stdio.h>
#include "gpio_platform.h"


inline uint8_t gpio_init(struct gpio_operations const *__FAR ops, void *__FAR io, enum gpio_mode mode) {
    return ops->init(io, mode);
}


inline enum gpio_mode gpio_get_mode(struct gpio_operations const *__FAR ops, void *__FAR io) {
    return ops->get_mode(io);
}

inline uint8_t gpio_input_is_high(struct gpio_operations const *__FAR ops, void *__FAR io) {
    return ops->input_is_high(io);
}

inline uint8_t gpio_output_is_high(struct gpio_operations const *__FAR ops, void *__FAR io) {
    return ops->output_is_high(io);
}

inline uint8_t gpio_set_output(struct gpio_operations const *__FAR ops, void *__FAR io, uint8_t is_high) {
    return ops->set_output(io, is_high);
}

inline const char *gpio_mode_to_string(enum gpio_mode mode) {
    switch (mode) {
    case GPIO_MODE_INPUT:
        return "INPUT";
    case GPIO_MODE_OUTPUT_PUSHPULL:
        return "OUTPUT_PP";
    case GPIO_MODE_OUTPUT_OPENDRAIN:
        return "OUTPUT_OD";
    default:
        return "UNKNOWN";
    }
}

void gpio_dump(struct gpio_operations const *__FAR ops, void *__FAR io, const char *prefix) {
    if (ops && io) {
        uint8_t v;
        enum gpio_mode mode = gpio_get_mode(ops, io);
        if (mode == GPIO_MODE_INPUT) {
            v = gpio_input_is_high(ops, io);
        } else {
            v = gpio_output_is_high(ops, io);
        }

        if (prefix) {
            printf("%s:%s:%d\n", prefix, gpio_mode_to_string(mode), v);
        } else {
            printf("%s:%d\n", gpio_mode_to_string(mode), v);
        }
    }
}

