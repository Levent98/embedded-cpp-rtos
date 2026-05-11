#ifndef FLASHSTORAGE_HPP
#define FLASHSTORAGE_HPP

#include <cstddef>
#include <cstdint>

class FlashStorage {
public:
    FlashStorage();
    ~FlashStorage() = default;

    FlashStorage(const FlashStorage&) = delete;
    FlashStorage& operator=(const FlashStorage&) = delete;
    
    FlashStorage(FlashStorage&&) = delete;
    FlashStorage& operator=(FlashStorage&&) = delete;

    static bool eraseConfigSector();
    bool write(std::uint32_t address, const void* data, std::size_t size) const;
    static bool read(std::uint32_t address, void* data, std::size_t size);
    static bool verify(std::uint32_t address, const void* data, std::size_t size);

private:
    static constexpr std::uint32_t FlashKey1 = 0x45670123U;
    static constexpr std::uint32_t FlashKey2 = 0xCDEF89ABU;

    static constexpr std::uint32_t ConfigFlashAddress = 0x08004000U;
    static constexpr std::uint32_t ConfigSectorSize = 0x4000U;
    static constexpr std::uint32_t ConfigSectorNumber = 1U;

    static bool isAddressInConfigSector(std::uint32_t address, std::size_t size);
    static bool waitUntilReady();
    static void unlock();
    static void lock();
};

#endif
