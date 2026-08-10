#include <stdint.h>
#include <riscv.h>
#include <mm.h>
#include <coms.h>
#include <keyboard.h>
#include <utils.h>
#include <video.h>

pci_device_t devices[32];
int device_count = 0;

typedef struct {
    uint16_t id;
    const char *name;
} pci_vendor_entry_t;

static const pci_vendor_entry_t pci_vendor_table[] = {
    { 0x0E11, "Compaq Computer Corp." },
    { 0x1000, "LSI Logic / Symbios Logic" },
    { 0x1002, "AMD / ATI Technologies" },
    { 0x1013, "Cirrus Logic" },
    { 0x1022, "Advanced Micro Devices, Inc. [AMD]" },
    { 0x1025, "Acer Incorporated" },
    { 0x1028, "Dell Technologies" },
    { 0x103C, "Hewlett-Packard Company" },
    { 0x1043, "ASUSTeK Computer Inc." },
    { 0x104C, "Texas Instruments" },
    { 0x1055, "Efar Microsystems" },
    { 0x106B, "Apple Inc." },
    { 0x1077, "QLogic Corp." },
    { 0x108E, "Oracle/Sun Microsystems" },
    { 0x10B7, "3Com Corporation" },
    { 0x10DE, "NVIDIA Corporation" },
    { 0x10EC, "Realtek Semiconductor Corp." },
    { 0x10F1, "Tyan Computer" },
    { 0x1106, "VIA Technologies, Inc." },
    { 0x111D, "Microsemi / IDT" },
    { 0x1137, "Cisco Systems, Inc." },
    { 0x1179, "Toshiba Corporation" },
    { 0x11AB, "Marvell Technology Group Ltd." },
    { 0x1234, "Technical Corp. / QEMU Standard VGA" },
    { 0x1264, "AMCC / Applied Micro" },
    { 0x1337, "Hackers / Test Vendor ID" },
    { 0x13B5, "ARM Ltd." },
    { 0x1414, "Microsoft Corporation" },
    { 0x144D, "Samsung Electronics Co Ltd" },
    { 0x14E4, "Broadcom Inc." },
    { 0x152D, "JMicron Technology Corp." },
    { 0x15AD, "VMware Inc." },
    { 0x15B3, "Mellanox Technologies" },
    { 0x168C, "Qualcomm Atheros" },
    { 0x177D, "Cavium, Inc." },
    { 0x17AA, "Lenovo" },
    { 0x18A3, "FPGA / Custom Vendor" },
    { 0x1969, "Qualcomm Atheros" },
    { 0x19A2, "Emulex Corporation" },
    { 0x1AF4, "Red Hat, Inc. (VirtIO)" },
    { 0x1B21, "ASMedia Technology Inc." },
    { 0x1B36, "Red Hat, Inc. (QEMU PCIe)" },
    { 0x1C5C, "SK Hynix" },
    { 0x1CC1, "ADATA Technology Co., Ltd." },
    { 0x1D0F, "Amazon.com, Inc." },
    { 0x1D87, "Rockchip Electronics" },
    { 0x1E0F, "Kioxia Corporation" },
    { 0x5143, "Qualcomm Inc." },
    { 0x8086, "Intel Corporation" },
    { 0x80EE, "Oracle Corporation (VirtualBox)" },
    { 0x9005, "Adaptec" },
    { 0xA0F0, "Sony Corporation" },
    { 0x1217, "O2 Micro, Inc." },
    { 0x16C3, "Synopsys, Inc." },
    { 0x174B, "PC Partner Limited / Sapphire" },
    { 0x19E5, "Huawei Technologies Co., Ltd." },
    { 0x1C1F, "SoftBank / ARM" }
};

uint32_t pci_read_config_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    volatile uint32_t *ptr = (volatile uint32_t *)(RISCV_ECAM_BASE + ((uint32_t)bus << 20) + ((uint32_t)slot << 15) + ((uint32_t)func << 12) + (offset & 0xFC));
    return *ptr;
}

void pci_write_config_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value) {
    volatile uint32_t *ptr = (volatile uint32_t *)(RISCV_ECAM_BASE + ((uint32_t)bus << 20) + ((uint32_t)slot << 15) + ((uint32_t)func << 12) + (offset & 0xFC));
    *ptr = value;
}

uint16_t pci_read_config_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    volatile uint16_t *ptr = (volatile uint16_t *)(RISCV_ECAM_BASE + ((uint32_t)bus << 20) + ((uint32_t)slot << 15) + ((uint32_t)func << 12) + (offset & 0xFC));
    return *ptr;
}

void pci_write_config_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value) {
    volatile uint16_t *ptr = (volatile uint16_t *)(RISCV_ECAM_BASE + ((uint32_t)bus << 20) + ((uint32_t)slot << 15) + ((uint32_t)func << 12) + (offset & 0xFC));
    *ptr = value;
}

uint8_t pci_read_config_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    volatile uint8_t *ptr = (volatile uint8_t *)(RISCV_ECAM_BASE + ((uint32_t)bus << 20) + ((uint32_t)slot << 15) + ((uint32_t)func << 12) + (offset & 0xFC));
    return *ptr;
}

int check_device(uint8_t bus, uint8_t slot, uint8_t func) {
    uint32_t vendor_id = pci_read_config_dword(bus, slot, func, 0) & 0xFFFF;
    if (vendor_id == 0xFFFF) {
        return 0; 
    }
    return 1;
}

uint32_t pci_find_device(uint16_t vendor_id, uint16_t device_id, uint8_t bar_offset) {
    for (int i = 0; i < device_count; i++) {
        if (devices[i].vendor_id == vendor_id && devices[i].device_id == device_id) {
            return pci_read_config_dword(devices[i].bus, devices[i].slot, devices[i].func, bar_offset);
        }
    }
    return 0xFFFFFFFF; 
}

void pci_scan() {
    device_count = 0; 

    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            if (check_device(bus, slot, 0)) {
                
                uint32_t reg3 = pci_read_config_dword(bus, slot, 0, 3);
                uint8_t header_type = (reg3 >> 16) & 0xFF;
                
                uint8_t max_functions = 1;
                if (header_type & 0x80) {
                    max_functions = 8;
                }
                
                for (uint8_t func = 0; func < max_functions; func++) {
                    if (check_device(bus, slot, func)) {
                        if (device_count >= 32) {
                            return; 
                        }

                        uint32_t data0 = pci_read_config_dword(bus, slot, func, 0);
                        uint32_t data2 = pci_read_config_dword(bus, slot, func, 2);

                        devices[device_count].bus = (uint8_t)bus;
                        devices[device_count].slot = slot;
                        devices[device_count].func = func;
                        devices[device_count].vendor_id = data0 & 0xFFFF;
                        devices[device_count].device_id = (data0 >> 16) & 0xFFFF;

                        devices[device_count].class_id = (data2 >> 24) & 0xFF;
                        devices[device_count].subclass   = (data2 >> 16) & 0xFF;
                        devices[device_count].prog_if = pci_read_config_byte(bus, slot, func, 0x09);

                        device_count++;
                    }
                }
            }
        }
    }
}

const char* pci_vendor_to_string(uint16_t vendor_id) {
    for (size_t i = 0; i < PCI_VENDOR_TABLE_SIZE; i++) {
        if (pci_vendor_table[i].id == vendor_id) {
            return pci_vendor_table[i].name;
        }
    }
    return "Unknown Vendor";
}

const char* pci_class_to_string(uint8_t class_id) {
    switch (class_id) {
        case 0x00: return "Unclassified Device";
        case 0x01: return "Mass Storage Controller";
        case 0x02: return "Network Controller";
        case 0x03: return "Display Controller";
        case 0x04: return "Multimedia Controller";
        case 0x05: return "Memory Controller";
        case 0x06: return "Bridge Device";
        case 0x07: return "Simple Communication Controller";
        case 0x08: return "Base System Peripherals";
        case 0x09: return "Input Device Controller";
        case 0x0A: return "Docking Station";
        case 0x0B: return "Processor";
        case 0x0C: return "Serial Bus Controller (USB/FireWire/SMBus)";
        case 0x0D: return "Wireless Controller";
        case 0x0E: return "Intelligent Controller";
        case 0x0F: return "Satellite Communication Controller";
        case 0x10: return "Encryption Controller";
        case 0x11: return "Signal Processing Controller";
        case 0x12: return "Processing Accelerator";
        case 0x13: return "Non-Essential Instrumentation";
        case 0x40: return "Co-Processor";
        default:   return "Unknown Device Class";
    }
}

void pci_print_devices() {
    if (device_count == 0) {
        printk("No PCI devices found.\n", COLOR_WHITE);
        return;
    }

    for (int i = 0; i < device_count; i++) {
        char buf[16];
        
        printk("[", COLOR_WHITE);
        itoa(devices[i].bus, buf); printk(buf, COLOR_WHITE); printk(":", COLOR_WHITE);
        itoa(devices[i].slot, buf); printk(buf, COLOR_WHITE); printk(".", COLOR_WHITE);
        itoa(devices[i].func, buf); printk(buf, COLOR_WHITE); printk("] ", COLOR_WHITE);
        
        printk(pci_class_to_string(devices[i].class_id), COLOR_WHITE);
        printk("\n", COLOR_WHITE);

        printk("  Vendor: ", COLOR_WHITE); 
        htoa(devices[i].vendor_id, buf); printk(buf, COLOR_WHITE);
        printk(" (", COLOR_WHITE); printk(pci_vendor_to_string(devices[i].vendor_id), COLOR_WHITE); printk(")", COLOR_WHITE);
        printk("\n", COLOR_WHITE);

        printk(" | Device ID: ", COLOR_WHITE);
        htoa(devices[i].device_id, buf); printk(buf, COLOR_WHITE);
        printk("\n", COLOR_WHITE);

        printk(" | Class: ", COLOR_WHITE);
        htoa(devices[i].class_id, buf); printk(buf, COLOR_WHITE);
        printk("\n", COLOR_WHITE);
    }
}
