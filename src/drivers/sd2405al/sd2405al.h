#ifndef __SD2405AL_H__
#define __SD2405AL_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef struct sd2405 *__FAR sd2405_t;


uint8_t sd2405_init(sd2405_t platform);
uint8_t sd2405_write_time(sd2405_t platform,
                          uint32_t time);
uint8_t sd2405_read_time(sd2405_t platform,
                         uint32_t *time);
uint8_t sd2405_store_data(sd2405_t platform,
                          const uint8_t *__FAR dat,
                          uint8_t offset,
                          uint8_t len);
uint8_t sd2405_restore_data(sd2405_t platform,
                            uint8_t *__FAR dat,
                            uint8_t offset,
                            uint8_t len);


uint8_t sd2405_dump_regs(sd2405_t platform, uint8_t __FAR dat[32]);

enum sd2405_interrupt_type {
    SD2405_INTERRUPT_DISABLE,
    SD2405_INTERRUPT_ALARM,
    SD2405_INTERRUPT_FREQUENCY,
    SD2405_INTERRUPT_COUNTDOWN,
};

enum sd2405_interrupt_single {
    SD2405_INTERRUPT_SINGLE_LOWLEVEL,
    SD2405_INTERRUPT_SINGLE_PAUSE,
};

uint8_t sd2405_config_interrupt(sd2405_t platform, enum sd2405_interrupt_type int_type, enum sd2405_interrupt_single);
uint8_t sd2405_clear_interrupt(sd2405_t platform);
uint8_t sd2405_set_alarm(sd2405_t platform, uint32_t seconds);

#endif

