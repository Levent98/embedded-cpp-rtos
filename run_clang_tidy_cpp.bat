@echo off
setlocal

if not exist reports mkdir reports

set "CLANG_TIDY=C:\Program Files\LLVM\bin\clang-tidy.exe"
set "ARM_INC=C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.07\arm-none-eabi\include"

set "COMMON_ARGS=-std=c++17 -DSTM32F410Rx -D__GNUC__ -D__GNUC_MINOR__=3 -D__GNUC_PATCHLEVEL__=1 -IApp\Inc -IConfig\Inc -IDrivers\Inc -ILegacy\Inc -IServices\Inc -ICommon\Inc -IDrivers\CMSIS\Include -IDrivers\CMSIS\Core\Include -IDrivers\CMSIS\Device\ST\STM32F4xx\Include -IRTE\Device\STM32F410RBTx -isystem "%ARM_INC%""

set "CHECKS=bugprone-*,performance-*,modernize-*,clang-analyzer-*,cppcoreguidelines-*,hicpp-*,-hicpp-use-auto,-modernize-use-trailing-return-type,-cppcoreguidelines-avoid-magic-numbers,-cppcoreguidelines-macro-usage,-modernize-macro-to-enum,-cppcoreguidelines-avoid-c-arrays,-modernize-avoid-c-arrays,-readability-magic-numbers,-readability-identifier-length,-modernize-use-using,-performance-no-int-to-ptr,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-pro-type-reinterpret-cast,-hicpp-no-array-decay"
set "FILTER=^(App|Config|Services|Common|Legacy|Drivers\\Src).*"

echo.
echo =========================================================
echo Running filtered clang-tidy + HICPP analysis...
echo =========================================================
echo.

(
"%CLANG_TIDY%" App\Src\Application.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" App\Src\ConfigManager.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" App\Src\main.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%

"%CLANG_TIDY%" Config\Src\structs.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%

"%CLANG_TIDY%" Drivers\Src\DelayTimer.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Drivers\Src\I2cBus.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Drivers\Src\LcdDisplay.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Drivers\Src\PwmOutput.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Drivers\Src\Sht3xSensor.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Drivers\Src\SpiBus.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Drivers\Src\SystemClockFacade.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%

"%CLANG_TIDY%" Legacy\Src\filter.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Legacy\Src\flash_write.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Legacy\Src\i2c.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Legacy\Src\interrupts.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Legacy\Src\sysclock.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Legacy\Src\uart.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%

"%CLANG_TIDY%" Services\Src\AdcService.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Services\Src\ErrorManager.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Services\Src\FlashStorage.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Services\Src\ModbusService.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Services\Src\SensorService.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%
"%CLANG_TIDY%" Services\Src\WatchdogService.cpp -header-filter="%FILTER%" -checks=%CHECKS% -- %COMMON_ARGS%

) > reports\clang_tidy_cpp_report.txt 2>&1

echo.
echo =========================================================
echo clang-tidy finished.
echo Filtered report:
echo reports\clang_tidy_cpp_report.txt
echo =========================================================
echo.

pause