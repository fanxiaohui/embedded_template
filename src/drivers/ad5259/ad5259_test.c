#include "ad5259_platform.h"
#include "unity_fixture.h"

char init(struct ad5259_platform const *__FAR platform) {
}

char i2c_write(struct ad5259_platform const *__FAR platform, uint8_t const *dat, char len) {
    return len;
}

char i2c_read(struct ad5259_platform const *__FAR platform, uint8_t *dat, char len) {
    *dat++ = 0x1C;
    *dat++ = 0x0F;
    return len;
}

static struct ad5259_platform ad5259_platform_test = {
    .platform_data = NULL,
    .nominal_resistance = 10000,
    .init = init,
    .i2c_read = i2c_read,
    .i2c_write = i2c_write,
};

static struct ad5259 ad5259_test = {
    .platform = &ad5259_platform_test,
};

TEST_GROUP(ad5259);

TEST_SETUP(ad5259) {
    TEST_ASSERT(ad5259_init(&ad5259_test));
}

TEST_TEAR_DOWN(ad5259) {
}

TEST(ad5259, get_resistance_ab) {
    uint32_t rab;
    rab = ad5259_get_resistance_ab(&ad5259_test);
    TEST_ASSERT_INT32_WITHIN(1, 12806, rab);
    printf("Rab = %d\n", rab);
}

TEST(ad5259, test_rwb) {
    uint32_t rwb;
    TEST_ASSERT(ad5259_set_resistance_wb(&ad5259_test, 8000));
    TEST_ASSERT(ad5259_get_resistance_wb(&ad5259_test, &rwb));
    TEST_ASSERT_INT32_WITHIN(10, 8000, rwb);
}

TEST_GROUP_RUNNER(ad5259) {
    RUN_TEST_CASE(ad5259, get_resistance_ab);
    //RUN_TEST_CASE(ad5259, test_rwb);
}

