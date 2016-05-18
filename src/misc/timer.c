#include "timer.h"
#include "platform_os.h"

void timer_init(struct timer *t) {
    t->start = os_get_time();
    t->wait = 0;
}

char timer_is_expired(struct timer *t) {
    return (t->start + t->wait) <= os_get_time();
}

void timer_countdown_ms(struct timer *t, os_time_t wait_ms) {
    t->start = os_get_time();
    t->wait = wait_ms;
}

void timer_countdown_second(struct timer *t, int wait_second) {
    t->start = os_get_time();
    t->wait = wait_second * 1000;
}

os_time_t timer_left_ms(struct timer *t) {
    os_time_t elapse = os_get_time() - t->start;
    if (elapse >= t->wait) {
        return 0;
    } else {
        return t->wait - elapse;
    }
}

