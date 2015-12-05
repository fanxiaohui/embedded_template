#include "mem_pool.h"
#include "misc/assert.h"

struct segment_header {
    struct segment_header *__FAR next;
    uint16_t size;
}

void mem_pool_add_segment(struct segment_header *__FAR pool, void *__FAR buf, uint16_t size) {
    struct segment_header *__FAR cur;
    struct segment_header *__FAR seg;
    ASSERT(pool != 0);

    seg = (struct segment_header *__FAR)buf;
    seg->size = size;

    for(;;) {
        cur = pool;
        pool = pool->next;

        if (pool == 0) { // end of list
            cur->next = seg;
            break;
        }

        if ((uint8_t *__FAR)pool > (uint8_t *__FAR)seg) {// next segment address > seg, add befor.
            ASSERT(((uint8_t *__FAR)cur + cur->size) > (uint8_t *__FAR)seg);
            if (((uint8_t *__FAR)cur  + cur->size) == (uint8_t *__FAR)seg) {
                cur->size += seg->size;
            } else {
                cur->next = seg;
            }
            break;
        }
    }
}

void mem_pool_malloc(struct segment_header_t *__FAR pool, uint16_t size) {
}

void mem_pool_free(struct segment_header_t *__FAR pool, void *__FAR men) {
}


