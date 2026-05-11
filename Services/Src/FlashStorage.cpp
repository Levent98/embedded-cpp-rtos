#include "FlashStorage.hpp"

extern "C" {
#include "stm32f410rx.h"
}

#include <cstring>
#include <cstdint>

FlashStorage::FlashStorage()
{
}

bool FlashStorage::waitUntilReady()
{
    while ((FLASH->SR & FLASH_SR_BSY) != 0U) {
    }

    if ((FLASH->SR & (FLASH_SR_PGSERR |
                      FLASH_SR_PGPERR |
                      FLASH_SR_PGAERR |
                      FLASH_SR_WRPERR)) != 0U) {
        FLASH->SR = FLASH_SR_PGSERR |
                    FLASH_SR_PGPERR |
                    FLASH_SR_PGAERR |
                    FLASH_SR_WRPERR;
        return false;
    }

    return true;
}

void FlashStorage::unlock()
{
    if ((FLASH->CR & FLASH_CR_LOCK) != 0U) {
        FLASH->KEYR = FlashKey1;
        FLASH->KEYR = FlashKey2;
    }
}

void FlashStorage::lock()
{
    FLASH->CR |= FLASH_CR_LOCK;
}

bool FlashStorage::isAddressInConfigSector(std::uint32_t address, std::size_t size)
{
    const std::uint32_t start = ConfigFlashAddress;
    const std::uint32_t end = ConfigFlashAddress + ConfigSectorSize;

    if (size == 0U) {
        return false;
    }

    if (address < start) {
        return false;
    }

    if ((address + size) > end) {
        return false;
    }

    return true;
}

bool FlashStorage::eraseConfigSector()
{
    unlock();

    if (!waitUntilReady()) {
        lock();
        return false;
    }

    FLASH->CR &= ~(FLASH_CR_SNB | FLASH_CR_PSIZE);
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (ConfigSectorNumber << FLASH_CR_SNB_Pos);
    FLASH->CR |= FLASH_CR_PSIZE_1;
    FLASH->CR |= FLASH_CR_STRT;

    const bool ok = waitUntilReady();

    FLASH->CR &= ~FLASH_CR_SER;
    lock();

    return ok;
}

bool FlashStorage::write(std::uint32_t address, const void* data, std::size_t size) const
{
    if ((data == nullptr) || !isAddressInConfigSector(address, size)) {
        return false;
    }

    const std::uint8_t* src = static_cast<const std::uint8_t*>(data);

    unlock();

    if (!waitUntilReady()) {
        lock();
        return false;
    }

    FLASH->CR &= ~FLASH_CR_PSIZE;
    FLASH->CR |= FLASH_CR_PSIZE_1;
    FLASH->CR |= FLASH_CR_PG;

    std::size_t offset = 0U;

    while (offset < size) {
        std::uint32_t word = 0xFFFFFFFFU;
        const std::size_t remaining = size - offset;
        const std::size_t chunk = (remaining >= 4U) ? 4U : remaining;

        std::memcpy(&word, &src[offset], chunk);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) memory-mapped flash programming
        *reinterpret_cast<volatile std::uint32_t*>(address + offset) = word;

        if (!waitUntilReady()) {
            FLASH->CR &= ~FLASH_CR_PG;
            lock();
            return false;
        }

        offset += 4U;
    }

    FLASH->CR &= ~FLASH_CR_PG;
    lock();

    return verify(address, data, size);
}

bool FlashStorage::read(std::uint32_t address, void* data, std::size_t size)
{
    if ((data == nullptr) || !isAddressInConfigSector(address, size)) {
        return false;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) memory-mapped flash read
    std::memcpy(data, reinterpret_cast<const void*>(address), size);
    return true;
}

bool FlashStorage::verify(std::uint32_t address, const void* data, std::size_t size)
{
    if ((data == nullptr) || !isAddressInConfigSector(address, size)) {
        return false;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) memory-mapped flash verify
    return std::memcmp(reinterpret_cast<const void*>(address), data, size) == 0;
}
