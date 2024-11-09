#include "debug.h"

void printk(const char *format, ...)
{
    char buffer[100] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    UART_SendBuffer(USART2, (uint8_t *)buffer, sizeof(buffer));
}