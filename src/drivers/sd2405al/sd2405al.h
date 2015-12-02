#ifndef __SD2405AL_H__
#define __SD2405AL_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef const struct sd2405_platform *__FAR sd2405_t;

uint8_t sd2405_init(sd2405_t platform);
uint8_t sd2405_write_time(sd2405_t platform,
                          uint32_t time);
uint8_t sd2405_read_time(sd2405_t platform,
                         uint32_t *__FAR time);
uint8_t sd2405_store_data(sd2405_t platform,
                          const uint8_t *__FAR dat,
                          uint8_t offset,
                          uint8_t len);
uint8_t sd2405_restore_data(sd2405_t platform,
                            uint8_t *__FAR dat,
                            uint8_t offset,
                            uint8_t len);

#endif

