# FreeRTOS Integration Plan

This branch adds a production-oriented FreeRTOS integration path while preserving the existing bare-metal build.

## Build Modes

Bare-metal build remains the default:

```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/armclang-keil.cmake
cmake --build build
```

FreeRTOS build:

```bash
cmake -S . -B build-rtos -G Ninja -DENABLE_FREERTOS=ON -DCMAKE_TOOLCHAIN_FILE=cmake/armclang-keil.cmake
cmake --build build-rtos
```

## Required FreeRTOS Layout

Place the official FreeRTOS Kernel source tree under:

```text
ThirdParty/FreeRTOS-Kernel/
```

Required files include:

```text
ThirdParty/FreeRTOS-Kernel/tasks.c
ThirdParty/FreeRTOS-Kernel/queue.c
ThirdParty/FreeRTOS-Kernel/list.c
ThirdParty/FreeRTOS-Kernel/timers.c
ThirdParty/FreeRTOS-Kernel/event_groups.c
ThirdParty/FreeRTOS-Kernel/include/FreeRTOS.h
ThirdParty/FreeRTOS-Kernel/portable/RVDS/ARM_CM4F/port.c
ThirdParty/FreeRTOS-Kernel/portable/MemMang/heap_4.c
```

## Task Model

The RTOS build uses the following task split:

```text
ModbusTask   Priority 4   Continuous, highest priority
SensorTask   Priority 3   Periodic sensor processing
AdcTask      Priority 2   Periodic ADC processing
HealthTask   Priority 1   Error recovery and watchdog supervision
```

## Watchdog Strategy

The watchdog is fed only by the HealthTask. HealthTask checks that the Modbus, Sensor, and ADC tasks have reported heartbeat progress within a supervision window before feeding the watchdog.

This prevents the watchdog from being fed if a critical task stalls.

## SysTick Ownership

When FreeRTOS is enabled, SysTick belongs to the FreeRTOS kernel. Existing microsecond timing based on TIM/DWT can remain independent, but blocking delay calls should not be used inside RTOS tasks.

## Migration Notes

This integration intentionally keeps the bare-metal execution path intact. RTOS functionality is enabled only through:

```text
-DENABLE_FREERTOS=ON
```

This allows safe incremental migration and easy regression testing against the original bare-metal firmware.
