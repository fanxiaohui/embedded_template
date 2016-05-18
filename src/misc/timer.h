#ifndef __TIMER_H__
#define __TIMER_H__

#include "platform_os.h"

struct timer {
    os_time_t start;
    os_time_t wait;
};

void timer_init(struct timer *);
char timer_is_expired(struct timer *);
void timer_countdown_ms(struct timer *, os_time_t);
void timer_countdown_second(struct timer *, int second);
os_time_t timer_left_ms(struct timer *);

#endif
