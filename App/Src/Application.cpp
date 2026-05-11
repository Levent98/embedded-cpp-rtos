#include "Application.hpp"

extern "C" {
#include "delay.h"
#include "Config_Manager.h"
#include "stm32f410rx.h"
}

Application::Application()
    : systemClock_(),
      uart_(),
      i2c_(),
      spi_(),
      sensor_(i2c_),
      lcd_(),
      pwm_(),
      errors_(),
      modbus_(uart_, errors_),
      adc_(modbus_),
      watchdog_(),
      flash_(),
      config_(flash_),
      sensorService_(sensor_, modbus_, lcd_, pwm_, errors_)
{
}

void Application::initialize()
{
    systemClock_.configure();

    /* Tek delay altyapisi */
    TimConfig();

    (void)config_.loadOrDefaults();

    LcdDisplay::initialize();
    LcdDisplay::clear();

    uart_.initialize(9600U);
    i2c_.initialize();
    pwm_.initialize();
    errors_.initialize();

#if defined(RCC_CSR_IWDGRSTF)
	  // MISRA Rule 8.2.2/8.2.6: C-Style cast ve integral-pointer dönüsümü
    // Kütüphane makrolari yerine güvenli pointer olusturma
    RCC_TypeDef* const rcc_ptr = reinterpret_cast<RCC_TypeDef*>(RCC_BASE);
    if ((rcc_ptr->CSR & RCC_CSR_IWDGRSTF) != 0U) {
        errors_.reportWatchdogReset();
    }
    rcc_ptr->CSR |= RCC_CSR_RMVF;
#endif

    adc_.initialize();
    modbus_.initialize(DeviceConfig.id);
    watchdog_.initialize();
    sensorService_.initialize();

    showStartupScreen();
}

void Application::showStartupScreen() const
{
    lcd_.setCursor(0U, 0U);
    lcd_.writeString("OOP Firmware");
    lcd_.setCursor(1U, 0U);
    lcd_.writeString("STM32F410   ");
    Delay_ms(300U);
    LcdDisplay::clear();
}

void Application::run()
{
    while (true) {
        const uint32_t nowMs = GetTick();

        modbus_.process();
// Rule 8.14.1: Side effects in logical operators (Önceden hesapla)
        const bool devSettingsReq = (SaveDevSettings != 0U);
        const bool sensorInfoReq = (SaveSensorInfo != 0U);

        if (devSettingsReq || sensorInfoReq) {
            if (config_.saveDeviceConfig()) {
                SaveDevSettings = 0U;
                SaveSensorInfo = 0U;
                errors_.reportFlashSaveSuccess();
            } else {
                errors_.reportFlashSaveFailure();
            }
        }

        adc_.process();
        sensorService_.process(nowMs);
        errors_.process();

        if (errors_.recoveryRequested()) {
            const bool recovered = i2c_.recoverBus();
            errors_.reportI2cRecovery(recovered);
            if (recovered) {
                errors_.markRecoveryDone();
            }
        }

        if (errors_.shouldFeedWatchdog()) {
            WatchdogService::kick();
        }
    }
}
