#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "SystemClock.hpp"
#include "DelayTimer.hpp"
#include "UartDriver.hpp"
#include "I2cBus.hpp"
#include "Sht3xSensor.hpp"
#include "LcdDisplay.hpp"
#include "PwmOutput.hpp"
#include "AdcService.hpp"
#include "ModbusService.hpp"
#include "WatchdogService.hpp"
#include "ErrorManager.hpp"
#include "FlashStorage.hpp"
#include "ConfigManager.hpp"
#include "SensorService.hpp"
#include "SpiBus.hpp"
#include "main.hpp"
class Application
{
public:
    explicit Application();
   ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;
    void initialize();
    void run();

private:
    void showStartupScreen() const;

    SystemClock systemClock_;
    UartDriver uart_;
    I2cBus i2c_;
    SpiBus spi_;
    Sht3xSensor sensor_;
    LcdDisplay lcd_;
    PwmOutput pwm_;
    ErrorManager errors_;
    ModbusService modbus_;
    AdcService adc_;
    WatchdogService watchdog_;
    FlashStorage flash_;
    ConfigManager config_;
    SensorService sensorService_;
};

#endif