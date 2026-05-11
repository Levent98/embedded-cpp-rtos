# Realtime Industrial Sensor Firmware  
STM32F4 • Bare-Metal • Modern C++ • Industrial Communication

This project implements a real-time industrial temperature and humidity monitoring system on an STM32F4 microcontroller. The firmware is written in modern C++ using a layered architecture and operates without an RTOS (bare-metal).

The system acquires sensor data over I2C, processes it deterministically, and exposes it through Modbus RTU (RS485) for integration with industrial monitoring and SCADA systems.

---

# Key Features

- Register-level peripheral drivers (I2C, UART, ADC, DMA, PWM)
- Modbus RTU implementation over RS485
- SHT3x temperature and humidity sensor integration
- Deterministic main loop timing and cycle-count profiling
- Flash-based persistent configuration storage with CRC validation
- Watchdog-based fault detection and recovery
- Layered and maintainable software architecture
- CMake-based multi-toolchain build system
- Host-side unit testing support
- Automated CI pipeline with GitHub Actions
- Dockerized reproducible build environment
- Static analysis and stack usage validation

---

# System Architecture

The project follows a modular layered architecture to improve maintainability, portability, and scalability.

```text
App/
 ├── Application entry point
 ├── Main orchestration logic

Services/
 ├── Modbus RTU service
 ├── Sensor service
 ├── Watchdog service
 ├── Flash/configuration service
 ├── Error management

Drivers/
 ├── I2C driver
 ├── UART/RS485 driver
 ├── ADC driver
 ├── PWM driver
 ├── System clock abstraction

Legacy/
 ├── Low-level C modules
 ├── Hardware compatibility layer

Config/
 ├── Persistent configuration structures
 ├── Flash configuration APIs

Common/
 ├── Shared utilities
 ├── Common definitions
```

---

# Hardware

| Component | Description |
|---|---|
| MCU | STM32F410 (Cortex-M4) |
| Communication | RS485 / Modbus RTU |
| Sensor | SHT3x (I2C interface) |

---

# Build System

The firmware uses a CMake-based build system and supports:

- ARMClang (Keil)
- GNU Arm Embedded Toolchain
- Ninja build generator

<p align="center">
  <img src="/images/build_process.png" width="700">
</p>

---

# Native Build

## Configure

```bash
cmake -B build -G Ninja \
-DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake
```

## Build

```bash
cmake --build build
```

---

# Host-Side Unit Tests

The project includes host-side unit tests that validate firmware logic without requiring STM32 hardware.

## Covered Logic

- Median filter behavior
- Configuration management logic
- Error escalation and recovery logic
- SHT3x conversion routines
- Modbus RTU frame handling and CRC validation

## Configure Tests

```bash
cmake -S Tests -B build-tests -G Ninja
```

## Build Tests

```bash
cmake --build build-tests
```

## Run Tests

```bash
ctest --test-dir build-tests --output-on-failure
```

---

# CI/CD Pipeline

The project uses GitHub Actions for automated validation.

## Continuous Integration Features

- Automated firmware builds
- Automated host-side unit tests
- Static analysis using cppcheck
- Stack usage analysis
- Firmware artifact generation
- Dockerized build verification

## CI Pipeline

```text
Git Push
   ↓
GitHub Actions
   ↓
Firmware Build
   ↓
Static Analysis
   ↓
Stack Usage Validation
   ↓
Host-Side Unit Tests
   ↓
Dockerized Build & Test
   ↓
Artifact Generation
```

---

# Dockerized Build Environment

The repository includes a Docker-based reproducible build environment.

This allows contributors to build and test the firmware in a standardized Linux environment independent of the host operating system.

## Build Docker Image

```bash
docker build -t stm32-fw-ci .
```

## Build Firmware Inside Docker

```bash
docker run --rm -v "$(pwd):/workspace" stm32-fw-ci \
cmake -B build-docker -G Ninja \
-DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake
```

```bash
docker run --rm -v "$(pwd):/workspace" stm32-fw-ci \
cmake --build build-docker
```

<p align="center">
  <img src="/images/build_process.png" width="700">
</p>


---

# Static Analysis

The project integrates automated static analysis using:

- cppcheck
- Stack usage analysis (`-fstack-usage`)
- CI validation gates

This helps identify:

- excessive stack usage
- portability issues
- potential logic problems
- risky embedded memory patterns

---

# Current Validation Coverage

## Firmware Validation

- Firmware compilation
- Linker validation
- Artifact generation
- Stack usage inspection

## Communication Validation

- Modbus RTU CRC validation
- Frame parsing
- Invalid frame rejection
- Wrong slave handling

## Sensor Validation

- Temperature conversion
- Humidity conversion
- Filtering logic

## System Validation

- Error escalation logic
- Recovery handling
- Persistent configuration behavior

---

# Future Improvements

- Hardware-in-the-loop (HIL) testing
- Automated STM32 flashing
- UART/RS485 integration tests
- Coverage reporting
- Release automation
- Embedded performance benchmarking

---

# License

This project is intended for educational, embedded systems, and industrial firmware development purposes.

