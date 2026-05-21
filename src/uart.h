#pragma once

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_uint(unsigned long n); 