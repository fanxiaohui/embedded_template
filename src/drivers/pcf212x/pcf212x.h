#ifndef __PCF212X_H__
#define __PCF212X_H__

#include "stdint.h"

#ifndef __FAR
#define __FAR
#endif

typedef struct pcf212x *__FAR pcf212x_t;


uint8_t pcf212x_init(pcf212x_t dev);
uint8_t pcf212x_write_time(pcf212x_t dev,
                           uint32_t seconds);
uint8_t pcf212x_read_time(pcf212x_t dev,
                          uint32_t *seconds);
uint8_t pcf212x_store_data(pcf212x_t dev,
                           const uint8_t *__FAR dat,
                           uint8_t offset,
                           uint8_t len);
uint8_t pcf212x_restore_data(pcf212x_t dev,
                             uint8_t *__FAR dat,
                             uint8_t offset,
                             uint8_t len);


uint8_t pcf212x_dump_regs(pcf212x_t dev, uint8_t __FAR dat[0x1C]);

enum PCF212X_interrupt_type {
    PCF212X_INTERRUPT_DISABLE,
    PCF212X_INTERRUPT_ALARM,
    PCF212X_INTERRUPT_FREQUENCY,
    PCF212X_INTERRUPT_COUNTDOWN,
};

enum pcf212x_interrupt_single {
    PCF212X_INTERRUPT_SINGLE_LOWLEVEL,
    PCF212X_INTERRUPT_SINGLE_PAUSE,
};

uint8_t pcf212x_config_interrupt(pcf212x_t dev, enum pcf212x_interrupt_type int_type, enum pcf212x_interrupt_single);
uint8_t pcf212x_clear_alarm(pcf212x_t dev);
uint8_t pcf212x_set_alarm(pcf212x_t dev, uint32_t seconds);


#endif