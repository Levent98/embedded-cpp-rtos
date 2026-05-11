#include "Config_Manager.h"
#include "appcons.h"
#include <string.h>

DeviceConfig_t DeviceConfig;
SensorStruct Sensor;
volatile uint8_t SaveDevSettings = 0U;
volatile uint8_t SaveSensorInfo = 0U;

static uint16_t Config_CalcCRC(const uint8_t* data, uint32_t length)
{
    uint16_t crc = 0xFFFFU;

    for (uint32_t i = 0U; i < length; ++i) {
        crc ^= data[i];

        for (uint8_t j = 0U; j < 8U; ++j) {
            if ((crc & 1U) != 0U) {
                crc = (uint16_t)((crc >> 1U) ^ 0xA001U);
            } else {
                crc = (uint16_t)(crc >> 1U);
            }
        }
    }

    return crc;
}

void Config_Init(void)
{
    Config_LoadFromFlash();
}

void Config_SetDefaults(void)
{
    /* CRC fail / first boot fallback: never leave runtime config as all-zero. */
    memset(&DeviceConfig, 0, sizeof(DeviceConfig));
    memset(&Sensor, 0, sizeof(Sensor));

    DeviceConfig.verify_key = CONFIG_MAGIC_NUM;
    DeviceConfig.id = 1U;
    DeviceConfig.baudrate = DEFAULT_MDBS_BAUDRATE;
    DeviceConfig.display = (uint16_t)(DISPLAY_ON | ENGLISH);
    DeviceConfig.config_reg = OVER_RANGE_AUTO_RESET;

    DeviceConfig.alarm_threshold[0] = 500U;
    DeviceConfig.alarm_threshold[1] = 100U;
    DeviceConfig.alarm_threshold[2] = 800U;
    DeviceConfig.alarm_threshold[3] = 0U;

    for (uint8_t i = 0U; i < 4U; ++i) {
        DeviceConfig.alarm_settings[i] = AUTO_RESET;
        DeviceConfig.alarm_off_hysteresis[i] = MIN_HYSTERESIS;
        DeviceConfig.relay_off_delay[i] = 0U;
    }

    DeviceConfig.analog_out_lev[FAULT_AN_LEV] = 10U;
    DeviceConfig.analog_out_lev[WARM_UP_AN_LEV] = 20U;
    DeviceConfig.analog_out_lev[CALB_AN_LEV] = 20U;
    DeviceConfig.analog_out_lev[INHIBIT_AN_LEV] = 20U;
    DeviceConfig.analog_out_lev[OVER_RNG_AN_LEV] = 22U;
    DeviceConfig.analog_out_lev[ZERO_GAS_AN_LEV] = 40U;

    Sensor.model = GENERIC_MODEL_DET;
    Sensor.type = CATALYTIC_SENSOR;
    Sensor.gas = CH4;
    Sensor.display_unit = LEL;
    Sensor.mul_factor = MODBUS_SCALING_FACTOR;
    Sensor.gas_conc_range = 100.0;
    Sensor.gas_lel_value = 100.0;
    Sensor.upper_range_limit = 100.0;
    Sensor.lower_range_limit = 0.0;
    Sensor.over_range_reset_thr = OVER_RANGE_RESET_PRCNT;
    Sensor.warm_up_time = MIN_WARM_UP_TIME;
    Sensor.supply_voltage = SENSOR_VOL_RANGE;
    Sensor.opr_temp_min = MIN_TEMPRATURE;
    Sensor.opr_temp_max = MAX_CTLY_TEMPRATURE;
    Sensor.Calibration.applied_gas = CH4;
    Sensor.Calibration.span_gas_type = CH4;
    Sensor.Calibration.aplied_gas_conc = 50.0;
    Sensor.Calibration.cycle = MIN_CALB_CYLE;
}

void Config_LoadFromFlash(void)
{
    if (DeviceConfig.verify_key != CONFIG_MAGIC_NUM) {
        Config_SetDefaults();
        return;
    }
}

uint8_t Config_SaveToFlash(void)
{
    DeviceConfig.verify_key = CONFIG_MAGIC_NUM;
    return 1U;
}

uint8_t Process_BaseSettings(uint16_t* data, uint8_t len)
{
    (void)len;
    if (data == 0) {
        return 0U;
    }
    DeviceConfig.id = data[0];
    SaveDevSettings = 1U;
    return 1U;
}

uint8_t Process_SensorSettings(uint16_t* data, uint8_t len)
{
    (void)len;
    if (data == 0) {
        return 0U;
    }
    DeviceConfig.alarm_threshold[0] = data[0];
    SaveSensorInfo = 1U;
    SaveDevSettings = 1U;
    return 1U;
}

void Config_QuickSave(void)
{
    (void)Config_SaveToFlash();
}

void Internal_Process_Base(uint16_t* data)
{
    (void)data;
}

void Internal_Process_Sensor(uint16_t* data, uint16_t start_addr, uint16_t qty)
{
    (void)data;
    (void)start_addr;
    (void)qty;
}

uint16_t GetSensorModel(uint16_t indx)
{
    return indx;
}

uint16_t CheckSensorType(uint16_t type)
{
    return type;
}

uint8_t CheckConctSettings(SensorStruct* sensor)
{
    if (sensor == 0) {
        return 0U;
    }
    return 1U;
}

void SetGasAlarmLimits(SensorStruct* sensor)
{
    (void)sensor;
}
