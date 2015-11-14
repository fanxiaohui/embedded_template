#ifndef __HCF4052_PLATFORM_H__
#define __HCF4052_PLATFORM_H__

#include "hcf4052.h"

#define HCF4052_CTRL_PIN_A 0
#define HCF4052_CTRL_PIN_B 1
#define HCF4052_CTRL_PIN_INH 2

struct hcf4052_platform {
    void *__FAR platform_data;
    void (*set_ctrl_pin)(struct hcf4052_platform const*__FAR platform, char pin, char is_high);
};

#endif
