#include "Config_Manager.h"
#include "appcons.h"
#include <cassert>

int main()
{
    DeviceConfig.verify_key = 0U;

    Config_LoadFromFlash();

    assert(DeviceConfig.verify_key == CONFIG_MAGIC_NUM);
    assert(DeviceConfig.id == 1U);
    assert(DeviceConfig.baudrate == DEFAULT_MDBS_BAUDRATE);
    assert(Sensor.mul_factor == MODBUS_SCALING_FACTOR);

    uint16_t baseData[] = {17U};

    SaveDevSettings = 0U;

    assert(Process_BaseSettings(baseData, 1U) == 1U);
    assert(DeviceConfig.id == 17U);
    assert(SaveDevSettings == 1U);

    assert(Process_BaseSettings(nullptr, 1U) == 0U);

    uint16_t sensorData[] = {650U};

    SaveSensorInfo = 0U;
    SaveDevSettings = 0U;

    assert(Process_SensorSettings(sensorData, 1U) == 1U);
    assert(DeviceConfig.alarm_threshold[0] == 650U);
    assert(SaveSensorInfo == 1U);
    assert(SaveDevSettings == 1U);

    assert(Process_SensorSettings(nullptr, 1U) == 0U);

    assert(CheckConctSettings(&Sensor) == 1U);
    assert(CheckConctSettings(nullptr) == 0U);

    return 0;
}