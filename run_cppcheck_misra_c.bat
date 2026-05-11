@echo off
setlocal

if not exist reports mkdir reports

set MISRA_ADDON=C:\Program Files\Cppcheck\addons\misra.py

echo Running normal C/C++ Cppcheck analysis...

cppcheck ^
--enable=all ^
--inconclusive ^
--std=c++17 ^
--force ^
--inline-suppr ^
--check-level=normal ^
-DSTM32F410Rx ^
-D__ARMCC_VERSION=6010050 ^
-D__cplusplus=201703L ^
-IApp\Inc ^
-IConfig\Inc ^
-IDrivers\Inc ^
-ILegacy\Inc ^
-IServices\Inc ^
-ICommon\Inc ^
-IDrivers\CMSIS\Include ^
-IDrivers\CMSIS\Device\ST\STM32F4xx\Include ^
-IRTE\Device\STM32F410RBTx ^
--suppress=missingIncludeSystem ^
--template=gcc ^
--output-file=reports\cppcheck_report.txt ^
App Config Drivers Legacy Services RTE

echo.
echo Running MISRA-C analysis for C files only...

cppcheck ^
--enable=all ^
--inconclusive ^
--std=c99 ^
--language=c ^
--force ^
--inline-suppr ^
--check-level=normal ^
--addon="%MISRA_ADDON%" ^
-DSTM32F410Rx ^
-D__ARMCC_VERSION=6010050 ^
-IConfig\Inc ^
-IDrivers\CMSIS\Include ^
-IDrivers\CMSIS\Device\ST\STM32F4xx\Include ^
-IRTE\Device\STM32F410RBTx ^
--suppress=missingIncludeSystem ^
--template=gcc ^
--output-file=reports\misra_c_report.txt ^
Config\Src\config_manager.c RTE\Device\STM32F410RBTx\system_stm32f4xx.c

echo.
echo Running XML C/C++ report...

cppcheck ^
--enable=all ^
--inconclusive ^
--std=c++17 ^
--force ^
--inline-suppr ^
--check-level=normal ^
-DSTM32F410Rx ^
-D__ARMCC_VERSION=6010050 ^
-D__cplusplus=201703L ^
-IApp\Inc ^
-IConfig\Inc ^
-IDrivers\Inc ^
-ILegacy\Inc ^
-IServices\Inc ^
-ICommon\Inc ^
-IDrivers\CMSIS\Include ^
-IDrivers\CMSIS\Device\ST\STM32F4xx\Include ^
-IRTE\Device\STM32F410RBTx ^
--suppress=missingIncludeSystem ^
--xml ^
--xml-version=2 ^
App Config Drivers Legacy Services RTE 2> reports\cppcheck_report.xml

echo.
echo =========================================================
echo Cppcheck finished.
echo C/C++ report : reports\cppcheck_report.txt
echo MISRA-C      : reports\misra_c_report.txt
echo XML report   : reports\cppcheck_report.xml
echo =========================================================
echo.

pause