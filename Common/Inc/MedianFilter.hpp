#ifndef MEDIAN_FILTER_HPP
#define MEDIAN_FILTER_HPP

#include <array>
#include <cstdint>

class MedianFilter {
public:
    static constexpr std::uint8_t Size = 18U;

    MedianFilter();
    ~MedianFilter() = default;

    MedianFilter(const MedianFilter&) = delete;
    MedianFilter& operator=(const MedianFilter&) = delete;
    
    MedianFilter(MedianFilter&&) = delete;
    MedianFilter& operator=(MedianFilter&&) = delete;

    void reset();
    void seed(std::int32_t value);
    std::int32_t apply(std::int32_t value);
    [[nodiscard]] bool initialized() const;

private:
    std::uint8_t index_;
    std::uint8_t count_;
    std::array<std::int32_t, Size> samples_;
    std::array<std::int32_t, Size> sortBuffer_;
};

#endif