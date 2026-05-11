#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include "FlashStorage.hpp"

extern "C" {
#include "Config_Manager.h"
}

class ConfigManager {
public:
    explicit ConfigManager(FlashStorage& flash);
    ~ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    const SettingsStruct& loadOrDefaults();

    bool save();
    bool saveDeviceConfig();
    bool quickSave();

    SettingsStruct& data();
    [[nodiscard]] const SettingsStruct& data() const;

    void loadDefaults();

private:
    FlashStorage& flash_;
    SettingsStruct config_;

    // Reusable flash image buffer. Keeping this as a member avoids placing
    // a full FlashConfig_t object on the stack inside load/save functions.
    FlashConfig_t flashData_;
};

#endif
