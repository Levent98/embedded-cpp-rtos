@echo off
setlocal

if not exist reports mkdir reports

set PYTHON_EXEC=C:\Users\Arge1\AppData\Local\Programs\Python\Python313\python.exe
set MISRA_ADDON=C:\Program Files\Cppcheck\addons\misra.py

echo Checking Python...
"%PYTHON_EXEC%" --version

echo.
echo Running Cppcheck text report with MISRA addon...

cppcheck ^
--enable=all ^
--inconclusive ^
--std=c++17 ^
--force ^
--inline-suppr ^
--check-level=normal ^
--addon="%MISRA_ADDON%" ^
-DSTM32F410Rx ^
-D__ARMCC_VERSION=6010050 ^
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
echo Running Cppcheck XML report with MISRA addon...

cppcheck ^
--enable=all ^
--inconclusive ^
--std=c++17 ^
--force ^
--inline-suppr ^
--check-level=normal ^
--addon="%MISRA_ADDON%" ^
-DSTM32F410Rx ^
-D__ARMCC_VERSION=6010050 ^
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
echo Text report: reports\cppcheck_report.txt
echo XML report : reports\cppcheck_report.xml
echo =========================================================
echo.

pause