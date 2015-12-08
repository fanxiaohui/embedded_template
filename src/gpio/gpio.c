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
    switch(mode) {
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
