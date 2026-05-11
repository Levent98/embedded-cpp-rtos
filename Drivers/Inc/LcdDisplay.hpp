#ifndef LCD_DISPLAY_HPP
#define LCD_DISPLAY_HPP

#include <stdint.h>

class LcdDisplay
{
public:
    LcdDisplay();
    LcdDisplay(const LcdDisplay&) = delete;
    LcdDisplay& operator=(const LcdDisplay&) = delete;
    LcdDisplay(LcdDisplay&&) = delete;
    LcdDisplay& operator=(LcdDisplay&&) = delete;

    static void initialize();
    static void clear();
    static void setCursor(uint8_t row, uint8_t column);
    static void writeChar(char c);
    static void writeString(const char* text);
    static void createChar(uint8_t location, const uint8_t* charmap);
    void writeFixed(int32_t value, uint8_t fracDigits);
    void test();

private:
    static constexpr uint8_t kBufferSize = 20U;
    char finalBuffer_[kBufferSize];

    static void rsHigh();
    static void rsLow();
    static void rwLow();
    static void eHigh();
    static void eLow();
    static void backlightHigh();

    static void writeDb4To7(uint8_t nibble);
    static void send4(uint8_t nibble);

    static void command(uint8_t cmd);
    static void data(uint8_t value);

    static uint32_t pow10(uint8_t exp);
};

#endif