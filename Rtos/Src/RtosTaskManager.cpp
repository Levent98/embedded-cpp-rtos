#include "RtosTaskManager.hpp"
#include "RtosConfig.hpp"

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

RtosTaskManager::RtosTaskManager(Application& application)
    : application_(application), heartbeatMask_(0U)
{
}

void RtosTaskManager::start()
{
    xTaskCreate(modbusTaskEntry,
                "Modbus",
                RtosConfig::ModbusTaskStackWords,
                this,
                4U,
                nullptr);

    xTaskCreate(sensorTaskEntry,
                "Sensor",
                RtosConfig::SensorTaskStackWords,
                this,
                3U,
                nullptr);

    xTaskCreate(adcTaskEntry,
                "ADC",
                RtosConfig::AdcTaskStackWords,
                this,
                2U,
                nullptr);

    xTaskCreate(healthTaskEntry,
                "Health",
                RtosConfig::HealthTaskStackWords,
                this,
                1U,
                nullptr);

    vTaskStartScheduler();

    while (1) {
        // Scheduler should not return. If it does, heap/stack configuration is invalid.
    }
}

void RtosTaskManager::setHeartbeat(uint32_t bit)
{
    taskENTER_CRITICAL();
    heartbeatMask_ |= bit;
    taskEXIT_CRITICAL();
}

uint32_t RtosTaskManager::consumeHeartbeatMask()
{
    taskENTER_CRITICAL();
    const uint32_t mask = heartbeatMask_;
    heartbeatMask_ = 0U;
    taskEXIT_CRITICAL();
    return mask;
}

void RtosTaskManager::modbusTaskEntry(void* context)
{
    static_cast<RtosTaskManager*>(context)->modbusTask();
}

void RtosTaskManager::sensorTaskEntry(void* context)
{
    static_cast<RtosTaskManager*>(context)->sensorTask();
}

void RtosTaskManager::adcTaskEntry(void* context)
{
    static_cast<RtosTaskManager*>(context)->adcTask();
}

void RtosTaskManager::healthTaskEntry(void* context)
{
    static_cast<RtosTaskManager*>(context)->healthTask();
}

void RtosTaskManager::modbusTask()
{
    TickType_t lastWake = xTaskGetTickCount();

    while (1) {
        application_.processModbusTask();
        setHeartbeat(RtosConfig::ModbusHeartbeat);
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(RtosConfig::ModbusTaskPeriodMs));
    }
}

void RtosTaskManager::sensorTask()
{
    TickType_t lastWake = xTaskGetTickCount();

    while (1) {
        application_.processSensorTask();
        setHeartbeat(RtosConfig::SensorHeartbeat);
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(RtosConfig::SensorTaskPeriodMs));
    }
}

void RtosTaskManager::adcTask()
{
    TickType_t lastWake = xTaskGetTickCount();

    while (1) {
        application_.processAdcTask();
        setHeartbeat(RtosConfig::AdcHeartbeat);
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(RtosConfig::AdcTaskPeriodMs));
    }
}

void RtosTaskManager::healthTask()
{
    TickType_t lastWake = xTaskGetTickCount();
    uint32_t elapsedMs = 0U;
    bool allTasksHealthy = true;

    while (1) {
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(RtosConfig::HealthTaskPeriodMs));

        elapsedMs += RtosConfig::HealthTaskPeriodMs;

        if (elapsedMs >= RtosConfig::HealthWindowMs) {
            elapsedMs = 0U;

            const uint32_t expected = RtosConfig::ModbusHeartbeat |
                                      RtosConfig::SensorHeartbeat |
                                      RtosConfig::AdcHeartbeat;

            allTasksHealthy = ((consumeHeartbeatMask() & expected) == expected);
        }

        application_.processHealthTask(allTasksHealthy);
    }
}
