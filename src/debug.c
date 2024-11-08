#include "debug.h"

void printk(const char *format, ...)
{
    char buffer[100];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    uart_send_buffer(USART2, (uint8_t *)buffer, sizeof(buffer));
}