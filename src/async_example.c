#include <stdio.h>
#include <async/async.h>
#include <async/event.h>
#include <async/looper.h>


char hello_world(async_event_t e) {
    int *p = async_event_get_data(e);
    printf("hello world: %d, now is %d\n", (*p)++, async_get_time());
    if ((*p) > 3) {
        async_looper_exit(async_event_get_looper(e));
    }
    return 1;
}

int main(int argc, char **argv) {
    static int i = 0;
    async_init();
    async_looper_t looper = async_looper_create();
    async_event_register(looper, hello_world, 1000, &i);
    async_looper_loop(looper);
    return 0;
}

