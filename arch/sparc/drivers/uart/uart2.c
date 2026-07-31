#include <stdint.h>
#include <sparc.h>

void uart_put_char(char c) {
    while ((*UART_STATUS & UART_STS_TE) == 0);
    
    *UART_DATA = (uint32_t)c;
}

void uart_printk(const char* str) {
    while (*str) {
        if (*str == '\n') {
            uart_put_char('\r');
        }
        uart_put_char(*str++);
    }
}

char uart_get_char() {
    if ((*UART_STATUS & UART_STS_DR) == 0) {
        return 0; 
    }
    
    return (char)(*UART_DATA & 0xFF);
}
