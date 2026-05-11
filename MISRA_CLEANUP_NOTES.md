# MISRA-oriented cleanup notes

This project is MISRA-aware, not fully MISRA-certified. The changes in this archive target project-owned code only. CMSIS/ST device headers still intentionally use memory-mapped register macros and C-style casts; those should be handled as documented deviations rather than rewritten.

## Applied changes

- Replaced build-date macros in `App/Inc/main.hpp` with `constexpr` constants/functions.
- Replaced `CONFIG_FLASH_ADDR` and `CONFIG_MAGIC_NUM` macros in `Config/Inc/Config_Manager.h` with an enum constant block for C/C++ compatibility.
- Replaced floating-point macros in `Config/Inc/appcons.h` with `static const float` constants.
- Removed local `TRUE`, `FALSE`, `RESET`, `SET` macros from `Services/Src/ModbusService.cpp` and used explicit values.
- Changed C++ source/header includes from C library headers to C++ headers where practical: `<cstring>`, `<cstddef>`, `<cstdint>`.
- Replaced C library calls in C++ files with `std::memset`, `std::memcpy`, and `std::memcmp` where practical.
- Centralized unavoidable DMA pointer-to-integer conversions through `dmaAddressOf()` helpers with NOLINT justification comments.
- Added flash CRC layout checks in `ConfigManager.cpp`:
  - CRC must be the final member of `FlashConfig_t`.
  - `FlashConfig_t` must contain a CRC field.
- Converted `MedianFilter` internal buffers from C arrays to `std::array`.

## Remaining intentional deviations

- CMSIS register access, e.g. `RCC->CSR`, `TIM6->CNT`, `GPIOC->BSRR`, still produces fixed-address/C-style-cast analyzer warnings. These are normal for STM32 memory-mapped IO.
- Flash memory access still requires carefully documented reinterpret casts.
- DMA register address programming still requires pointer-to-integer conversion.
- Some C-compatible legacy headers still use C types and C linkage for compatibility with `.c` files.
- Existing legacy enum types remain unscoped to avoid breaking Modbus register maps, flash layout, and C interoperability.

## Build note

A host-side syntax check was run on representative C++ files. It only reported CMSIS host-size warnings and existing packed-member warnings around `FlashConfig_t`; no syntax errors were found in the modified files. Please rebuild with ARMClang/Keil on the target project.
