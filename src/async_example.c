#include <stdio.h>
#include <async/async.h>
#include <async/event.h>
#include <async/looper.h>


char hello_world(async_event_t e) {
    int *p = async_event_get_data(e);
    printf("%s(%d): Hello %d, now is %u\n", __func__, __LINE__, (*p)++, async_get_time());
    return 1;
}

char hello_after_immediatly(async_event_t e) {
    static int i = 0;
    printf("%s(%d): Hello %d, now is %u\n", __func__, __LINE__, i++, async_get_time());
    return i <= 10;
}

char hello_immediatly(async_event_t e) {
    async_event_set_callback(e, hello_after_immediatly);
    async_event_set_timeout(e, 1200);
    hello_after_immediatly(e);
    return 1;
}

int main(int argc, char **argv) {
    static int i = 0;
    async_init();
    async_looper_t looper = async_looper_create();
    async_event_register(looper, hello_world, 1000, &i);
    async_event_register(looper, hello_immediatly, 0, 0);
    async_looper_loop(looper);
    return 0;
}

