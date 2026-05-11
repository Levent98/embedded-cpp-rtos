#ifndef APPLICATION_USER_HEADER_APPCONS_H_
#define APPLICATION_USER_HEADER_APPCONS_H_

/* Exported integral constants ---------------------------------------------*/
enum
{
    VERIFY_KEY = 0x20220817,
    AUTH_PASSWORD_PRV = 0x1017,
    AUTH_PASSWORD = 0x2609,
    FW_VERSION = 0x06020100,

    DEBUG_BUF_SIZE = 64,
    USB_DEBUG_CONSOLE = 0,
    RS485_UART = 1,
    DISPLAY_UART = 5,
    RS485_ERROR_TIMEOUT = 30000,
    DISPLAY_ERROR_TIMEOUT = 60000,
    HART_ERROR_TIMEOUT = 30000,
    RS485_TX_TIME_DELAY = 20,
    DEFAULT_MDBS_BAUDRATE = 57600,

    QUANTITY_HOLDING_REG = 124,
    QUANTITY_INPUT_REG = 124,
    HOLDING_REG_END_ADDRS = QUANTITY_HOLDING_REG - 1,

    SENSOR_VOL_RANGE = 5,
    MODBUS_SCALING_FACTOR = 10,
    RATIO_SCALING_FACTOR = 10000,
    CONCT_SCALING_FACTOR = 1000,
    COEFFIC_SCALING_FACTOR = 1000000,
    IR_MEAS_STOP_TIME = 8,

    MIN_WARM_UP_TIME = 1,
    MAX_WARM_UP_TIME = 10,
    TEST_TIMEOUT = 120 * 10,

    MAX_PPM_VALUE = 10000,
    MIN_PPM_VALUE = 1,
    MAX_LEL_VALUE = 100,
    MAX_LEL_VOL_VALUE = 15,
    MIN_LEL_VALUE = 10,
    MAX_VOL_VALUE = 100,
    MIN_VOL_VALUE = 1,
    MAX_O2_VALUE1 = 25,
    MAX_O2_VALUE2 = 30,
    MIN_O2_VALUE = 10,
    MAX_PPB_VALUE = 10000,
    MIN_PPB_VALUE = 1,

    MAX_RD_DELAY = 50,
    MIN_RD_DELAY = 10,
    LAMP_PERIOD_HIGH = 500,
    LAMP_PERIOD_LOW = 250,
    MAX_CALB_CYLE = 12,
    MIN_CALB_CYLE = 1,

    PELLISTOR_SAVER_LEVEL = 105,
    PLSTR_SAVER_LEVEL_RST = 101,
    MIN_CALIBRATION_TIME = 1 * 60,
    MAX_CALIBRATION_TIME = 10 * 60,
    CAL_DATA_REFRESH_TIME = 15 * 60 * 2,

    CALL_EXIT_WAIT_TIME = 5 * 60 * 2,
    INHIBIT_RESET_TIMEOUT = 20 * 60,
    OVERRANGE_LATCH_TIME = 5 * 60 * 2,

    DEF_APPLICATION_TIMER = 7,
    IR_APPLICATION_TIMER = 2,
    IDLE_LINE_MSR_TIMER = 13,

    MAX_HYSTERESIS = 20,
    MIN_HYSTERESIS = 2,
    MAX_REL_OFF_DELAY = 99,
    MAX_LOG_SIZE = 249,
    SIMULATION_RESET_TIMEOUT = 5 * 60,

    ADC_VREF_VOLTAGE_MV = 3300,
    UART_RX_READY_FLAG = 0xAA
};

/* Exported floating-point constants ---------------------------------------*/
static const float LIN_COEFFIC_A             = 0.267F;
static const float LIN_COEFFIC_N             = 0.725F;
static const float LIN_COEFFIC_ALPHA_POS     = 0.000363F;
static const float LIN_COEFFIC_ALPHA_NEG     = 0.000235F;
static const float LIN_COEFFIC_BETA_POS      = -0.106F;
static const float LIN_COEFFIC_BETA_NEG      = -0.137F;

static const float LIN_CO_A_100VOL           = 0.0563F;
static const float LIN_CO_N_100VOL           = 0.4970F;
static const float LIN_CO_ALPHA_POS_100VOL   = 0.000363F;
static const float LIN_CO_ALPHA_NEG_100VOL   = 0.000235F;
static const float LIN_CO_BETA_POS_100VOL    = 0.1451F;
static const float LIN_CO_BETA_NEG_100VOL    = 0.2318F;

static const float O2_ZERO_VOLUME            = 20.9F;
static const float O2_ZERO_VOLUME_MIN        = O2_ZERO_VOLUME - 0.2F;
static const float O2_ZERO_VOLUME_MAX        = O2_ZERO_VOLUME + 0.2F;

static const float MIN_CAL_CONCNT_PCT        = 0.10F;
static const float MAX_CAL_CONCNT_PCT        = 0.9F;

static const float MAX_ZERO_DEVIAITON        = -9.99F;
static const float ZERO_DEVIATION_RESET      = -8.0F;
static const float ZERO_SUPPRESSION_VALUE    = +3.0F;

static const float MIN_O2_ALARM_LEVEL        = 5.0F;
static const float MIN_ALARM_LEVEL           = 0.04F;
static const float VRTH_MIN                  = 0.270F;
static const float VRTH_MAX                  = 3.150F;
static const float MIN_TEMPRATURE            = -40.0F;
static const float MAX_IR_TEMPRATURE         = 60.0F;
static const float MAX_CTLY_TEMPRATURE       = 70.0F;
static const float MAX_TEMPRATURE_RANGE_H    = 100.0F;
static const float MAX_TEMPRATURE_RANGE_L    = 40.0F;
static const float MIN_TEMPRATURE_RANGE_L    = -65.0F;
static const float MIN_TEMPRATURE_RANGE_H    = -1.0F;

static const float MAX_CONCENT_RANGE_RATIO   = 1.15F;
static const float MIN_CONCENT_RANGE_RATIO   = -0.15F;
static const float LINEAR_OVER_RANGE_PRCNT   = 103.125F;
static const float OVER_RANGE_RESET_PRCNT    = 96.0F;
static const float LINEAR_UNDER_RANGE_PRCNT  = 0.0F;
static const float SGX_CH4_GAS_CNCT_RANGE    = 5.0F;

/* Exported enumerated types -----------------------------------------------*/
enum eAlarmConfigBitDef
{
    AUTO_RESET = 0x01,
    DFLT_ENERGISED = 0x02,
    TRIP_DIRECTION = 0x04,
    ALRM_TRG_TYPE = 0x08,
    ALRM_INHIBIT = 0x10,
    ALRM_STATUS = 0x80
};

enum eTripDirDef
{
    TRIP_RISING = 0x00,
    TRIP_FALLING = TRIP_DIRECTION
};

enum eTransmiterConfigBitDef
{
    TEST_DUE_FLT_INHIBIT = 0x0001,
    ZERO_SUPPRESSION_EN = 0x0002,
    RS485_FLT_INHIBIT = 0x0004,
    RELAY_CARD_INSTALLED = 0x0008,
    CALL_DUE_FLT_INHIBIT = 0x0010,
    BATTERY_FLT_INHIBIT = 0x0020,
    ANLG_OUT_FLT_AUTO_RESET = 0x0040,
    OVER_RANGE_AUTO_RESET = 0x0080
};

enum eHartModemConfigBitDef
{
    HART_ADDRESS_BITS = 0x003F,
    HART_LOOP_CURRENT_EN = 0x0040,
    HART_MODEM_ON = 0x0080,
    HART_SAVE_FLASH_EN = 0x0100
};

enum eAnalogLevelsDef
{
    FAULT_AN_LEV = 0,
    WARM_UP_AN_LEV = 1,
    CALB_AN_LEV = 2,
    INHIBIT_AN_LEV = 3,
    OVER_RNG_AN_LEV = 4,
    ZERO_GAS_AN_LEV = 5
};

enum eTrueFalseDef
{
    FALSE = 0,
    TRUE = 1
};

enum eAdcChan
{
    ADC1_DET_CHN = 0,
    ADC1_VLAMP_CHN = 8,
    ADC1_MCU_TEMP_CHN = 16,
    ADC1_VREFINT_CHN = 17,
    ADC1_VBAT_CHN = 18,

    ADC2_VSENSOR_CHN = 1,
    ADC2_VIN_CHN = 5,
    ADC2_ANG_OUT_CHN = 6,

    ADC3_TEMP_CHN = 2,
    ADC3_ILINE_CHN = 3,
    ADC3_VLINE_CHN = 10
};

enum eUnits
{
    PPM = 2,
    VOL = 3,
    LEL = 4,
    PPB = 5
};

enum eGasType
{
    GAS_INDX_OFFSET = 30,
    LPG = 30,
    CH4 = 31,
    C3H8 = 34,
    C5H12 = 37,
    C2H2 = 60,
    O2 = 65,
    CO2 = 71,
    FORMALDEHYDE = 74,
    ACETALDEHYDE = 78,
    HYDROGEN_CHLORINE = 79,
    TVOC = 80,
    VOC = 81,
    OZONE = 82,
    HF = 83,
    PHOSPHINE = 84,
    ISOBUTYLENE = 85,
    A2L = 91,
    LAST_GAS_INDX = 255
};

enum
{
    NUMBER_GASSES = LAST_GAS_INDX - GAS_INDX_OFFSET + 1
};

enum eSensorCardType
{
    INFRARED_SENSOR = 0x4576,
    ELECTRO_CHEM_SENSOR = 0x8844,
    CATALYTIC_SENSOR = 0x6522,
    SEMICONDUCTOR_SENSOR = 0x1906,
    PID_SENSOR = 0x3754,
    IRNET_7S_SENSOR = 0x5109,
    IRNET_MPS_SENSOR = 0x5317,
    IR_VOLT_SENSOR = 0x2342,
    IR_VOLT_MPS_SENSOR = 0x3589
};

enum eDetModelType
{
    GENERIC_MODEL_DET = 0x5202,
    P_MODEL_DET = 0x5687,
    PQ_MODEL_DET = 0x5420,
    PQS_MODEL_DET = 0x5735,
    PPS_MODEL_DET = 0x5831,
    PE_MODEL_DET = 0x5810,
    PEM_MODEL_DET = 0x5893,
    PC3_MODEL_DET = 0x5685,
    SOMA_MODEL_DET = 0x5324,
    PX_MODEL_DET = 0x4934,
    AN2INMOD_MODEL_DET = 0x4731,
    BTN_MODEL_DET = 0x5183
};

enum eLanguage
{
    ENGLISH = 0,
    TURKISH,
    RUSSIAN,
    GERMAN,
    NO_DSP_LNG,
    DISPLAY_MODULE_DISBALE = 0x40,
    DISPLAY_ON = 0x80
};

enum eCalDate
{
    CLB_YEAR = 0,
    CLB_MONTH = 1,
    CLB_DATE = 2
};

enum eOneWireOp
{
    DETECT_PRESENCE = 0,
    READ_ROMID = 1,
    READ_EEPROM = 2,
    READ_SENS_TYPE = 3,
    SET_SENS_TYPE = 4,
    SET_WR_PRTCT = 5,
    WRITE_EEPROM_PAGE = 6,
    SET_SENS_CALB_DATA = 7,
    READ_EEPROM_WORD = 8,
    SET_SENS_TEST_DATE = 9
};

enum eAlarmLevel
{
    ALARM1_SET_POINT = 0x00,
    ALARM2_SET_POINT = 0x01,
    ALARM3_SET_POINT = 0x02
};

enum eTestDevice
{
    DISPLAY_TEST = 1,
    RELAY_TEST = 2,
    ANALOGUE_OUTPUT_TEST = 3,
    BUMP_TEST = 4,
    CANCEL_TEST = 5
};

enum eOPerationalMode
{
    QUIESCENT = 0x00,
    ALARM = 0x01,
    WARM_UP = 0x02,
    FAULT = 0x03,
    INHIBIT = 0x04,
    FUNCTIONAL_TEST = 0x05,
    CALIBRATION = 0x06,
    SETUP = 0x07
};

enum eCalibrationMode
{
    CALL_INACTIVE = 0x00,
    SET_ZERO = 0x01,
    SPAN_EXIT_TIMEOUT = 0x02,
    SET_SPAN = 0x03,
    SET_CALL_ERROR = 0x04,
    SET_CALL_SUCCESS = 0x05
};

enum eBumpTestMode
{
    BUMPTEST_INACTIVE = 0x00,
    BUMPTEST_CLEAN_AIR = 0x01,
    BUMPTEST_APPLY_GAS = 0x02,
    BUMPTEST_REMOVE_GAS = 0x03,
    BUMPTEST_RESULT = 0x04
};

enum eAlarmStatus
{
    ALARM_RESET = 0x00,
    ALARM1 = 0x01,
    ALARM2 = 0x02,
    ALARM3 = 0x03,
    OVER_RANGE_AL = 0x04
};

enum ePowerFault
{
    POWER_ON_RST = 0x01,
    LOW_PWR_RST = 0x02,
    EXT_PIN_RST = 0x03,
    WATCHDOG_RST = 0x04,
    SOFTWARE_RST = 0x05,
    UNKNOWN_RST = 0x06
};

enum eRelayCardOutPortDef
{
    FAULT_REL = 0x01,
    ALRM_LEV1_REL = 0x02,
    ALRM_LEV2_REL = 0x04,
    RL_MCU_ALIVE = 0x08,
    REL_FAULT_LED = 0x10,
    REL_AL1_LED = 0x20,
    REL_AL2_LED = 0x40,
    SHFTR_PRL_LD = 0x80,
    ALL_ALRM_REL = 0x06
};

enum eRelayCardInPortDef
{
    FAULT_REL_ST = 0x01,
    ALRM_LEV1_REL_ST = 0x02,
    ALRM_LEV2_REL_ST = 0x04,
    REL_WATCHDOG_ST = 0x08,

    REL_FAULT_COM_ST = 0x10,
    REL_AL1_COM_ST = 0x20,
    REL_AL2_COM_ST = 0x40,
    REMOTE_RESET_ST = 0x80
};

enum eDeviceStatus
{
    DEVICE_OK = 0xaa
};

enum eFaultDef
{
    NO_FAULT = 0x00,

    SENSOR_ERROR = 0x10,
    NOT_CONNECTED = 0x11,
    SIGNAL_OUT_RANGE = 0x12,
    TEMPERATURE_ERR = 0x13,
    SENSOR_TYPE_ERR = 0x14,
    IR_LAMP_FRQ_ERR = 0x15,
    IR_LAMP_REG_ERR = 0x17,
    SENSOR_REGULATOR = 0x19,
    SENSOR_MODEL_ERR = 0x1A,
    SENSOR_DATA_CRC_ERR = 0x1B,
    NEGATIVE_DRIFT = 0x1C,
    SENSOR_NTC_ERR = 0x1D,
    OVER_RANGE_ERR = 0x1E,

    ADC_ERR = 0x20,
    INT_ADC1_ERR = 0x21,
    INT_ADC2_ERR = 0x22,
    EXT_ADC_ERR = 0x23,
    ADC_REF_ERR = 0x24,

    CURRENT_OUTPUT_ERR = 0x30,
    DAC_OUTPUT_ERROR = 0x31,
    LINE_OUTPUT_OPEN = 0x32,
    LINE_OUPUT_SHORT = 0x33,

    SUPPLY_VOL_ERROR = 0x40,
    LOW_SUPPLY_VOLTAGE = 0x41,
    HIGH_SUPPLY_VOLTAGE = 0x42,

    PRG_RAM_ERROR = 0x50,
    RAM_TEST_FAILURE = 0x51,
    SRAM_FAILURE = 0x52,

    PRG_FLASH_ERR = 0x60,
    FLASH_CRC_ERR = 0x61,
    CRC_INIT_ERR = 0x62,
    XTRM_STNGS_CRC_ERR = 0x63,
    XTRM_USER_STNGS_ERR = 0x64,

    SENSOR_EEPROM_ERR = 0x70,
    SETTINGS_CRC_ERR = 0x71,
    ONE_WIRE_ERR = 0x72,
    TYPE_CRC_ERR = 0x73,

    DUE_DATE_ERROR = 0x80,
    SENSOR_CALIB_DUE = 0x81,
    BUMP_TEST_DUE = 0x82,

    BATTERY_ERROR = 0x90,
    BATTERY_VOLTAGE_LOW = 0x91,
    NO_BATTERY_DETECTED = 0x92,

    DETECTOR_COMM_ERR = 0xA0,
    DSP_COMMM_ERR = 0xA1,
    RS485_COMMM_ERR = 0xA2,
    RS485_PORT_ERR = 0xA3,
    HART_PORT_ERR = 0xA4,

    RELAY_CARD_ERR = 0xB0,
    FLT_RL_IO_ERR = 0xB1,
    AL1_RL_IO_ERR = 0xB2,
    AL2_RL_IO_ERR = 0xB3,
    FLT_RELAY_ERR = 0xB4,
    AL1_RELAY_ERR = 0xB5,
    AL2_RELAY_ERR = 0xB6,

    CPU_ERROR = 0xC0,
    CPU_TEST_FAILURE = 0xC1,

    PROGRAM_ERROR = 0xD0,
    STACK_TEST_FAILURE = 0xD1,
    STACK_OVERFLOW_ERR = 0xD2,
    FLOW_CONTROL_ERR = 0xD3,

    AMBIENT_ERROR = 0xE0,
    HIGH_TEMP_ERR = 0xE1,
    LOW_TEMP_ERR = 0xE2
};

enum eLogTypeDef
{
    EVENT_RESET_LOG = 0x00,
    DATE_TIME_CAHANGE_LOG = 0x01,
    DATE_TIME_SET_LOG = 0x02,
    ALARM_LOG = 0x03,
    FAULT_LOG = 0x04,
    CALIBRATION_LOG = 0x05,
    POWER_ON_LOG = 0x06,
    LOG_BUFFER_DELETED = 0x07
};

enum eUartIdleTimerStatus
{
    STOP = 0x00,
    RUN = 0x01
};

enum eAccsLevel
{
    ACCESS_L1 = 0,
    ACCESS_L2 = 1
};

enum eCallStartError
{
    CALL_REQ_SUCCESS = 0,
    CALL_REQ_OPERATION_MODE_ERROR = 10,
    CALL_REQ_SENSOR_ERROR = 11,
    CALL_REQ_ZERO_LEVEL_ERROR = 12,
    CALL_REQ_SIGNAL_LEVEL_ERROR = 13,
    REF_GAS_CONCENT_HIGH_ERROR = 14,
    REF_GAS_CONCENT_LOW_ERROR = 15,
    REF_GAS_TYPE_ERROR = 16,
    REF_GAS_UNIT_ERROR = 17
};

enum eAlarmConfigSetError
{
    AL_CONFIG_SET_SUCCESS = 0,
    AL_CONFIG_AL_ACTIVE_ERROR = 20,
    AL_CONFIG_PARAMETER_HIGH_ERROR = 21,
    AL_CONFIG_PARAMETER_LOW_ERROR = 22,
    AL_CONFIG_INV_PARAMETER = 23
};

#endif