#include <unity_fixture.h>



static void run_all_test(void) {
    RUN_TEST_GROUP(ringbuffer);
    RUN_TEST_GROUP(hcf4052);
    RUN_TEST_GROUP(ad5259);
}

int main(int argc, const char **argv) {
    (void)UnityMain(argc, argv, run_all_test);
}
