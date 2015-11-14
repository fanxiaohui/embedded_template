#include "hcf4052_platform.h"
#include "unity_fixture.h"

#define A_BIT 0x01
#define B_BIT 0x02
#define INH_BIT 0x04

static void set_ctrl_pin(struct hcf4052_platform const *__FAR platform, char pin, char is_high) {
    unsigned char bit;
    unsigned char *__FAR dat = platform->platform_data;
    TEST_ASSERT(pin == HCF4052_CTRL_PIN_A
                || pin == HCF4052_CTRL_PIN_B
                || pin == HCF4052_CTRL_PIN_INH);

    if (pin == HCF4052_CTRL_PIN_A) {
        bit = A_BIT;
    } else if (pin == HCF4052_CTRL_PIN_B) {
        bit = B_BIT;
    } else if (pin == HCF4052_CTRL_PIN_INH) {
        bit = INH_BIT;
    }

    if (is_high) {
        *dat |= bit;
    } else {
        *dat &= ~bit;
    }
}

static unsigned char dat;
static const struct hcf4052_platform hcf4052_platform_test = {
    .platform_data = &dat,
    .set_ctrl_pin = set_ctrl_pin,
};


static char bits_is_set(struct hcf4052_platform const *__FAR platform, unsigned char bits) {
    unsigned char *__FAR dat = platform->platform_data;
    return (*dat & bits) == bits;
}

static char bits_is_clear(struct hcf4052_platform const *__FAR platform, unsigned char bits) {
    unsigned char *__FAR dat = platform->platform_data;
    return (*dat & bits) == 0;
}


TEST_GROUP(hcf4052);

TEST_SETUP(hcf4052) {
}

TEST_TEAR_DOWN(hcf4052) {
}

TEST(hcf4052, test) {
    hcf4052_select_channel(&hcf4052_platform_test, HCS4052_CHANNEL_NONE);
    TEST_ASSERT(bits_is_set(&hcf4052_platform_test, INH_BIT));

    hcf4052_select_channel(&hcf4052_platform_test, HCS4052_CHANNEL_0);
    TEST_ASSERT(bits_is_clear(&hcf4052_platform_test, A_BIT | B_BIT | INH_BIT));

    hcf4052_select_channel(&hcf4052_platform_test, HCS4052_CHANNEL_1);
    TEST_ASSERT(bits_is_set(&hcf4052_platform_test, A_BIT));
    TEST_ASSERT(bits_is_clear(&hcf4052_platform_test, B_BIT | INH_BIT));

    hcf4052_select_channel(&hcf4052_platform_test, HCS4052_CHANNEL_2);
    TEST_ASSERT(bits_is_set(&hcf4052_platform_test, B_BIT));
    TEST_ASSERT(bits_is_clear(&hcf4052_platform_test, A_BIT | INH_BIT));

    hcf4052_select_channel(&hcf4052_platform_test, HCS4052_CHANNEL_3);
    TEST_ASSERT(bits_is_set(&hcf4052_platform_test, A_BIT | B_BIT));
    TEST_ASSERT(bits_is_clear(&hcf4052_platform_test, INH_BIT));

    hcf4052_select_channel(&hcf4052_platform_test, 126);
    TEST_ASSERT(bits_is_set(&hcf4052_platform_test, INH_BIT));
}


TEST_GROUP_RUNNER(hcf4052) {
    RUN_TEST_CASE(hcf4052, test);
}
