# FreeRTOS Kernel placeholder

Place the FreeRTOS Kernel source tree here before building with `-DENABLE_FREERTOS=ON`.

Expected files include:

```text
ThirdParty/FreeRTOS-Kernel/tasks.c
ThirdParty/FreeRTOS-Kernel/queue.c
ThirdParty/FreeRTOS-Kernel/list.c
ThirdParty/FreeRTOS-Kernel/timers.c
ThirdParty/FreeRTOS-Kernel/event_groups.c
ThirdParty/FreeRTOS-Kernel/portable/RVDS/ARM_CM4F/port.c
ThirdParty/FreeRTOS-Kernel/portable/MemMang/heap_4.c
ThirdParty/FreeRTOS-Kernel/include/FreeRTOS.h
```

Recommended source: official FreeRTOS-Kernel repository.
