#ifndef SPARC_H
#define SPARC_H

#define LEON3_UART_BASE 0x80000100
#define UART_DATA   ((volatile uint32_t*)(LEON3_UART_BASE + 0x00))
#define UART_STATUS ((volatile uint32_t*)(LEON3_UART_BASE + 0x04))
#define UART_STS_DR (1 << 0) 
#define UART_STS_TS (1 << 1)
#define UART_STS_TE (1 << 2)
#define UART_CTRL   ((volatile uint32_t*)(LEON3_UART_BASE + 0x08))

void uart_put_char(char c);
void uart_printk(const char* str);
char uart_get_char();

void get_string(char *buffer);

static inline uint32_t read_psr(void) {
    uint32_t value;
    asm volatile("rd %%psr, %0" : "=r"(value));
    return value;
}

#define read_mhartid() (0)

#define read_mstatus() read_psr()
#define read_mie()     ((read_psr() >> 8) & 0xF)

#define NULL ((void*)0)

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_id;   
    uint8_t subclass; 
    uint8_t prog_if;   
} pci_device_t;

void pci_print_devices();
void pci_scan();
uint32_t pci_read_config_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_write_config_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value);
uint16_t pci_read_config_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_write_config_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value);
uint8_t pci_read_config_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pci_find_device(uint16_t vendor_id, uint16_t device_id, uint8_t bar_offset);

#define GRPCI_CONFIG_ADDR ((volatile uint32_t *)0x80000D00)
#define GRPCI_CONFIG_DATA ((volatile uint32_t *)0x80000D04)

void itoa2(int n, char* s);
void htoa2(int n, char str[]);

#define LEON3_TIMER_BASE  0x80000300
#define LEON3_TIMER_CTRL  ((volatile uint32_t*)(LEON3_TIMER_BASE + 0x18))
#define LEON3_TIMER_RELOAD ((volatile uint32_t*)(LEON3_TIMER_BASE + 0x14))
#define LEON3_TIMER_VAL    ((volatile uint32_t*)(LEON3_TIMER_BASE + 0x10))

#define LEON3_IRQMP_BASE  0x80000200
#define LEON3_IRQMASK     ((volatile uint32_t*)(LEON3_IRQMP_BASE + 0x40))

#define TIMER_CTRL_ENABLE  (1 << 0) 
#define TIMER_CTRL_RELOAD  (1 << 1) 
#define TIMER_CTRL_IRQEN   (1 << 2)
#define TIMER_CTRL_LOAD    (1 << 3)

#define TIMER_FREQUENCY_HZ 1000000 

int cmp_strings(const char *str1, const char *str2);

extern unsigned int timer_ticks;
void init_timer();

#define PCI_VENDOR_TABLE_SIZE (sizeof(pci_vendor_table) / sizeof(pci_vendor_table[0]))

#define TIMER_CTRL_IP (1 << 4) 

#endif
