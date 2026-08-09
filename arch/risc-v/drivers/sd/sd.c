#include <stdint.h>
#include <riscv.h>
#include <mm.h>
#include <coms.h>
#include <keyboard.h>
#include <utils.h>
#include <video.h>

#define SPI_BASE 0x10000000 
#define REG(off) (*(volatile uint8_t*)(SPI_BASE + off))

uint8_t spi_xfer(unsigned char b) {
    REG(0) = b;                  
    while (REG(1) & 1);          
    return REG(0);               
}

uint8_t sd_cmd(uint8_t cmd, uint32_t arg, uint8_t crc) {
    spi_xfer(0xFF);
  
    spi_xfer(0x40 | cmd);
  
    spi_xfer(arg >> 24); 
    spi_xfer(arg >> 16);
    spi_xfer(arg >> 8);  
  
    spi_xfer(arg);
    spi_xfer(crc);
    
    uint8_t r;
    for (int i = 0; i < 10 && (r = spi_xfer(0xFF)) == 0xFF; i++);
    return r;
}

void sd_init() {
    for (int i = 0; i < 10; i++) spi_xfer(0xFF); 
    sd_cmd(0, 0, 0x95);         
  
    while (sd_cmd(1, 0, 0x01) != 0);            
}

void sd_read_sector(uint32_t lba, uint8_t *buf) {
    sd_cmd(17, lba, 0x01);                       
    while (spi_xfer(0xFF) != 0xFE);   
  
    for (int i = 0; i < 512; i++) buf[i] = spi_xfer(0xFF); 
  
    spi_xfer(0xFF); 
    spi_xfer(0xFF);              
}

void sd_write_sector(uint32_t lba, const uint8_t *buf) {
    sd_cmd(24, lba, 0x01);                    
    spi_xfer(0xFE);             
  
    for (int i = 0; i < 512; i++) spi_xfer(buf[i]); 
  
    spi_xfer(0xFF); 
    spi_xfer(0xFF); 
  
    while ((spi_xfer(0xFF) & 0x1F) != 0x05);     
    while (spi_xfer(0xFF) == 0x00);              
}
