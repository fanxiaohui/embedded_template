#include "hcf4052_platform.h"
#include "unity_fixture.h"

#define A_BIT 0x01
#define B_BIT 0x02
#define INH_BIT 0x04

static const struct hcf4052_platform hcf4052_platform_test = {
};


static char bits_is_set(struct hcf4052_platform const *__FAR platform, unsigned char bits) {
    return 0;
}

static char bits_is_clear(struct hcf4052_platform const *__FAR platform, unsigned char bits) {
    return 0;
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
