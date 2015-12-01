#include "gpio_platform.h"

inline char gpio_init(gpio_ops_t ops, gpio_t io, enum gpio_mode mode) {
    return ops->init(io, mode);
}

inline char gpio_input_is_high(gpio_ops_t ops, gpio_t io) {
    return ops->input_is_high(io);
}

inline char gpio_output_is_high(gpio_ops_t ops, gpio_t io) {
    return ops->output_is_high(io);
}

inline void gpio_set_output(gpio_ops_t ops, gpio_t io, char is_high) {
    ops->set_output(io, is_high);
}

