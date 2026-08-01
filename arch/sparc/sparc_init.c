#include <stdint.h>
#include <sparc.h>
#include <mm.h>

char com[512];
char buffer[4096];

void system_sparc(uint32_t hartid, uint32_t dtb_ptr) {
    *UART_CTRL = 0x3;
    
	init_memory_manager();
	pci_scan();
	
    while(1) {
        uart_printk("> ");
        get_string(com);

        uart_printk("\n");

        if (cmp_strings(com, "help")) {
            uart_printk("Available commands:\n");
            uart_printk("  help - show this message\n");
            uart_printk("  status - print CPU status\n");
            uart_printk("  reboot - reboot the system\n");
            uart_printk("  devices - print PCI devices\n");
        }
		else if (cmp_strings(com, "status")) {
            uint32_t psr = read_mstatus(); 
            uint32_t pil = read_mie();   
            uint32_t hartid = read_mhartid(); 

            char buf[32];

            uart_printk("Current Hart (Core) ID: ");
            itoa2(hartid, buf); uart_printk(buf); uart_printk("\n");

            uart_printk("PSR register value: ");
            htoa2(psr, buf); uart_printk(buf); uart_printk("\n");

            uart_printk("Global Traps/Interrupts (ET): ");
            if (psr & (1 << 5)) {
                uart_printk("ENABLED\n");
            } else {
                uart_printk("DISABLED\n");
            }

            uart_printk("Current Privilege Mode: ");
            if (psr & (1 << 7)) {
                uart_printk("Supervisor Mode (S-Mode)\n");
            } else {
                uart_printk("User Mode (U-Mode)\n");
            }

            uart_printk("Previous Privilege Mode: ");
            if (psr & (1 << 6)) {
                uart_printk("Supervisor Mode (S-Mode)\n");
            } else {
                uart_printk("User Mode (U-Mode)\n");
            }

            uart_printk("Processor Interrupt Level (PIL): ");
            itoa2(pil, buf); uart_printk(buf); uart_printk("\n\n");
        }
        else if (cmp_strings(com, "reboot")) {
            uint32_t psr;
            asm volatile(
                "rd %%psr, %0\n\t"
                "andn %0, 0x20, %0\n\t" 
                "wr %0, 0x0, %%psr\n\t"
                "nop; nop; nop\n\t"
                : "=r"(psr)
            );

            asm volatile(
                "ta 0\n\t" 
                "nop\n\t"
            );

            while(1);
        }
		else if (cmp_strings(com, "devices")) {
			pci_print_devices();
		}
		else {
			if (com[0] != '\0') {
				uart_printk("Unknown command. Type 'help'\n");
			}
	    }
	};
}
 
