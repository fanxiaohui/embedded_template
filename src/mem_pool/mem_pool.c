#include "mem_pool.h"
#include "misc/assert.h"
#include "misc/misc.h"

struct mem_pool_segment *__FAR const MEM_POOL_POISON = (struct mem_pool_segment *__FAR)0xEFCDAB89;

struct mem_pool_segment {
    struct mem_pool_segment *__FAR next;
    uint16_t size;
};

static inline void try_combine_with_next(struct mem_pool_segment *__FAR seg) {
    struct mem_pool_segment *__FAR next = seg->next;
    if (0 != next) {
        if (seg->size + (uint8_t *__FAR)seg == (uint8_t *__FAR)next) {
            seg->next = next->next;
            seg->size += next->size;
            return;
        }
        ASSERT(seg->size + (uint8_t *__FAR)seg < (uint8_t *__FAR)next);
    }
}

static inline struct mem_pool_segment *__FAR add_segment(struct mem_pool_segment *__FAR pool, struct mem_pool_segment *__FAR seg) {
    struct mem_pool_segment *__FAR pre;
    struct mem_pool_segment *__FAR cur;

    if (0 == pool) { // NULL memory pool
        seg->next = 0;
        return seg;
    }

    if ((uint8_t *__FAR)seg < (uint8_t *__FAR)pool) {
        seg->next = pool;
        try_combine_with_next(seg);
        return seg;
    }

    pre = pool;
    for (;;) {
        cur = pre->next;
        if (0 == cur) {
            pre->next = seg;
            try_combine_with_next(pre);
            return pool;

        }

        if ((uint8_t *__FAR)seg < (uint8_t *__FAR)cur) {
            seg->next = cur;
            try_combine_with_next(seg);
            pre->next = seg;
            try_combine_with_next(pre);
            return pool;
        }

        pre = cur;
        cur = cur->next;
    }
}

static inline uint8_t splite(struct mem_pool_segment *__FAR seg, uint16_t size, struct mem_pool_segment *__FAR *left) {
    if (seg->size < size) {
        return 0;
    }

    if (seg->size > size + 2 * ALLIGN_SIZEOF(struct mem_pool_segment)) {
        *left = (struct mem_pool_segment * __FAR)(((uint8_t *__FAR)seg) + size);
        (*left)->size = seg->size - size;
        (*left)->next = seg->next;
        seg->size = size;
    } else {
        *left = 0;
    }
    return 1;
}

void mem_pool_add_buffer(struct mem_pool_segment *__FAR *__FAR pool, void *__FAR buffer, uint16_t size) {
    struct mem_pool_segment *__FAR seg = (struct mem_pool_segment *__FAR)buffer;
    seg->size = size;
    *pool = add_segment(*pool, seg);
}

void *__FAR mem_pool_malloc(struct mem_pool_segment *__FAR *__FAR pool, uint16_t size) {
    struct mem_pool_segment *__FAR cur;
    struct mem_pool_segment *__FAR n;

    ASSERT(pool != 0);

    if (0 == *pool) {
        return 0;
    }

    size += ALLIGN_SIZEOF(struct mem_pool_segment);


    cur = *pool;
    if (splite(cur, size, &n)) {
        *pool = n;
        cur->next = MEM_POOL_POISON;
        return &cur[1];
    }

    for (;;) {
        if (0 == cur->next) return 0;
        if (splite(cur->next, size, &n)) {
            struct mem_pool_segment *__FAR ret = cur->next;
            cur->next = n;
            ret->next = MEM_POOL_POISON;
            return &ret[1];
        }
        cur = cur->next;
    }
}

void mem_pool_free(struct mem_pool_segment *__FAR *__FAR pool, void *__FAR mem) {
    ASSERT(pool != 0);
    if (mem != 0) {
        struct mem_pool_segment *__FAR seg = &((struct mem_pool_segment * __FAR)mem)[-1];
        ASSERT(MEM_POOL_POISON == seg->next);
        *pool = add_segment(*pool, seg);
        return;
    }
}


uint16_t mem_pool_get_free_size(struct mem_pool_segment *__FAR pool) {
    if (pool != 0) {
        uint16_t ret = 0;
        for (; 0 != pool; pool = pool->next) {
            ret += pool->size;
        }
        return ret;        
    }
    
    return 0;
    
}

uint16_t mem_pool_get_segment_count(mem_pool_t pool) {
    if (pool != 0) {
        uint16_t ret = 0;
        for (; 0 != pool; pool = pool->next) {
            ++ret;
        }
        return ret;        
    }
    return 0;
}

