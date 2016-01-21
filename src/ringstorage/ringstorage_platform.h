#ifndef __RINGSTRAGE_PLATFORM_H__
#define __RINGSTRAGE_PLATFORM_H__

#include "ringstorage.h"

struct ringstorage_media {
    uint16_t blocks_per_chip;
    uint16_t sectors_per_block;
    uint16_t bytes_per_sector;

    char (*erase_block)(uint32_t addr);
    char (*write)(uint32_t sector, const char *buf);
    char (*read)(uint32_t sector, char *buf, uint16_t len);
};

struct ringstorage {
    const struct ringstorage_media *__FAR media;
    uint8_t *__FAR write_buffer;
    uint8_t *__FAR read_buffer;
    uint16_t first_block;
    uint16_t last_block;
    uint32_t next_block_flag;
    uint16_t last_sector;
    uint16_t last_byte;
};

#endif
