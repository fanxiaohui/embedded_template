#include "hcf4052_platform.h"

void hcf4052_select_channel(hcf4052_platform_t hcf4052_platform, enum hcf4052_channel channel) {
    // disable
    hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_INH, 1);

    // switch channel
    switch(channel) {
    case HCS4052_CHANNEL_0:
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_A, 0);
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_B, 0);
        break;
    case HCS4052_CHANNEL_1:
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_A, 1);
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_B, 0);
        break;
    case HCS4052_CHANNEL_2:
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_A, 0);
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_B, 1);
        break;
    case HCS4052_CHANNEL_3:
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_A, 1);
        hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_B, 1);
        break;
    default:
        return;
    }

    // enable

    hcf4052_platform->set_ctrl_pin(hcf4052_platform, HCF4052_CTRL_PIN_INH, 0);
}
