#ifndef RTOS_TASK_MANAGER_HPP
#define RTOS_TASK_MANAGER_HPP

#include <stdint.h>
#include "Application.hpp"

class RtosTaskManager {
public:
    explicit RtosTaskManager(Application& application);

    void start();

private:
    Application& application_;
    volatile uint32_t heartbeatMask_;

    void setHeartbeat(uint32_t bit);
    uint32_t consumeHeartbeatMask();

    static void modbusTaskEntry(void* context);
    static void sensorTaskEntry(void* context);
    static void adcTaskEntry(void* context);
    static void healthTaskEntry(void* context);

    void modbusTask();
    void sensorTask();
    void adcTask();
    void healthTask();
};

#endif
