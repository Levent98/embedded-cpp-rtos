#ifndef PWM_OUTPUT_HPP
#define PWM_OUTPUT_HPP

#include <stdint.h>

class PwmOutput {
public:
    PwmOutput();
    PwmOutput(const PwmOutput&) = delete;
    PwmOutput& operator=(const PwmOutput&) = delete;
    PwmOutput(PwmOutput&&) = delete;
    PwmOutput& operator=(PwmOutput&&) = delete;

    static void initialize();

    void setTemperature(int32_t tempX100);
    void setHumidity(uint32_t humX100);

private:
    static constexpr uint16_t PWM_PSC = 23;
    static constexpr uint16_t PWM_ARR = 99;

    static constexpr uint16_t DUTY_MIN = 18;
    static constexpr uint16_t DUTY_MAX = 91;

    static constexpr uint32_t CURRENT_MIN = 4;
    static constexpr uint32_t CURRENT_MAX = 20;

    static constexpr int32_t TEMP_MIN_X100 = 0;
    static constexpr int32_t TEMP_MAX_X100 = 100;

    static constexpr uint32_t HUM_MIN_X100 = 0;
    static constexpr uint32_t HUM_MAX_X100 = 100;

    static void setTim5(uint16_t duty);
    static void setTim11(uint16_t duty);

    static uint32_t temperatureToCurrent(int32_t temp);
    static uint32_t humidityToCurrent(uint32_t hum);
    static uint16_t currentToPwm(uint32_t current);
};

#endif