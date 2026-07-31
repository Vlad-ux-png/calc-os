#include <cmos.h>
#include <video.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>
#include <ata.h>
#include <idt.h>
#include <stdint.h>
#include <fat.h>
#include <pci.h>

pci_device_t devices[32];
int device_count = 0;

uint32_t pci_read_config_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(0xCF8, address);
    return inl(0xCFC);
}

void pci_write_config_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value) {
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(0xCF8, address);
    outl(0xCFC, value);
}

uint16_t pci_read_config_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(0xCF8, address);
    return (inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF;
}

void pci_write_config_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value) {
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(0xCF8, address);
    
    uint32_t tmp = inl(0xCFC);
    if ((offset & 2) == 0) {
        tmp = (tmp & 0xFFFF0000) | value;
    } else {
        tmp = (tmp & 0x0000FFFF) | ((uint32_t)value << 16);
    }
    
    outl(0xCFC, tmp);
}

uint8_t pci_read_config_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);

    outl(0xCF8, address);
    uint32_t data = inl(0xCFC);

    return (data >> ((offset & 3) * 8)) & 0xFF;
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

const char* pci_class_to_string(uint8_t class_id) {
    switch (class_id) {
        case 0x00: return "Unclassified Device";
        case 0x01: return "Mass Storage Controller (HDD/SSD/IDE)";
        case 0x02: return "Network Controller (Ethernet/Wi-Fi)";
        case 0x03: return "Display Controller (Video Card)";
        case 0x04: return "Multimedia Controller (Audio/Sound)";
        case 0x05: return "Memory Controller";
        case 0x06: return "Bridge Device (Host/PCI-to-PCI)";
        case 0x07: return "Simple Communication Controller";
        case 0x08: return "Base System Peripherals";
        case 0x0C: return "Serial Bus Controller (USB/FireWire)";
        default:   return "Unknown Device Class";
    }
}

const char* pci_vendor_to_string(uint16_t vendor_id) {
    switch (vendor_id) {
        case 0x8086: return "Intel Corporation";
        case 0x10DE: return "NVIDIA Corporation";
        case 0x1002: return "AMD/ATI Technologies";
        case 0x1013: return "Cirrus Logic";
        case 0x1AF4: return "QEMU/VirtIO Virtual Device";
        case 0x10EC: return "Realtek Semiconductor (LAN/Audio)";
        case 0x14E4: return "Broadcom (Wi-Fi/LAN)";
        case 0x168C: return "Qualcomm Atheros (Wi-Fi)"; 
        case 0x5143: return "Qualcomm Inc.";            
        case 0xA0F0: return "Sony Corporation";
        default:   return "Unknown Vendor";
    }
}

void pci_print_devices() {
    if (device_count == 0) {
        print("No PCI devices found.\n", 15);
        return;
    }

    for (int i = 0; i < device_count; i++) {
        char buf[16];
        
        print("[", 15);
        itoa(devices[i].bus, buf); print(buf, 15); print(":", 15);
        itoa(devices[i].slot, buf); print(buf, 15); print(".", 15);
        itoa(devices[i].func, buf); print(buf, 15); print("] ", 15);
        
        print(pci_class_to_string(devices[i].class_id), 15);
        print("\n", 15);

        print("  Vendor: ", 15); 
        htoa(devices[i].vendor_id, buf); print(buf, 15);
        print(" (", 15); print(pci_vendor_to_string(devices[i].vendor_id), 15); print(")", 15);
        print("\n", 15);

        print(" | Device ID: ", 15);
        htoa(devices[i].device_id, buf); print(buf, 15);
        print("\n", 15);

        print(" | Class: ", 15);
        htoa(devices[i].class_id, buf); print(buf, 15);
        print("\n", 15);
    }
}
