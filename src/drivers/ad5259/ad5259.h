#ifndef __AD5259_H__
#define __AD5259_H__

#include "stdint.h"

#ifndef __FAR
#define __FAR
#endif

typedef struct ad5259 *__FAR ad5259_t;

char ad5259_init(ad5259_t dev);
uint32_t ad5259_get_resistance_ab(ad5259_t dev);
char ad5259_set_resistance_wb(ad5259_t dev, uint32_t rwb);
char ad5259_get_resistance_wb(ad5259_t dev, uint32_t *__FAR rwb);

#endif

