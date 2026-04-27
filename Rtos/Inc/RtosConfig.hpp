#ifndef RTOS_CONFIG_HPP
#define RTOS_CONFIG_HPP

#include <stdint.h>

namespace RtosConfig {

static constexpr uint16_t ModbusTaskStackWords = 512U;
static constexpr uint16_t SensorTaskStackWords = 512U;
static constexpr uint16_t AdcTaskStackWords = 256U;
static constexpr uint16_t HealthTaskStackWords = 256U;

static constexpr uint32_t ModbusTaskPeriodMs = 1U;
static constexpr uint32_t SensorTaskPeriodMs = 10U;
static constexpr uint32_t AdcTaskPeriodMs = 100U;
static constexpr uint32_t HealthTaskPeriodMs = 250U;

static constexpr uint32_t HealthWindowMs = 2000U;

static constexpr uint32_t ModbusHeartbeat = 0x01UL;
static constexpr uint32_t SensorHeartbeat = 0x02UL;
static constexpr uint32_t AdcHeartbeat = 0x04UL;

}

#endif
