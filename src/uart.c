#include "uart.h"

#define MMIO_BASE   0x3F000000UL

static inline void mmio_write(unsigned long addr, unsigned int val) {
    *(volatile unsigned int *)addr = val;
}
static inline unsigned int mmio_read(unsigned long addr) {
    return *(volatile unsigned int *)addr;
}

/* GPIO registers */
#define GPFSEL1     (MMIO_BASE + 0x00200004)
#define GPPUD       (MMIO_BASE + 0x00200094)
#define GPPUDCLK0   (MMIO_BASE + 0x00200098)

/* PL011 UART0 registers */
#define UART0_BASE  (MMIO_BASE + 0x00201000)
#define UART0_DR    (UART0_BASE + 0x00)
#define UART0_FR    (UART0_BASE + 0x18)
#define UART0_IBRD  (UART0_BASE + 0x24)
#define UART0_FBRD  (UART0_BASE + 0x28)
#define UART0_LCRH  (UART0_BASE + 0x2C)
#define UART0_CR    (UART0_BASE + 0x30)
#define UART0_ICR   (UART0_BASE + 0x44)

static void delay(unsigned int count) {
    while (count--) asm volatile("nop");
}

void uart_init(void) {
    mmio_write(UART0_CR, 0);

    mmio_write(GPPUD, 0);
    delay(150);
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    mmio_write(GPPUDCLK0, 0);

    unsigned int sel = mmio_read(GPFSEL1);
    sel &= ~((7 << 12) | (7 << 15));
    sel |=  (4 << 12) | (4 << 15);
    mmio_write(GPFSEL1, sel);

    mmio_write(UART0_ICR, 0x7FF);
    mmio_write(UART0_IBRD, 26);
    mmio_write(UART0_FBRD,  3);
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(char c) {
    while (mmio_read(UART0_FR) & (1 << 5));
    mmio_write(UART0_DR, (unsigned int)c);
}

void uart_puts(const char *s) {
    while (*s) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s++);
    }
}

void uart_put_uint(unsigned long n) {
    if (n == 0) { uart_putc('0'); return; }
    char buf[20];
    int i = 0;
    while (n) { buf[i++] = '0' + (n % 10); n /= 10; }
    while (i--) uart_putc(buf[i]);
}
