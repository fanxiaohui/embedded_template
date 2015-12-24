#include "hcf4052_platform.h"

void hcf4052_init(const struct hcf4052_platform *__FAR platform) {
    gpio_init(platform->gpio_ops, platform->ctrl_inh, GPIO_MODE_OUTPUT_PUSHPULL);
    gpio_set_output(platform->gpio_ops, platform->ctrl_inh, 1);
    gpio_init(platform->gpio_ops, platform->ctrl_a, GPIO_MODE_OUTPUT_PUSHPULL);
    gpio_init(platform->gpio_ops, platform->ctrl_b, GPIO_MODE_OUTPUT_PUSHPULL);
}

void hcf4052_select_channel(const struct hcf4052_platform *__FAR platform, enum hcf4052_channel channel) {
    // disable
    gpio_set_output(platform->gpio_ops, platform->ctrl_inh, 1);

    // switch channel
    switch (channel) {
    case HCF4052_CHANNEL_0:
        gpio_set_output(platform->gpio_ops, platform->ctrl_a, 0);
        gpio_set_output(platform->gpio_ops, platform->ctrl_b, 0);
        break;
    case HCF4052_CHANNEL_1:
        gpio_set_output(platform->gpio_ops, platform->ctrl_a, 1);
        gpio_set_output(platform->gpio_ops, platform->ctrl_b, 0);
        break;
    case HCF4052_CHANNEL_2:
        gpio_set_output(platform->gpio_ops, platform->ctrl_a, 0);
        gpio_set_output(platform->gpio_ops, platform->ctrl_b, 1);
        break;
    case HCF4052_CHANNEL_3:
        gpio_set_output(platform->gpio_ops, platform->ctrl_a, 1);
        gpio_set_output(platform->gpio_ops, platform->ctrl_b, 1);
        break;
    default:
        return;
    }

    // enable
    gpio_set_output(platform->gpio_ops, platform->ctrl_inh, 0);
}


enum hcf4052_channel hcf4052_get_current_channel(const struct hcf4052_platform *__FAR platform) {
    uint8_t bits = 0;
    static const enum hcf4052_channel io_to_channel[] = {
        HCF4052_CHANNEL_0,
        HCF4052_CHANNEL_2,
        HCF4052_CHANNEL_1,
        HCF4052_CHANNEL_3,
    };
    if (gpio_output_is_high(platform->gpio_ops, platform->ctrl_inh)) {
        return HCF4052_CHANNEL_NONE;
    }

    if (gpio_output_is_high(platform->gpio_ops, platform->ctrl_a)) {
        bits |= 0x01;
    }
    if (gpio_output_is_high(platform->gpio_ops, platform->ctrl_a)) {
        bits |= 0x02;
    }

    return io_to_channel[bits];
}

