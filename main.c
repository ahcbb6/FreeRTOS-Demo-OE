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
    char line[128];
    unsigned int length = 0;
    int previous_cr = 0;

    (void)argument;
    uart_puts("Waiting For Notification - Blocked...\r\n");
    for (;;) {
        int value = uart_getc();

        if (value >= 0) {
            taskENTER_CRITICAL();
            if (value == '\n' && previous_cr) {
                previous_cr = 0;
            } else {
                previous_cr = value == '\r';
                if (value == '\r' || value == '\n') {
                    line[length] = '\0';
                    uart_puts("\r\nYou entered: \"");
                    uart_puts(line);
                    uart_puts("\"\r\nUnblocked\r\nWaiting For Notification - Blocked...\r\n");
                    length = 0;
                } else if (value == '\b' || value == 0x7f) {
                    if (length > 0) {
                        --length;
                        uart_puts("\b \b");
                    }
                } else if (value >= ' ' && value <= '~' &&
                           length < sizeof(line) - 1) {
                    line[length++] = value;
                    uart_putc(value);
                }
            }
            taskEXIT_CRITICAL();
        }
        vTaskDelay(1);
    }
}

static void task1(void *argument)
{
    (void)argument;
    for (;;) {
        taskENTER_CRITICAL();
        uart_puts("Task1\r\n");
        taskEXIT_CRITICAL();
        vTaskDelay(pdMS_TO_TICKS(2000));
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
    configASSERT(xTaskCreate(task1, "task1", 512, NULL, 1, NULL) == pdPASS);
    configASSERT(xTaskCreate(periodic_task, "periodic", 512, NULL, 1, NULL) == pdPASS);
    vTaskStartScheduler();
    vPlatformAssert("scheduler returned", 0);
}
