/* SPDX-License-Identifier: MIT */

#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"

void vApplicationMallocFailedHook(void)
{
    vPlatformAssert("heap exhausted", 0);
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *name)
{
    (void)task;
    vPlatformAssert(name, 0);
}

void vApplicationIdleHook(void)
{
    vPlatformIdle();
}

static void echo_task(void *argument)
{
    (void)argument;
    uart_puts("Waiting For Notification - Blocked...\r\n");
    for (;;) {
        int value = uart_getc();
        int printable = value >= ' ' && value <= '~';

        if (printable || value == '\r' || value == '\n') {
            taskENTER_CRITICAL();
            uart_puts("You entered: \"");
            if (printable)
                uart_putc(value);
            uart_puts("\"\r\nUnblocked\r\nWaiting For Notification - Blocked...\r\n");
            taskEXIT_CRITICAL();
        }
        vTaskDelay(1);
    }
}

static void periodic_task(void *argument)
{
    TickType_t wake_time = xTaskGetTickCount();

    (void)argument;
    for (;;) {
        xTaskDelayUntil(&wake_time, pdMS_TO_TICKS(10000));
        taskENTER_CRITICAL();
        uart_puts("Periodic task 10 secs\r\n");
        taskEXIT_CRITICAL();
    }
}

int main(void)
{
    uart_puts("FreeRTOS " tskKERNEL_VERSION_NUMBER " on QEMU virt\r\n");
    configASSERT(xTaskCreate(echo_task, "echo", 512, NULL, 2, NULL) == pdPASS);
    configASSERT(xTaskCreate(periodic_task, "periodic", 512, NULL, 1, NULL) == pdPASS);
    vTaskStartScheduler();
    vPlatformAssert("scheduler returned", 0);
}
