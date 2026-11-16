# SPDX-License-Identifier: MIT

ifndef BSP_SRC
$(error BSP_SRC must point to the FreeRTOS-Demo-OE-BSP source directory)
endif

APP_OBJECTS = main.o
include $(BSP_SRC)/freertos-bsp.mk
