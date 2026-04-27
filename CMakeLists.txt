cmake_minimum_required(VERSION 3.20)

project(realtime_industrial_sensor C CXX ASM)

option(ENABLE_FREERTOS "Build firmware with FreeRTOS scheduler" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

set(PROJECT_SOURCES
    App/Src/main.cpp
    App/Src/Application.cpp
    App/Src/ConfigManager.cpp

    Services/Src/AdcService.cpp
    Services/Src/ErrorManager.cpp
    Services/Src/FlashStorage.cpp
    Services/Src/ModbusService.cpp
    Services/Src/SensorService.cpp
    Services/Src/WatchdogService.cpp

    Drivers/Src/DelayTimer.cpp
    Drivers/Src/I2cBus.cpp
    Drivers/Src/LcdDisplay.cpp
    Drivers/Src/PwmOutput.cpp
    Drivers/Src/Sht3xSensor.cpp
    Drivers/Src/SystemClockFacade.cpp

    Config/Src/config_manager.c
    Config/Src/structs.cpp

    Legacy/Src/filter.cpp
    Legacy/Src/flash_write.cpp
    Legacy/Src/i2c.cpp
    Legacy/Src/interrupts.cpp
    Legacy/Src/sysclock.cpp
    Legacy/Src/uart.cpp

    RTE/Device/STM32F410RBTx/system_stm32f4xx.c
    RTE/Device/STM32F410RBTx/startup_stm32f410rx_gcc.s
)

if(ENABLE_FREERTOS)
    set(FREERTOS_KERNEL_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/FreeRTOS-Kernel)

    set(FREERTOS_SOURCES
        ${FREERTOS_KERNEL_DIR}/tasks.c
        ${FREERTOS_KERNEL_DIR}/queue.c
        ${FREERTOS_KERNEL_DIR}/list.c
        ${FREERTOS_KERNEL_DIR}/timers.c
        ${FREERTOS_KERNEL_DIR}/event_groups.c
        ${FREERTOS_KERNEL_DIR}/portable/GCC/ARM_CM4F/port.c
        ${FREERTOS_KERNEL_DIR}/portable/MemMang/heap_4.c

        Rtos/Src/RtosTaskManager.cpp
        Rtos/Src/RtosHooks.c
    )

    list(APPEND PROJECT_SOURCES ${FREERTOS_SOURCES})
endif()

add_executable(${PROJECT_NAME}.elf ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${CMAKE_SOURCE_DIR}

    ${CMAKE_SOURCE_DIR}/App/Inc
    ${CMAKE_SOURCE_DIR}/Services/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/Inc
    ${CMAKE_SOURCE_DIR}/Legacy/Inc
    ${CMAKE_SOURCE_DIR}/Config/Inc
    ${CMAKE_SOURCE_DIR}/Common/Inc
    ${CMAKE_SOURCE_DIR}/Rtos/Inc

    ${CMAKE_SOURCE_DIR}/RTE
    ${CMAKE_SOURCE_DIR}/RTE/Device/STM32F410RBTx

    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)

if(ENABLE_FREERTOS)
    target_include_directories(${PROJECT_NAME}.elf PRIVATE
        ${FREERTOS_KERNEL_DIR}/include
        ${FREERTOS_KERNEL_DIR}/portable/GCC/ARM_CM4F
    )
endif()

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32F410Rx
)

if(ENABLE_FREERTOS)
    target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
        ENABLE_FREERTOS=1
    )
endif()

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -Os
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -T${CMAKE_SOURCE_DIR}/Project/stm32f410.ld
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
    -specs=nano.specs
    -specs=nosys.specs
)

set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    OUTPUT_NAME "firmware"
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-size $<TARGET_FILE:${PROJECT_NAME}.elf>

    COMMAND arm-none-eabi-objcopy
    -O ihex
    $<TARGET_FILE:${PROJECT_NAME}.elf>
    ${CMAKE_BINARY_DIR}/firmware.hex

    COMMAND arm-none-eabi-objcopy
    -O binary
    $<TARGET_FILE:${PROJECT_NAME}.elf>
    ${CMAKE_BINARY_DIR}/firmware.bin
)