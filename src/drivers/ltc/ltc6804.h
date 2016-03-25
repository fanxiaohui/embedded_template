#ifndef __LTC6804_H__
#define __LTC6804_H__

#include "stdint.h"

#ifndef __FAR
#define __FAR
#endif

struct ltc6804_config_reg {
    uint8_t CFGR0;
    uint8_t CFGR1;
    uint8_t CFGR2;
    uint8_t CFGR3;
    uint8_t CFGR4;
    uint8_t CFGR5;
};

typedef struct ltc6804 *__FAR ltc6804_t;

void ltc6804_init(ltc6804_t dev);
uint8_t ltc6804_1_read_cfg(ltc6804_t dev, struct ltc6804_config_reg *cfg, uint8_t num);
void ltc6804_1_write_cfg(ltc6804_t dev, struct ltc6804_config_reg const *cfg, uint8_t num);
uint8_t ltc6804_1_get_voltage(ltc6804_t dev, uint16_t *buf, uint8_t size);
uint8_t ltc6804_1_get_aux(ltc6804_t dev, uint16_t *buf, uint8_t size);
uint8_t ltc6804_get_voltage(ltc6804_t dev, uint8_t index, uint16_t *vol);
uint8_t ltc6804_read(ltc6804_t dev, uint16_t cmd, uint8_t *buf, uint8_t len);
uint8_t ltc6804_start_channel(ltc6804_t dev, uint8_t channel);
uint8_t ltc6804_start_all(ltc6804_t dev);

#endif
