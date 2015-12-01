#ifndef __ADS8317_H__
#define __ADS8317_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef const struct ads8317_platform *__FAR ads8317_t;

void ads8317_init(ads8317_t dev);
int16_t ads8317_read(ads8317_t dev);

#endif
