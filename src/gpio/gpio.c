#include "gpio_platform.h"

inline char gpio_init(struct gpio_operations const *__FAR ops, void *__FAR io, enum gpio_mode mode) {
    return ops->init(io, mode);
}

inline char gpio_input_is_high(struct gpio_operations const *__FAR ops, void *__FAR io) {
    return ops->input_is_high(io);
}

inline char gpio_output_is_high(struct gpio_operations const *__FAR ops, void *__FAR io) {
    return ops->output_is_high(io);
}

inline char gpio_set_output(struct gpio_operations const *__FAR ops, void *__FAR io, uint8_t is_high) {
    return ops->set_output(io, is_high);
}

