#include "Sht3xSensor.hpp"
#include <cassert>

I2cBus::I2cBus()
    : initialized_(false)
{
}

void I2cBus::initialize()
{
    initialized_ = true;
}

I2cStatus I2cBus::write(std::uint8_t, const std::uint8_t*, std::size_t)
{
    return I2cStatus::InvalidArgument;
}

I2cStatus I2cBus::read(std::uint8_t, std::uint8_t*, std::size_t)
{
    return I2cStatus::InvalidArgument;
}

I2cStatus I2cBus::writeRead(std::uint8_t,
                            const std::uint8_t*,
                            std::size_t,
                            std::uint8_t*,
                            std::size_t)
{
    return I2cStatus::InvalidArgument;
}

bool I2cBus::recoverBus()
{
    return false;
}

int main()
{
    assert(Sht3xSensor::temperatureX100(0U) == -4500);
    assert(Sht3xSensor::temperatureX100(65535U) == 13000);

    assert(Sht3xSensor::humidityX100(0U) == 0);
    assert(Sht3xSensor::humidityX100(65535U) == 10000);

    return 0;
}