#include "ConfigManager.hpp"

extern "C" {
#include "appcons.h"
}

#include <cstddef>
#include <cstdint>

namespace {

static_assert(static_cast<uint32_t>(offsetof(FlashConfig_t, crc)) == (sizeof(FlashConfig_t) - sizeof(uint16_t)),
              "FlashConfig_t::crc must be the final field for CRC calculation.");
static_assert((sizeof(FlashConfig_t) >= sizeof(uint16_t)),
              "FlashConfig_t must contain a CRC field.");

uint16_t calcCrc(const uint8_t* data, uint32_t length)
{
    uint16_t crc = 0xFFFFU;

    for (uint32_t i = 0U; i < length; ++i) {
        crc ^= data[i];

        for (uint8_t j = 0U; j < 8U; ++j) {
            if ((crc & 1U) != 0U) {
                crc = static_cast<uint16_t>((crc >> 1U) ^ 0xA001U);
            } else {
                crc = static_cast<uint16_t>(crc >> 1U);
            }
        }
    }

    return crc;
}

bool isValidFlashConfig(const FlashConfig_t& flashData)
{
    const uint16_t expected =
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) intentional byte-wise CRC over flash image
        calcCrc(reinterpret_cast<const uint8_t*>(&flashData),
                static_cast<uint32_t>(offsetof(FlashConfig_t, crc)));

    return (expected == flashData.crc) &&
           (flashData.device.verify_key == CONFIG_MAGIC_NUM);
}

bool isSupportedBaudrate(uint32_t baudrate)
{
    return (baudrate == 9600U) ||
           (baudrate == 19200U) ||
           (baudrate == 38400U) ||
           (baudrate == 57600U) ||
           (baudrate == 115200U);
}

bool isValidRuntimeConfig(const SettingsStruct& device, const SensorStruct& sensor)
{
    if (device.verify_key != CONFIG_MAGIC_NUM) {
        return false;
    }

    if ((device.id == 0U) || (device.id > 247U)) {
        return false;
    }

    if (!isSupportedBaudrate(device.baudrate)) {
        return false;
    }

    if ((sensor.type == 0U) || (sensor.model == 0U)) {
        return false;
    }

    if (sensor.mul_factor == 0U) {
        return false;
    }

    if ((sensor.display_unit < PPM) || (sensor.display_unit > PPB)) {
        return false;
    }

    if (sensor.gas < GAS_INDX_OFFSET) {
        return false;
    }

    return true;
}

void fillDefaults()
{
    Config_SetDefaults();
}


} // namespace

ConfigManager::ConfigManager(FlashStorage& flash)
    : flash_(flash),
      config_{},
      flashData_{}
{
}

const SettingsStruct& ConfigManager::loadOrDefaults()
{
    if (flash_.read(CONFIG_FLASH_ADDR, &flashData_, sizeof(flashData_)) &&
        isValidFlashConfig(flashData_) &&
        isValidRuntimeConfig(flashData_.device, flashData_.sensor)) {
        DeviceConfig = flashData_.device;
        Sensor = flashData_.sensor;
        config_ = DeviceConfig;
        return config_;
    }

    fillDefaults();
    config_ = DeviceConfig;
    (void)saveDeviceConfig();
    return config_;
}

bool ConfigManager::save()
{
    config_.verify_key = CONFIG_MAGIC_NUM;

    DeviceConfig = config_;
    return saveDeviceConfig();
}

bool ConfigManager::saveDeviceConfig()
{
    DeviceConfig.verify_key = CONFIG_MAGIC_NUM;

    flashData_.device = DeviceConfig;
    flashData_.sensor = Sensor;
    flashData_.crc =
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) intentional byte-wise CRC over flash image
        calcCrc(reinterpret_cast<const uint8_t*>(&flashData_),
                static_cast<uint32_t>(offsetof(FlashConfig_t, crc)));

    config_ = DeviceConfig;

    if (!flash_.eraseConfigSector()) {
        return false;
    }

    return flash_.write(CONFIG_FLASH_ADDR, &flashData_, sizeof(flashData_));
}

bool ConfigManager::quickSave()
{
    return saveDeviceConfig();
}

SettingsStruct& ConfigManager::data()
{
    return config_;
}

const SettingsStruct& ConfigManager::data() const
{
    return config_;
}

void ConfigManager::loadDefaults()
{
    fillDefaults();
    config_ = DeviceConfig;
    (void)saveDeviceConfig();
}
