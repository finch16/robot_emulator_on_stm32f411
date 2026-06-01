#ifndef H_UART_DRIVER_H
#define H_UART_DRIVER_H

#include <stdint.h>

void init_uart_driver();

void uart_send_char(char ch);

void uart_send_hex(uint32_t value)
{
    for(int i = 7; i >= 0; i--)
    {
        uint8_t nibble = (value >> (i * 4)) & 0xF;
        uart_send_char(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);
    }
}

void uart_send_string(const char* str)
{
    while(*str) uart_send_char(*str++);
}

void uart_send_data(const uint8_t* data, const uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        uart_send_char(data[i]);
    }
}

#endif //H_UART_DRIVER_H
