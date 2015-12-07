#ifndef __MEM_POLL_H__
#define __MEM_POLL_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

struct mem_pool_segment;

typedef struct mem_pool_segment *__FAR mem_pool_t;
typedef mem_pool_t *__FAR mem_pool_p; 

void mem_pool_add_buffer(mem_pool_p pool, void *__FAR buffer, uint16_t size);
void *__FAR mem_pool_malloc(mem_pool_p pool, uint16_t size);
void mem_pool_free(mem_pool_p pool, void *__FAR mem);
uint16_t mem_pool_get_free_size(mem_pool_t pool);
uint16_t mem_pool_get_segment_count(mem_pool_t pool);

#endif
