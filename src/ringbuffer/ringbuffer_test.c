#include <string.h>

#include "ringbuffer.h"
#include "unity_fixture.h"


#define TEST_RINGBUFFER_SIZE 128

RINGBUFFER_DEFINE_STATIC(forTest, TEST_RINGBUFFER_SIZE);

TEST_GROUP(ringbuffer);

TEST_SETUP(ringbuffer) {
}

TEST_TEAR_DOWN(ringbuffer) {
}

TEST(ringbuffer, aaa) {

    unsigned int len;
    unsigned char buf1[20];
    unsigned char buf2[20];

    (void)memset(buf2, 0x28, sizeof(buf2));

    ringbuffer_clear(&forTest);
    TEST_ASSERT_EQUAL(TEST_RINGBUFFER_SIZE, ringbuffer_get_left_space(&forTest));
    TEST_ASSERT_EQUAL(56, ringbuffer_put(&forTest, 0x28, 56));
    TEST_ASSERT_EQUAL(TEST_RINGBUFFER_SIZE - 56, ringbuffer_get_left_space(&forTest));

    for (len = 0; len < 56; ) {
        int read = 56 - len;
        read = read > sizeof(buf1) ? sizeof(buf1) : read;
        TEST_ASSERT_EQUAL(read, ringbuffer_read(&forTest, buf1, read));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buf1, buf2, read);
        //TEST_ASSERT_EQUAL_UINT8_ARRAY(buf1, "Hello world!!!", read);
        len += read;
    }


    (void)strcpy((char *)buf1, "Hello world\n");
    TEST_ASSERT_EQUAL(strlen(buf1) + 1, ringbuffer_write(&forTest, buf1, strlen(buf1) + 1));
    TEST_ASSERT_EQUAL(strlen(buf1) + 1, ringbuffer_read_string(&forTest, buf2, sizeof(buf2)));
    TEST_ASSERT_EQUAL_STRING(buf1, buf2);



    (void)strcpy(buf1, "Hello world\n");
    len = sizeof(buf2);
    TEST_ASSERT_EQUAL(strlen(buf1), ringbuffer_write(&forTest, buf1, strlen(buf1)));
    TEST_ASSERT(ringbuffer_read_string_and_is_with_prefix(&forTest, "Hello", buf2, &len));
    TEST_ASSERT_EQUAL(strlen(buf1), len);
    TEST_ASSERT_EQUAL_STRING(buf1, buf2);

    (void)strcpy(buf1, "Hello world\n");
    len = sizeof(buf2);
    TEST_ASSERT_EQUAL(strlen(buf1), ringbuffer_write(&forTest, buf1, strlen(buf1)));
    TEST_ASSERT_FALSE(ringbuffer_read_string_and_is_with_prefix(&forTest, "world", buf2, &len));
    TEST_ASSERT_EQUAL(strlen(buf1), len);
    TEST_ASSERT_EQUAL_STRING(buf1, buf2);
}

TEST_GROUP_RUNNER(ringbuffer) {
    RUN_TEST_CASE(ringbuffer, aaa);
}

