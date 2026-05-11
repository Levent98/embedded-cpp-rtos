#ifndef I2CBUS_HPP
#define I2CBUS_HPP

#include <cstdint>
#include <cstddef>

enum class I2cStatus : std::uint8_t {
    Ok = 0,
    Timeout,
    Nack,
    BusError,
    InvalidArgument
};

class I2cBus {
public:
    I2cBus();
    I2cBus(const I2cBus&) = delete;
    I2cBus& operator=(const I2cBus&) = delete;
    I2cBus(I2cBus&&) = delete;
    I2cBus& operator=(I2cBus&&) = delete;

    void initialize();

    I2cStatus write(std::uint8_t address, const std::uint8_t* data, std::size_t size);
    I2cStatus read(std::uint8_t address, std::uint8_t* data, std::size_t size);
    I2cStatus writeRead(std::uint8_t address,
                        const std::uint8_t* txData,
                        std::size_t txSize,
                        std::uint8_t* rxData,
                        std::size_t rxSize);

    bool recoverBus();

private:
    bool initialized_;
};

#endif