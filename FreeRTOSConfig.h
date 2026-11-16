/* SPDX-License-Identifier: MIT */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "FreeRTOSBSPConfig.h"

#define configUSE_PREEMPTION 1
#define configUSE_IDLE_HOOK 1
#define configUSE_TICK_HOOK 0
#define configTICK_RATE_HZ 100
#define configMAX_PRIORITIES 5
#define configMINIMAL_STACK_SIZE 256
#define configTOTAL_HEAP_SIZE (128 * 1024)
#define configMAX_TASK_NAME_LEN 16
#define configUSE_16_BIT_TICKS 0
#define configIDLE_SHOULD_YIELD 1
#define configUSE_TIMERS 0
#define configUSE_TASK_FPU_SUPPORT 2
#define configCHECK_FOR_STACK_OVERFLOW 2
#define configUSE_MALLOC_FAILED_HOOK 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_xTaskDelayUntil 1

#endif
