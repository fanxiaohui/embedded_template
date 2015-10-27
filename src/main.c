/*#include <stdio.h>
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
*/

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "FreeRTOS.h"
#include "task.h"

static void  prvInitialiseHeap(void);

void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationTickHook(void);

#define mainCHECK_TASK_PRIORITY 10
void vAssertCalled( unsigned long ulLine, const char * const pcFileName ) {
}

static void task(void *data) {
    while (1) {
        printf("Hello world\n");
        fflush(stdout);
        vTaskDelay(configTICK_RATE_HZ);
    }
}

int main(void) {
    prvInitialiseHeap();

    printf( "\r\nHello, this is a demo.\n");
    fflush( stdout );
    xTaskCreate( task, "task", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
    vTaskStartScheduler();
    return 0;
}
void vApplicationMallocFailedHook(void) {
}
void vApplicationIdleHook(void) {
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
    (void)pcTaskName;
    (void)pxTask;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {
}

#define mainREGION_1_SIZE 12*1024
#define mainREGION_2_SIZE 5*1024
#define mainREGION_3_SIZE 20*1024
static void  prvInitialiseHeap(void) {
    static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
    volatile uint32_t ulAdditionalOffset = 19;
    const HeapRegion_t xHeapRegions[] = {
        /* Start address with dummy offsets						Size */
        { ucHeap + 1,											mainREGION_1_SIZE },
        { ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
        { ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
        { NULL, 0 }
    };

    vPortDefineHeapRegions(xHeapRegions);
}
/*-----------------------------------------------------------*/


