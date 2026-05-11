#ifndef SPIBUS_HPP
#define SPIBUS_HPP

#include "stm32f4xx.h"
#include <stdint.h>

class SpiBus
{
public:
    SpiBus();
    SpiBus(const SpiBus&) = delete;
    SpiBus& operator=(const SpiBus&) = delete;
    SpiBus(SpiBus&&) = delete;
    SpiBus& operator=(SpiBus&&) = delete;

    void init();
    static uint8_t transfer(uint8_t data);
    void transfer(uint8_t* tx, uint8_t* rx, uint16_t len);

    static void csLow();
    static void csHigh();

private:
    void gpioInit();
    static void spiInit();
};

#endif