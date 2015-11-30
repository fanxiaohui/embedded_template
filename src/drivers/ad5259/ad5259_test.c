#include "ad5259_platform.h"
#include "unity_fixture.h"

static struct ad5259_platform ad5259_platform_test = {
    .nominal_resistance = 10000,
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

