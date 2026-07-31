#ifndef CMOS_H
#define CMOS_H
#include <stdint.h>

void write(uint8_t addr, uint8_t value);
uint8_t read(uint8_t addr);

#endif