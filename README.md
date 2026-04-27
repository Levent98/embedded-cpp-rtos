# Realtime Industrial Sensor (STM32, Bare-Metal, C++)

This project implements a real-time industrial temperature and humidity monitoring system on an STM32F4 microcontroller. The firmware is written in modern C++ using a layered architecture and runs without an RTOS (bare-metal).

## Overview

The system collects sensor data via I2C, processes it deterministically, and exposes it over Modbus RTU (RS485) for integration with industrial monitoring systems such as SCADA.

## Features

- Register-level peripheral drivers (I2C, UART, ADC, DMA, PWM)
- Modbus RTU protocol implementation (RS485 communication)
- Sensor integration (SHT3x temperature and humidity sensor)
- Deterministic main loop timing using cycle counting
- Flash-based configuration storage with CRC validation
- Watchdog-based fault detection and recovery
- Layered and maintainable software architecture
- CMake-based build system (ARMClang / Keil toolchain)

## System Architecture

The project is organized into modular layers to ensure maintainability and scalability:

- App: Application entry point and orchestration
- Services: Business logic (sensor handling, Modbus, watchdog, flash)
- Drivers: Hardware abstraction and peripheral control
- Legacy: Low-level C drivers kept for compatibility
- Config: Configuration structures and APIs
- Common: Shared utilities


## Hardware

- MCU: STM32F410 (Cortex-M4)
- Communication: RS485 (Modbus RTU)
- Sensor: SHT3x (I2C interface)


## Build System

The project uses CMake with ARMClang (Keil toolchain).

<p align="center">
  <img src="/images/build_process.png" width="700">
</p>

### Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/armclang-keil.cmake
cmake --build build
