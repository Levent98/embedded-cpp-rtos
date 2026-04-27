#include "UartDriver.hpp"
#include "DelayTimer.hpp"

#ifndef ENABLE_FREERTOS
extern "C" void SysTick_Handler(void)
{
    DelayTimer::onSysTickInterrupt();
}
#endif
