#include "UartDriver.hpp"

extern "C" {
#include "stm32f410rx.h"
#include "delay.h"
}

#include <cstring>
#include <cstdint>

namespace
{

    std::uint32_t dmaAddressOf(const volatile void* address) noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) required for STM32 DMA address registers
        return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(address));
    }
    struct RingBuffer
    {
        std::uint8_t buf[UART_RX_BUF_SIZE];
        volatile std::uint16_t head;
        volatile std::uint16_t tail;
    };

    typedef enum
    {
        RtuIdle = 0,
        RtuReceiving
    } RtuState;

    struct TxItem
    {
        std::uint8_t data[UART_TX_BUF_SIZE];
        std::uint16_t len;
    };

    struct TxQueue
    {
        TxItem items[8];
        std::uint8_t head;
        std::uint8_t tail;
        std::uint8_t count;
    };

    static RingBuffer rxRing = {{0}, 0U, 0U};
    static std::uint8_t txBuf[UART_TX_BUF_SIZE] = {0};
    static TxQueue txQueue = {{{{0}, 0U}}, 0U, 0U, 0U};

    static RtuState rtuState = RtuIdle;
    static std::uint16_t rtuLastHead = 0U;
    static std::uint16_t rtuFrameHead = 0U;
    static std::uint32_t rtuDeadline = 0U;

    static inline std::uint16_t dmaHead()
    {
        return static_cast<std::uint16_t>(UART_RX_BUF_SIZE - DMA2_Stream2->NDTR);
    }

    static inline void rs485TransmitMode()
    {
        GPIOA->BSRR = GPIO_BSRR_BS8;
    }

    static inline void rs485ReceiveMode()
    {
        GPIOA->BSRR = GPIO_BSRR_BR8;
    }
}

volatile std::uint16_t baud_rate = 0U;
volatile std::uint8_t uart_tx_busy = 0U;
volatile std::uint8_t rtu_frame_ready = 0U;
volatile std::uint16_t modbus_t35_steps = 35U;
volatile std::uint16_t modbus_timer_counter = 0U;
volatile std::uint8_t modbus_timer_running = 0U;

UartDriver* UartDriver::active_ = 0;

UartDriver::UartDriver()
    : baudrate_(0U),
      txBusy_(false),
      frameReady_(false),
      rxLength_(0U),
      frameLength_(0U),
      lastByteTimestampUs_(0U),
      rxBuffer_{0},
      frameBuffer_{0}
{
}

UartDriver* UartDriver::active()
{
    return active_;
}

void UartDriver::initialize(std::uint32_t baudrate)
{
    baudrate_ = baudrate;
    active_ = this;
    UART_Init(baudrate);
}

bool UartDriver::send(const std::uint8_t* data, std::size_t length)
{
    if ((data == 0) || (length == 0U) || (length > BufferSize)) {
        return false;
    }

    return (UART_Send(data, static_cast<std::uint16_t>(length)) != 0U);
}

void UartDriver::pollFrameTimeout(std::uint32_t nowUs)
{
    (void)nowUs;
    UART_RTU_Poll();
}

bool UartDriver::frameAvailable()
{
    UART_RTU_Poll();
    return (UART_FrameReady() != 0U);
}

std::size_t UartDriver::fetchFrame(std::uint8_t* destination, std::size_t maxLength)
{
    if ((destination == 0) || (maxLength == 0U)) {
        return 0U;
    }

    if (maxLength > BufferSize) {
        maxLength = BufferSize;
    }

    if (UART_FrameReady() == 0U) {
        return 0U;
    }

    const std::uint16_t len = UART_GetFrame(destination, static_cast<std::uint16_t>(maxLength));
    UART_ClearFrameFlag();

    return static_cast<std::size_t>(len);
}

void UartDriver::simulateReceive(const std::uint8_t* data, std::size_t length)
{
    if ((data == 0) || (length == 0U)) {
        return;
    }

    if (length > BufferSize) {
        length = BufferSize;
    }

    for (std::size_t i = 0U; i < length; ++i) {
        frameBuffer_[i] = data[i];
    }

    frameLength_ = length;
    frameReady_ = true;
}

void UartDriver::onIdleInterrupt()
{
}

void UartDriver::onTxCompleteInterrupt()
{
}

void UartDriver::onErrorInterrupt()
{
    clearRxState();
}

void UartDriver::clearRxState()
{
    rxLength_ = 0U;
    frameLength_ = 0U;
    frameReady_ = false;
}

extern "C" void UART_Init(std::uint32_t baud)
{
    std::memset(static_cast<void*>(rxRing.buf), 0, sizeof(rxRing.buf));
    rxRing.head = 0U;
    rxRing.tail = 0U;

    std::memset(static_cast<void*>(txBuf), 0, sizeof(txBuf));
    std::memset(static_cast<void*>(&txQueue), 0, sizeof(txQueue));

    baud_rate = static_cast<std::uint16_t>(baud);
    uart_tx_busy = 0U;
    rtu_frame_ready = 0U;
    modbus_timer_running = 0U;
    modbus_timer_counter = 0U;
    rtuState = RtuIdle;
    rtuLastHead = 0U;
    rtuFrameHead = 0U;
    rtuDeadline = 0U;

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    (void)RCC->AHB1ENR;
    (void)RCC->APB2ENR;

    /* PA9 = USART1_TX, PA10 = USART1_RX */
    GPIOA->MODER &= ~((3U << (9U * 2U)) | (3U << (10U * 2U)));
    GPIOA->MODER |=  ((2U << (9U * 2U)) | (2U << (10U * 2U)));

    GPIOA->OSPEEDR |= ((3U << (9U * 2U)) | (3U << (10U * 2U)));

    GPIOA->AFR[1] &= ~((0xFU << 4U) | (0xFU << 8U));
    GPIOA->AFR[1] |=  ((7U << 4U) | (7U << 8U));

    /* PA8 = RS485 DE */
    GPIOA->MODER &= ~(3U << (8U * 2U));
    GPIOA->MODER |=  (1U << (8U * 2U));
    rs485ReceiveMode();

    USART1->CR1 = 0U;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;
    USART1->BRR = static_cast<std::uint16_t>(24000000UL / baud);
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_IDLEIE;
    USART1->CR1 |= USART_CR1_UE;

    /* RX DMA: DMA2 Stream2 Channel4, circular */
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    while ((DMA2_Stream2->CR & DMA_SxCR_EN) != 0U) {
    }

    DMA2->LIFCR = (DMA_LIFCR_CTCIF2 |
                   DMA_LIFCR_CHTIF2 |
                   DMA_LIFCR_CTEIF2 |
                   DMA_LIFCR_CDMEIF2 |
                   DMA_LIFCR_CFEIF2);

    DMA2_Stream2->PAR  = dmaAddressOf(&USART1->DR);
    DMA2_Stream2->M0AR = dmaAddressOf(rxRing.buf);
    DMA2_Stream2->NDTR = UART_RX_BUF_SIZE;
    DMA2_Stream2->CR   = (4U << 25U) |
                         (2U << 16U) |
                         DMA_SxCR_MINC |
                         DMA_SxCR_CIRC;
    DMA2_Stream2->CR |= DMA_SxCR_EN;

    USART1->CR3 |= USART_CR3_DMAR;

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    NVIC_SetPriority(USART1_IRQn, 1U);
    NVIC_SetPriority(DMA2_Stream7_IRQn, 2U);
}

extern "C" std::uint8_t UART_Send(const std::uint8_t* data, std::uint16_t len)
{
    if ((data == 0) || (len == 0U) || (len > UART_TX_BUF_SIZE)) {
        return 0U;
    }

    if (uart_tx_busy != 0U) {
        return 0U;
    }

    uart_tx_busy = 1U;
    std::memcpy(static_cast<void*>(txBuf), data, len);

    rs485TransmitMode();

    DMA2_Stream7->CR &= ~DMA_SxCR_EN;
    while ((DMA2_Stream7->CR & DMA_SxCR_EN) != 0U) {
    }

    DMA2->HIFCR = DMA_HIFCR_CTCIF7 |
                  DMA_HIFCR_CHTIF7 |
                  DMA_HIFCR_CTEIF7 |
                  DMA_HIFCR_CDMEIF7 |
                  DMA_HIFCR_CFEIF7;

    DMA2_Stream7->PAR  = dmaAddressOf(&USART1->DR);
    DMA2_Stream7->M0AR = dmaAddressOf(txBuf);
    DMA2_Stream7->NDTR = len;
    DMA2_Stream7->CR   = (4U << 25U) |
                         DMA_SxCR_MINC |
                         DMA_SxCR_DIR_0 |
                         DMA_SxCR_TCIE;

    USART1->CR3 |= USART_CR3_DMAT;
    DMA2_Stream7->CR |= DMA_SxCR_EN;

    return 1U;
}

extern "C" std::uint16_t UART_RxAvailable(void)
{
    std::uint16_t head;
    std::uint16_t tail;

    do {
        rxRing.head = dmaHead();
        tail = rxRing.tail;
        head = dmaHead();
    } while (rxRing.head != head);

    if (head >= tail) {
        return static_cast<std::uint16_t>(head - tail);
    }

    return static_cast<std::uint16_t>(UART_RX_BUF_SIZE - tail + head);
}

extern "C" std::uint16_t UART_RxRead(std::uint8_t* dst, std::uint16_t len)
{
    if (dst == 0) {
        return 0U;
    }

    std::uint16_t available = UART_RxAvailable();
    if (len > available) {
        len = available;
    }

    for (std::uint16_t i = 0U; i < len; ++i) {
        dst[i] = rxRing.buf[rxRing.tail++];
        if (rxRing.tail >= UART_RX_BUF_SIZE) {
            rxRing.tail = 0U;
        }
    }

    return len;
}

extern "C" std::uint16_t UART_GetFrame(std::uint8_t* dst, std::uint16_t max_len)
{
    if (dst == 0) {
        return 0U;
    }

    std::uint16_t head = rtuFrameHead;
    std::uint16_t tail = rxRing.tail;
    std::uint16_t len;

    if (head >= tail) {
        len = static_cast<std::uint16_t>(head - tail);
    } else {
        len = static_cast<std::uint16_t>(UART_RX_BUF_SIZE - tail + head);
    }

    if (len == 0U) {
        return 0U;
    }

    if (len > max_len) {
        len = max_len;
    }

    for (std::uint16_t i = 0U; i < len; ++i) {
        dst[i] = rxRing.buf[rxRing.tail++];
        if (rxRing.tail >= UART_RX_BUF_SIZE) {
            rxRing.tail = 0U;
        }
    }

    return len;
}

extern "C" std::uint8_t UART_FrameReady(void)
{
    return rtu_frame_ready;
}

extern "C" void UART_ClearFrameFlag(void)
{
    rtu_frame_ready = 0U;
}

extern "C" void UART_RTU_Poll(void)
{
    std::uint16_t head = dmaHead();
    std::uint32_t now = GetTick();

    switch (rtuState) {
    case RtuIdle:
        if (rtu_frame_ready != 0U) {
            break;
        }

        if (head != rxRing.tail) {
            rtuState = RtuReceiving;
            rtuLastHead = head;
            rtuDeadline = now + 3U;
        }
        break;

    case RtuReceiving:
        if (head != rtuLastHead) {
            rtuLastHead = head;
            rtuDeadline = now + 3U;
        } else {
            if ((int32_t)(now - rtuDeadline) >= 0) {
                rtuFrameHead = head;
                rtu_frame_ready = 1U;
                rtuState = RtuIdle;
            }
        }
        break;

    default:
        rtuState = RtuIdle;
        break;
    }
}

extern "C" void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_SR_RXNE) != 0U) {
        (void)USART1->DR;
        modbus_timer_running = 0U;
        modbus_timer_counter = 0U;
    }

    if ((USART1->SR & USART_SR_IDLE) != 0U) {
        (void)USART1->SR;
        (void)USART1->DR;

        rtuFrameHead = dmaHead();
        modbus_timer_counter = 0U;
        modbus_timer_running = 1U;

        if (baud_rate > 19200U) {
            modbus_t35_steps = 18U;
        } else {
            modbus_t35_steps = static_cast<std::uint16_t>((38500000UL / baud_rate) / 50UL);
        }
    }

    if ((USART1->SR & USART_SR_TC) != 0U) {
        (void)USART1->SR;
        (void)USART1->DR;

        USART1->CR1 &= ~USART_CR1_TCIE;
        rs485ReceiveMode();
        uart_tx_busy = 0U;
    }
}

extern "C" void DMA2_Stream7_IRQHandler(void)
{
    if ((DMA2->HISR & DMA_HISR_TCIF7) != 0U) {
        DMA2->HIFCR = DMA_HIFCR_CTCIF7;
        USART1->CR1 |= USART_CR1_TCIE;
    }
}
