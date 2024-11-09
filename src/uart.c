#include "uart.h"
#include "gpio.h"

extern uint32_t SystemCoreClock;

static inline void UART_ClockEnable(USART_TypeDef *p_usart)
{
    if (p_usart == USART1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if (p_usart == USART2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
    else if (p_usart == USART6)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    }
}

static inline void UART_Enable(USART_TypeDef *p_usart)
{
    p_usart->CR1 |= USART_CR1_UE;
}

static inline void UART_Disable(USART_TypeDef *p_usart)
{
    p_usart->CR1 &= ~USART_CR1_UE;
}

static inline void UART_ModeConfig(USART_TypeDef *p_usart, uart_mode mode)
{
    if (mode == BOTH)
    {
        p_usart->CR1 |= USART_CR1_TE | USART_CR1_RE;
    }
    else if (mode == TX_ONLY)
    {
        p_usart->CR1 |= USART_CR1_TE;
        p_usart->CR1 &= ~USART_CR1_RE;
    }
    else if (mode == RX_ONLY)
    {
        p_usart->CR1 &= ~USART_CR1_TE;
        p_usart->CR1 |= USART_CR1_RE;
    }
}

static inline void UART_BaudRateConfig(USART_TypeDef *p_usart, uint32_t br)
{
    uint32_t usart_div = SystemCoreClock / br;
    p_usart->BRR |= ((usart_div / 16U) << USART_BRR_DIV_Mantissa_Pos) | ((usart_div / 16U) << USART_BRR_DIV_Fraction_Pos);
}

static inline void UART_ParityConfig(USART_TypeDef *p_usart, uart_parity parity)
{
    if (parity == NO_PARITY)
    {
        p_usart->CR1 &= ~USART_CR1_PCE;
    }
    else if (parity == EVEN_PARITY)
    {
        p_usart->CR1 |= USART_CR1_PCE;
        p_usart->CR1 &= ~USART_CR1_PS;
    }
    else if (parity == ODD_PARITY)
    {
        p_usart->CR1 |= USART_CR1_PCE | USART_CR1_PS;
    }
}

static inline void UART_WordLenConfig(USART_TypeDef *p_usart, uart_word_len w_len)
{
    if (w_len == W8_BTIS)
    {
        p_usart->CR1 &= ~USART_CR1_M;
    }
    if (w_len == W9_BITS)
    {
        p_usart->CR1 |= USART_CR1_M;
    }
}

static inline void UART_ReadyToSend(USART_TypeDef *p_usart)
{
    while (!(p_usart->SR & USART_SR_TXE))
        ;
}

void UART2_GPIO_Init()
{
    GPIOConfig_t Tx, Rx;
    Tx.Pin = 2;
    Tx.Mode = GPIO_MODE_ALTFN;
    Tx.OSpeed = GPIO_OSPEED_HIGH;
    Tx.OType = GPIO_OTYPE_OD;
    Tx.PUPD = GPIO_PUPD_PULLUP;
    Tx.AFn = 7U;

    Rx.Pin = 3;
    Rx.Mode = GPIO_MODE_ALTFN;
    Rx.OSpeed = GPIO_OSPEED_HIGH;
    Rx.OType = GPIO_OTYPE_OD;
    Rx.PUPD = GPIO_PUPD_PULLUP;
    Rx.AFn = 7U;

    GPIO_ClockEnable(GPIOA);
    GPIO_Init(GPIOA, &Tx);
    GPIO_Init(GPIOA, &Rx);
}

void UART_Init(UARTConfig_t *config)
{
    UART_ClockEnable(config->p_usart); // Enable The clock for USART so that it can be modified

    UART_Disable(config->p_usart); // Disable the USART peripheral so that it can be configured

    UART_ModeConfig(config->p_usart, config->mode); // Setting Uart mode, ex: TX_ONLY, RX_ONLY, BOTH

    UART_BaudRateConfig(config->p_usart, config->baud_rate); // Setting Baud Rate for

    UART_ParityConfig(config->p_usart, config->parity); // Setting the parity for better error handle

    UART_WordLenConfig(config->p_usart, config->word_len); // Setting Word Length

    UART_Enable(config->p_usart); // Enabling Uart Peripheral
}

void UART_SendChar(USART_TypeDef *p_usart, uint8_t ch)
{
    UART_ReadyToSend(p_usart);
    p_usart->DR = ch;
}

void UART_SendBuffer(USART_TypeDef *p_usart, uint8_t *p_buffer, uint8_t len)
{
    while (*p_buffer && len)
    {
        UART_SendChar(p_usart, (uint8_t)(*p_buffer));
        p_buffer++;
        len--;
    }
}