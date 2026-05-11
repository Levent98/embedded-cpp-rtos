#include "MedianFilter.hpp"
#include <algorithm> // Eger std::copy veya std::sort kullanilmasina izin varsa

MedianFilter::MedianFilter()
    : index_{0U},
      count_{0U},
      samples_{},
      sortBuffer_{}
{
    // C++11 uniform initialization ({}) kullanimi POD tipler için en güvenlisidir.
}

void MedianFilter::reset()
{
    index_ = 0U;
    count_ = 0U;

    for (std::uint8_t i = 0U; i < Size; ++i) {
        samples_[i] = 0;
        sortBuffer_[i] = 0;
    }
}

void MedianFilter::seed(std::int32_t value)
{
    index_ = 0U;
    count_ = Size;

    for (std::uint8_t i = 0U; i < Size; ++i) {
        samples_[i] = value;
        sortBuffer_[i] = value;
    }
}

bool MedianFilter::initialized() const
{
    return (count_ >= Size);
}

std::int32_t MedianFilter::apply(std::int32_t value)
{
    // 1. Yeni degeri dairesel buffer'a ekle
    samples_[index_] = value;
    
    if (count_ < Size) {
        ++count_;
    }
    
    index_++;
    if (index_ >= Size) {
        index_ = 0U;
    }

    // 2. Siralama için veriyi kopyala
    for (std::uint8_t i = 0U; i < count_; ++i) {
        sortBuffer_[i] = samples_[i];
    }

    // 3. Insertion Sort (MISRA uyumlu hale getirildi)
    // Degiskenler döngü kapsamina (scope) çekilerek Note 901 giderildi.
    for (std::uint8_t i = 1U; i < count_; ++i) {
        const std::int32_t key = sortBuffer_[i];
        std::uint8_t j = i;

        // Note 9007: Yan etki riskini azaltmak için kosul kontrolü netlestirildi
        while (j > 0U) {
            if (sortBuffer_[j - 1U] > key) {
                sortBuffer_[j] = sortBuffer_[j - 1U];
                --j;
            } else {
                break; 
            }
        }
        sortBuffer_[j] = key;
    }

    // 4. Medyani döndür
    // Eger örnek sayisi çok azsa (örn: 1-2), dogrudan degeri döndürmek daha güvenlidir.
    std::int32_t result;
    if (count_ > 0U) {
        result = sortBuffer_[count_ / 2U];
    } else {
        result = value;
    }

    return result;
}