#ifndef WATCHDOG_SERVICE_HPP
#define WATCHDOG_SERVICE_HPP

#include <stdint.h>

class WatchdogService
{
public:
    WatchdogService();
    
    ~WatchdogService() = default;
    
    WatchdogService(const WatchdogService&) = delete;
    WatchdogService& operator=(const WatchdogService&) = delete;
    
    WatchdogService(WatchdogService&&) = delete;
    WatchdogService& operator=(WatchdogService&&) = delete;

    void initialize() const;
    static void kick();

private:
    static constexpr uint16_t ReloadValue = 999U;
    static constexpr uint8_t Prescaler = 0x04U; // /64

    static void waitForUpdate();
};

#endif