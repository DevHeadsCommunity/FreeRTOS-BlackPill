#include "uart.h"

extern uint32_t SystemCoreClock;

static inline void uart_clock_enable(USART_TypeDef *p_usart)
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

static inline void uart_enable(USART_TypeDef *p_usart)
{
    p_usart->CR1 |= USART_CR1_UE;
}

static inline void uart_disable(USART_TypeDef *p_usart)
{
    p_usart->CR1 &= ~USART_CR1_UE;
}

static inline void uart_mode_config(USART_TypeDef *p_usart, uart_mode mode)
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

static inline void uart_baudrate_config(USART_TypeDef *p_usart, uint32_t br)
{
    uint32_t usart_div = SystemCoreClock / br;
    p_usart->BRR |= ((usart_div / 16U) << USART_BRR_DIV_Mantissa_Pos) | ((usart_div / 16U) << USART_BRR_DIV_Fraction_Pos);
}

static inline void uart_parity_config(USART_TypeDef *p_usart, uart_parity parity)
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

static inline void uart_word_len_config(USART_TypeDef *p_usart, uart_word_len w_len)
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

static inline void uart_ready_sent(USART_TypeDef *p_usart)
{
    while (!(p_usart->SR & USART_SR_TXE))
        ;
}

void uart_gpio_init()
{
    // Enabling Clock for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Configuring RX and TX Pin
    GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
    GPIOA->OTYPER |= GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;
    GPIOA->AFR[0] |= 7 << GPIO_AFRL_AFSEL2_Pos | 7 << GPIO_AFRL_AFSEL3_Pos;
}

void uart_init(uart_config_t *config)
{
    uart_clock_enable(config->p_usart); // Enable The clock for USART so that it can be modified

    uart_disable(config->p_usart); // Disable the USART peripheral so that it can be configured

    uart_mode_config(config->p_usart, config->mode); // Setting Uart mode, ex: TX_ONLY, RX_ONLY, BOTH

    uart_baudrate_config(config->p_usart, config->baud_rate); // Setting Baud Rate for

    uart_parity_config(config->p_usart, config->parity); // Setting the parity for better error handle

    uart_word_len_config(config->p_usart, config->word_len); // Setting Word Length

    uart_enable(config->p_usart); // Enabling Uart Peripheral
}

void uart_send_char(USART_TypeDef *p_usart, uint8_t ch)
{
    uart_ready_sent(p_usart);
    p_usart->DR = ch;
}

void uart_send_buffer(USART_TypeDef *p_usart, uint8_t *p_buffer, uint8_t len)
{
    while (*p_buffer && len)
    {
        uart_send_char(p_usart, (uint8_t)(*p_buffer));
        p_buffer++;
        len--;
    }
}