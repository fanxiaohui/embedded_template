#include "ringstorage_platform.h"

char ringstorage_init(struct ringstorage *__FAR rs) {
    uint16_t block;
    uint16_t sector;
    uint32_t addr;

    for (addr = 0, block = 0; block < rs->media->blocks_per_chip; ++block) {
        for (sector = 0; sector < rs->media->sectors_per_block; ++sector) {
        }
    }
}

