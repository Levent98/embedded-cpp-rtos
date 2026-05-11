@echo off
setlocal EnableExtensions
cd /d "%~dp0"

where cppcheck >nul 2>nul
if errorlevel 1 (
    echo [ERROR] cppcheck.exe was not found in PATH.
    pause
    exit /b 1
)

if not exist reports mkdir reports

set INCLUDES= -IApp\Inc -ICommon\Inc -IConfig\Inc -IDrivers\Inc -IDrivers\CMSIS\Include -IDrivers\CMSIS\Device\ST\STM32F4xx\Include -ILegacy\Inc -IRTE\Device\STM32F410RBTx -IRTE\_Target_1 -IServices\Inc
set DEFINES= -DSTM32F410Rx -D__ARMCC_VERSION=6010050 -D__STATIC_INLINE="static inline" -D__INLINE="inline" -D__IO="volatile" -D__I="volatile const" -D__O="volatile" -D__weak="" -D__packed=""

cppcheck App Config Drivers Legacy RTE Services ^
 --enable=all ^
 --inconclusive ^
 --std=c++17 ^
 --platform=arm32-wchar_t4 ^
 --force ^
 --inline-suppr ^
 --template=gcc ^
 --error-exitcode=1 ^
 --suppress=missingIncludeSystem ^
 --suppress=preprocessorErrorDirective ^
 %INCLUDES% ^
 %DEFINES% ^
 2> reports\cppcheck_strict_report.txt

set RESULT=%ERRORLEVEL%
type reports\cppcheck_strict_report.txt
exit /b %RESULT%
