#ifndef __HCF4052_PLATFORM_H__
#define __HCF4052_PLATFORM_H__

#include "hcf4052.h"
#include "gpio/gpio_platform.h"

struct hcf4052_platform {
    void *ctrl_a;
    void *ctrl_b;
    void *ctrl_inh;
    const struct gpio_operations *gpio_ops;
};

#endif
