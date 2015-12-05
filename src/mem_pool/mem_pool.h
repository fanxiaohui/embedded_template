#ifndef __MEM_POLL_H__
#define __MEM_POLL_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef struct segment_header *__FAR mem_pool_t 

void mem_pool_add_segment(mem_pool_t pool, void *__FAR seg, uint16_t size);
void mem_pool_malloc(mem_pool_t pool, uint16_t size);
void mem_pool_free(mem_pool_t pool, void *__FAR seg);

#endif
