#include "mem_pool.h"
#include "unity_fixture.h"

#define SEGMENG_LENGTH_0 500
#define SEGMENG_LENGTH_1 500


static mem_pool_t pool = (mem_pool_t)0;
static unsigned char buffer[SEGMENG_LENGTH_0 + SEGMENG_LENGTH_1];
static unsigned char buffer2[200];

TEST_GROUP(mem_pool);

TEST_SETUP(mem_pool) {
    pool = 0;
    mem_pool_add_buffer(&pool, &buffer[0], SEGMENG_LENGTH_0);
    TEST_ASSERT_EQUAL(SEGMENG_LENGTH_0, mem_pool_get_free_size(pool));
    TEST_ASSERT_EQUAL(1, mem_pool_get_segment_count(pool));
    mem_pool_add_buffer(&pool, &buffer[SEGMENG_LENGTH_0], SEGMENG_LENGTH_1);
    TEST_ASSERT_EQUAL(SEGMENG_LENGTH_0 + SEGMENG_LENGTH_1, mem_pool_get_free_size(pool));
    TEST_ASSERT_EQUAL(1, mem_pool_get_segment_count(pool)); 
    mem_pool_add_buffer(&pool, &buffer2[1], sizeof(buffer2)-1);
    TEST_ASSERT_EQUAL(SEGMENG_LENGTH_0 + SEGMENG_LENGTH_1 + sizeof(buffer2) - 1, mem_pool_get_free_size(pool));
    TEST_ASSERT_EQUAL(2, mem_pool_get_segment_count(pool)); 
}

TEST_TEAR_DOWN(mem_pool) {
}

TEST(mem_pool, malloc) {
    void *__FAR m1;
    void *__FAR m2;
    void *__FAR m3;
    
    m1 = mem_pool_malloc(&pool, 100);
    TEST_ASSERT_EQUAL(2, mem_pool_get_segment_count(pool));  
    m2 = mem_pool_malloc(&pool, 100);
    TEST_ASSERT_EQUAL(2, mem_pool_get_segment_count(pool)); 
    m3 = mem_pool_malloc(&pool, 100);
    TEST_ASSERT_EQUAL(2, mem_pool_get_segment_count(pool));
    
    mem_pool_free(&pool, m2); 
    TEST_ASSERT_EQUAL(3, mem_pool_get_segment_count(pool));
    mem_pool_free(&pool, m1);
    TEST_ASSERT_EQUAL(3, mem_pool_get_segment_count(pool)); 
    mem_pool_free(&pool, m3);
    TEST_ASSERT_EQUAL(2, mem_pool_get_segment_count(pool));  
}

TEST_GROUP_RUNNER(mem_pool) {
    RUN_TEST_CASE(mem_pool, malloc);
}

