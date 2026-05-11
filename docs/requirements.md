# Firmware Requirements and Traceability

This document defines the main firmware requirements and maps them to implementation modules and automated tests.

## Requirement Traceability Matrix

| ID | Requirement | Implementation | Verification |
|---|---|---|---|
| REQ-SYS-001 | The firmware shall run as a bare-metal application without requiring an RTOS. | App, Services, Drivers | Firmware CI build |
| REQ-SYS-002 | The firmware shall use a layered architecture separating application logic, services, drivers, configuration, and common utilities. | App/, Services/, Drivers/, Config/, Common/ | Repository structure review |
| REQ-MODBUS-001 | The device shall support Modbus RTU communication over RS485. | ModbusService, UartDriver | test_modbus_service |
| REQ-MODBUS-002 | The device shall reject Modbus frames with invalid CRC. | ModbusService::processFrame | test_modbus_service |
| REQ-MODBUS-003 | The device shall ignore frames addressed to a different slave ID. | ModbusService::processFrame | test_modbus_service |
| REQ-MODBUS-004 | The device shall support reading holding registers. | ModbusService | test_modbus_service |
| REQ-MODBUS-005 | The device shall support writing supported configuration registers. | ModbusService, ConfigManager | test_modbus_service |
| REQ-CONFIG-001 | The firmware shall load default configuration if persistent configuration is invalid. | Config_LoadFromFlash, Config_SetDefaults | test_config_manager |
| REQ-CONFIG-002 | The firmware shall update runtime configuration after valid configuration writes. | Process_BaseSettings, Process_SensorSettings | test_config_manager |
| REQ-CONFIG-003 | The firmware shall validate configuration data before accepting it. | CheckConctSettings | test_config_manager |
| REQ-SENSOR-001 | The firmware shall support SHT3x temperature and humidity measurements. | Sht3xSensor | test_sht3x_conversion |
| REQ-SENSOR-002 | The firmware shall convert raw SHT3x temperature data into scaled engineering units. | Sht3xSensor::temperatureX100 | test_sht3x_conversion |
| REQ-SENSOR-003 | The firmware shall convert raw SHT3x humidity data into scaled engineering units. | Sht3xSensor::humidityX100 | test_sht3x_conversion |
| REQ-FILTER-001 | The firmware shall filter sensor data to reduce transient noise. | MedianFilter | test_median_filter |
| REQ-ERROR-001 | The firmware shall track CRC, UART, and sensor read failures. | ErrorManager | test_error_manager |
| REQ-ERROR-002 | The firmware shall request recovery after repeated recoverable failures. | ErrorManager | test_error_manager |
| REQ-ERROR-003 | The firmware shall enter a fatal fault state after repeated critical communication failures. | ErrorManager | test_error_manager |
| REQ-WDG-001 | The firmware shall support watchdog-based recovery for abnormal runtime behavior. | WatchdogService, ErrorManager | Firmware CI build / planned HIL |
| REQ-FLASH-001 | The firmware shall store persistent configuration in internal flash. | FlashStorage, ConfigManager | test_config_manager / planned HIL |
| REQ-CI-001 | The project shall automatically build firmware on every push to the CI branch. | GitHub Actions | Firmware CI |
| REQ-CI-002 | The project shall run host-side unit tests automatically. | GitHub Actions, Tests/ | unit-tests job |
| REQ-CI-003 | The project shall run static analysis automatically. | cppcheck, GitHub Actions | build job |
| REQ-CI-004 | The project shall validate stack usage artifacts. | scripts/check_stack_usage.py | build job |
| REQ-CI-005 | The project shall provide a reproducible Dockerized build environment. | Dockerfile, .dockerignore | docker-build-test job |

## Current Automated Tests

| Test | Purpose |
|---|---|
| test_median_filter | Validates sensor filtering behavior |
| test_config_manager | Validates configuration defaults, updates, and validation logic |
| test_error_manager | Validates error counters, recovery request, and fatal fault behavior |
| test_sht3x_conversion | Validates raw SHT3x temperature/humidity conversion |
| test_modbus_service | Validates Modbus queueing, CRC rejection, slave ID filtering, register read/write behavior |

## Planned Validation Improvements

| Area | Planned Improvement |
|---|---|
| Hardware-in-the-loop | Flash firmware to STM32 and validate Modbus RTU over real RS485/UART |
| Coverage | Add gcov/lcov coverage reporting for host-side unit tests |
| Release automation | Generate versioned GitHub releases with firmware artifacts |
| Fault injection | Add tests for partial frames, UART overflow, invalid flash configuration, and reset diagnostics |
| Brown-out validation | Validate reset reason handling for BOR, IWDG, POR, and software reset |
| MISRA compliance | Maintain PC-Lint/MISRA reports with documented suppressions |