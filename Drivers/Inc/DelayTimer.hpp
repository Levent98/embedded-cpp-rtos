#ifndef DELAY_TIMER_HPP
#define DELAY_TIMER_HPP

#include <stdint.h>

class DelayTimer
{
public:
    DelayTimer() noexcept = default;
    ~DelayTimer() = default;

    DelayTimer(const DelayTimer&) = delete;
    DelayTimer& operator=(const DelayTimer&) = delete;

    DelayTimer(DelayTimer&&) = delete;
    DelayTimer& operator=(DelayTimer&&) = delete;

    static void initialize();
    static void delayUs(uint16_t us);
    static void delayMs(uint16_t ms);
    static uint32_t tickMs();
    static void onSysTickInterrupt();

private:
    static volatile uint32_t msTicks_;
};

#endif