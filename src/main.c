#include "uart.h"

static unsigned int get_current_el(void) {
    unsigned long el;
    asm volatile("mrs %0, CurrentEL" : "=r"(el));
    return (unsigned int)((el >> 2) & 3);
}

void main(void) {
    uart_init();

    uart_puts("\r\n");
    uart_puts("========================================\r\n");
    uart_puts("  RP3 Custom Bootloader — v1.0          \r\n");
    uart_puts("  AArch64 Bare Metal                    \r\n");
    uart_puts("========================================\r\n");

    unsigned int el = get_current_el();
    uart_puts("  Exception Level : EL");
    uart_put_uint(el);
    uart_puts("\r\n");

    uart_puts("  Status          : Boot successful\r\n");
    uart_puts("========================================\r\n");
    uart_puts("  Halting — extend main() to load OS\r\n");
    uart_puts("========================================\r\n");

    while (1) {
        asm volatile("wfe");
    }
} 