#ifndef MAIN_HPP
#define MAIN_HPP

#include <stdint.h>

namespace BuildInfo
{
constexpr uint32_t yearFromDate(const char* date) noexcept
{
    return static_cast<uint32_t>(((date[9] - '0') * 10) + (date[10] - '0'));
}

constexpr uint32_t monthFromDate(const char* date) noexcept
{
    return (date[0] == 'J' && date[1] == 'a') ? 1U  :
           (date[0] == 'F')                   ? 2U  :
           (date[0] == 'M' && date[2] == 'r') ? 3U  :
           (date[0] == 'A' && date[1] == 'p') ? 4U  :
           (date[0] == 'M' && date[2] == 'y') ? 5U  :
           (date[0] == 'J' && date[2] == 'n') ? 6U  :
           (date[0] == 'J' && date[2] == 'l') ? 7U  :
           (date[0] == 'A' && date[1] == 'u') ? 8U  :
           (date[0] == 'S')                   ? 9U  :
           (date[0] == 'O')                   ? 10U :
           (date[0] == 'N')                   ? 11U : 12U;
}

constexpr uint32_t dayFromDate(const char* date) noexcept
{
    return static_cast<uint32_t>((((date[4] == ' ') ? 0 : (date[4] - '0')) * 10) +
                                 (date[5] - '0'));
}

constexpr uint32_t valueFromTime(const char tens, const char ones) noexcept
{
    return static_cast<uint32_t>(((tens - '0') * 10) + (ones - '0'));
}
} // namespace BuildInfo

#if defined(__DATE__)
static constexpr uint32_t BUILD_YEAR  = BuildInfo::yearFromDate(__DATE__);
static constexpr uint32_t BUILD_MONTH = BuildInfo::monthFromDate(__DATE__);
static constexpr uint32_t BUILD_DAY   = BuildInfo::dayFromDate(__DATE__);
static constexpr uint32_t FW_VER_DATE = ((BUILD_YEAR & 0xFFU) << 24U) |
                                        ((BUILD_MONTH & 0xFFU) << 16U) |
                                        ((BUILD_DAY & 0xFFU) << 8U);
#else
static constexpr uint32_t BUILD_YEAR  = 0U;
static constexpr uint32_t BUILD_MONTH = 0U;
static constexpr uint32_t BUILD_DAY   = 0U;
static constexpr uint32_t FW_VER_DATE = 0x00000000U;
#endif

#if defined(__TIME__)
static constexpr uint32_t BUILD_HOUR = BuildInfo::valueFromTime(__TIME__[0], __TIME__[1]);
static constexpr uint32_t BUILD_MIN  = BuildInfo::valueFromTime(__TIME__[3], __TIME__[4]);
static constexpr uint32_t BUILD_SEC  = BuildInfo::valueFromTime(__TIME__[6], __TIME__[7]);
#else
static constexpr uint32_t BUILD_HOUR = 0U;
static constexpr uint32_t BUILD_MIN  = 0U;
static constexpr uint32_t BUILD_SEC  = 0U;
#endif

#endif
