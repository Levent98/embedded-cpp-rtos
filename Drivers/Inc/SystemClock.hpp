#ifndef SYSTEM_CLOCK_HPP
#define SYSTEM_CLOCK_HPP

#include <stdint.h>

class SystemClock
{
public:
    SystemClock();
    SystemClock(const SystemClock&) = delete;
    SystemClock& operator=(const SystemClock&) = delete;
    SystemClock(SystemClock&&) = delete;
    SystemClock& operator=(SystemClock&&) = delete;

    void configure();
    uint32_t coreFrequencyHz() const;

private:
    uint32_t coreFrequencyHz_;

    static constexpr uint32_t kClockTimeout = 0xFFFFU;

    static bool enableHse();
    static bool configurePllFromHse24MHz();
    static void configureFlashLatency();
    static void configureBusPrescalers();
    static void switchSystemClockToPll();
};

#endif