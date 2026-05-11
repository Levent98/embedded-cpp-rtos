@echo off
setlocal enabledelayedexpansion

:: Proje Klasör Yapılandırması
set BUILD_DIR=build
set TOOLCHAIN_FILE=%cd%\cmake\gcc-arm-none-eabi.cmake

echo ========================================
echo [1/4] Eski Build Klasoru Temizleniyor...
echo ========================================
if exist %BUILD_DIR% (
    rmdir /s /q %BUILD_DIR%
)
mkdir %BUILD_DIR%

echo.
echo ========================================
echo [2/4] CMake Yapilandirmasi Baslatiliyor...
echo ========================================
cmake -B %BUILD_DIR% -G Ninja -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAIN_FILE%"
if %errorlevel% neq 0 (
    echo [HATA] CMake yapilandirmasi basarisiz!
    pause
    exit /b %errorlevel%
)

echo.
echo ========================================
echo [3/4] PC-Lint Analizi Calistiriliyor...
echo ========================================
cmake --build %BUILD_DIR% --target pclint
if %errorlevel% neq 0 (
    echo [UYARI] PC-Lint bazi hatalar buldu veya calisirken sorun yasadi.
    echo Sonuclari 'lint_results.txt' dosyasindan kontrol edebilirsiniz.
)

echo.
echo ========================================
echo [4/4] Proje Derleniyor (Fimware Build)...
echo ========================================
cmake --build %BUILD_DIR%
if %errorlevel% neq 0 (
    echo [HATA] Derleme islemi basarisiz!
    pause
    exit /b %errorlevel%
)

:: ... önceki komutlar (cmake build vb.) ...

echo.
echo ========================================
echo [5/5] Stack Kullanim Analizi Toplaniyor...
echo ========================================
:: Eski dosyayı temizle
if exist all_stack.txt del all_stack.txt

:: .su dosyalarını tara ve all_stack.txt içine yaz
:: NOT: .bat içinde %%f kullanılması zorunludur!
for /r %%f in (*.su) do (
    echo ===== %%f ===== >> all_stack.txt
    type "%%f" >> all_stack.txt
)

echo ISLEM TAMAMLANDI!
echo Stack raporu: all_stack.txt
echo ========================================
pause

echo.
echo ========================================
echo ISLEM TAMAMLANDI!
echo ========================================
pause