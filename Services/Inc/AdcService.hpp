#ifndef ADC_SERVICE_HPP
#define ADC_SERVICE_HPP

#include <stdint.h>
#include "ModbusService.hpp"

class AdcService {
public:
    explicit AdcService(ModbusService& modbus);
    
    ~AdcService() = default;
    
    AdcService(const AdcService&) = delete;
    AdcService& operator=(const AdcService&) = delete;
    
    AdcService(AdcService&&) = delete;
    AdcService& operator=(AdcService&&) = delete;

    void initialize();
    void process();

    static uint16_t rawChannel3();
    static uint16_t rawChannel4();

private:
    static constexpr float VoltageScale = 0.008864468F;

    ModbusService& modbus_;

    static volatile uint16_t adcBuffer_[2];

    static void configureGpio();
    static void configureDma();
    static void configureAdc();
};

#endif