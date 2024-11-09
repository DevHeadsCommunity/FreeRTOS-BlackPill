#ifndef __INC_UART_H__
#define __INC_UART_H__

#include "stm32f411xe.h"

typedef enum
{
    W8_BTIS = 0,
    W9_BITS = 1
} uart_word_len;

typedef enum
{
    BOTH = 0,
    TX_ONLY = 1,
    RX_ONLY = 2

} uart_mode;

typedef enum
{
    NO_PARITY = 0U,
    EVEN_PARITY = 1U,
    ODD_PARITY = 2U

} uart_parity;

typedef struct
{
    USART_TypeDef *p_usart;
    uint32_t baud_rate;
    uint32_t mode;
    uint32_t parity;
    uint32_t word_len;
} UARTConfig_t;

void UART_Init(UARTConfig_t *config);

void UART1_GPIO_Init();
void UART2_GPIO_Init();
void UART6_GPIO_Init();

void UART_SendChar(USART_TypeDef *p_usart, uint8_t ch);

void UART_SendBuffer(USART_TypeDef *p_usart, uint8_t *p_buffer, uint8_t len);

void UART_SendBufferIT(USART_TypeDef *p_usart, uint8_t *p_buffer, uint8_t len);

void UART_SendBufferDMA(USART_TypeDef *p_usart, uint8_t *p_buffer, uint8_t len);

void UART_RecvChar();

void UART_RecvBuffer();

void UART_RecvBufferIT();

void UART_RecvBufferDMA();

#endif