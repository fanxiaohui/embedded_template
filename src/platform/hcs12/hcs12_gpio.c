#include "hcs12_gpio_platform.h"

uint8_t hcs12_gpio_init(const struct hcs12_gpio *__FAR io, enum gpio_mode mode) {
    if (GPIO_MODE_OUTPUT_PUSHPULL == mode || GPIO_MODE_OUTPUT_OPENDRAIN == mode) {
        *io->dir_reg |= io->bit;
        return 1;
    }
    if (GPIO_MODE_INPUT == mode) {
        *io->dir_reg &= ~io->bit;
        return 1;
    }
    return 0;
}

enum gpio_mode hcs12_gpio_get_mode(hcs12_gpio_t io) {
    if (*io->dir_reg & io->bit) {
        return GPIO_MODE_OUTPUT_PUSHPULL;
    } else {
        return GPIO_MODE_INPUT;
    }
}

uint8_t hcs12_gpio_input_is_high(const struct hcs12_gpio *__FAR io) {
    if (*io->dir_reg & io->bit) { // output mode
        return 0;
    }

    return 0 != (*io->in_reg & io->bit);
}

uint8_t hcs12_gpio_output_is_high(const struct hcs12_gpio *__FAR io) {
    if (*io->dir_reg & io->bit) { // output mode
        return 0 != (*io->out_reg & io->bit);
    }
    return 0;
}

uint8_t hcs12_gpio_set_output(const struct hcs12_gpio *__FAR io, uint8_t is_high) {
    if (*io->dir_reg & io->bit) { // output mode
        if (is_high) {
            *io->out_reg |= io->bit;
        } else {
            *io->out_reg &= ~io->bit;
        }
        return 1;
    }

    return 0;
}

const struct gpio_operations hcs12_gpio_ops = {
    hcs12_gpio_init,
    hcs12_gpio_get_mode,
    hcs12_gpio_input_is_high,
    hcs12_gpio_output_is_high,
    hcs12_gpio_set_output,
};

