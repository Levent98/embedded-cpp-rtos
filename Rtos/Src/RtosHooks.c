#include "FreeRTOS.h"
#include "task.h"

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *taskName)
{
    (void)task;
    (void)taskName;
    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}

void vApplicationIdleHook(void)
{
}

void vApplicationTickHook(void)
{
}
