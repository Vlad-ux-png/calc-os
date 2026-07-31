#include <cmos.h>
#include <video.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>
#include <stdint.h>

void write(uint8_t addr, uint8_t value) {
    __asm__ volatile ("cli");
    outb(0x70, addr | 0x80);  
    outb(0x71, value);
    __asm__ volatile ("sti");
}

uint8_t read(uint8_t addr) {
    uint8_t res;
    __asm__ volatile ("cli");
    
    outb(0x70, addr | 0x80);  
    res = inb(0x71);  
    
    __asm__ volatile ("sti"); 
    return res;
}

uint8_t check_battery() {
    uint8_t status = read(0x0D);
    if (!(status & 0x80)) {
        return 0; 
    }
    return 1; 
}

uint8_t bcd_to_bin(uint8_t val) {
    return (val & 0x0F) + ((val >> 4) * 10);
}

void get_time(int *h, int *m) {
    while (read(0x0A) & 0x80);

    uint8_t raw_h = read(0x04);
    uint8_t raw_m = read(0x02);

    *h = bcd_to_bin(raw_h);
    *m = bcd_to_bin(raw_m);
}