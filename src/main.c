#include <stdio.h>
#include <async/async.h>
#include <async/timer.h>

static int i;

async_timeout_t hello_world(async_timer_t *t) {
    int *p = async_timer_get_data(t);
    printf("hello world: %d, now is %d\n", (*p)++, async_get_time());
    if (*p > 10) async_exit();
    return 1000;
}

int main(int argc, char **argv) {
    async_init();
    async_timer_register(hello_world, 0, &i);
    async_looper_loop();
    return 0;
}

