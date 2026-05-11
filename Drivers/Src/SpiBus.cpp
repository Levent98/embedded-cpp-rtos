#include "SpiBus.hpp"

SpiBus::SpiBus()
{
}

void SpiBus::init()
{
    gpioInit();
    spiInit();
}

void SpiBus::gpioInit()
{
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // -------- PB3 -> SCK --------
    GPIOB->MODER   &= ~(3U << (3 * 2));
    GPIOB->MODER   |=  (2U << (3 * 2)); // AF
    GPIOB->AFR[0]  &= ~(0xF << (3 * 4));
    GPIOB->AFR[0]  |=  (5U << (3 * 4)); // AF5 SPI1

    // -------- PB4 -> MISO --------
    GPIOB->MODER   &= ~(3U << (4 * 2));
    GPIOB->MODER   |=  (2U << (4 * 2));
    GPIOB->AFR[0]  &= ~(0xF << (4 * 4));
    GPIOB->AFR[0]  |=  (5U << (4 * 4));

    // -------- PB5 -> MOSI --------
    GPIOB->MODER   &= ~(3U << (5 * 2));
    GPIOB->MODER   |=  (2U << (5 * 2));
    GPIOB->AFR[0]  &= ~(0xF << (5 * 4));
    GPIOB->AFR[0]  |=  (5U << (5 * 4));

    // -------- PB12 -> CS (GPIO) --------
    GPIOB->MODER   &= ~(3U << (12 * 2));
    GPIOB->MODER   |=  (1U << (12 * 2)); // output

    GPIOB->OSPEEDR |= (3U << (3 * 2)) | (3U << (4 * 2)) | (3U << (5 * 2));
    GPIOB->PUPDR   &= ~((3U << (3 * 2)) | (3U << (4 * 2)) | (3U << (5 * 2)));

    csHigh();
}

void SpiBus::spiInit()
{
    SPI1->CR1 = 0;

    SPI1->CR1 |= SPI_CR1_MSTR;     // Master
    SPI1->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM; // Software NSS
    SPI1->CR1 |= SPI_CR1_BR_1;     // Baudrate (fPCLK/8)
    SPI1->CR1 |= SPI_CR1_SPE;      // Enable SPI
}

uint8_t SpiBus::transfer(uint8_t data)
{
    while (!(SPI1->SR & SPI_SR_TXE));
    *((__IO uint8_t*)&SPI1->DR) = data;

    while (!(SPI1->SR & SPI_SR_RXNE));
    return *((__IO uint8_t*)&SPI1->DR);
}

void SpiBus::transfer(uint8_t* tx, uint8_t* rx, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        uint8_t data = tx ? tx[i] : 0xFF;
        uint8_t r = transfer(data);

        if (rx) rx[i] = r;
    }
}

void SpiBus::csLow()
{
    GPIOB->BSRR = (1U << (12 + 16));
}

void SpiBus::csHigh()
{
    GPIOB->BSRR = (1U << 12);
}