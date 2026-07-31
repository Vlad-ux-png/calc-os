#include <stdint.h>
#include <riscv.h>
#include <mm.h>
#include <coms.h>

char com[512];
char buffer[4096];

void system_riscv(uint32_t hartid, uint32_t dtb_ptr) {
    init_memory_manager();
    pci_scan();
	init_timer();
    
    while(1) {
        uart_printk("> ");
        get_string(com);

        uart_printk("\n");

        if (cmp_strings(com, "help")) {
            help();
        }
        else if (cmp_strings(com, "status")) {
			uint32_t mstatus = read_mstatus();
            uint32_t mie = read_mie();
            uint32_t hartid = read_mhartid(); 

            char buf[32];
    
            uart_printk("Current Hart (Core) ID: ");
            itoa2(hartid, buf); uart_printk(buf); uart_printk("\n");

            uart_printk("mstatus register: ");
            htoa2(mstatus, buf); uart_printk(buf); uart_printk("\n");

            uart_printk("Global Interrupts: ");
            if (mstatus & (1 << 3)) {
                uart_printk("ENABLED\n");
            } else {
                uart_printk("DISABLED\n");
            }

            uint8_t mpp = (mstatus >> 11) & 0x3;
            uart_printk("Previous Privilege Mode: ");
            if (mpp == 3) uart_printk("Machine Mode (M-Mode)\n");
            else if (mpp == 1) uart_printk("Supervisor Mode (S-Mode)\n");
            else uart_printk("User Mode (U-Mode)\n");

            uart_printk("Enabled Interrupts (mie): ");
            htoa2(mie, buf); uart_printk(buf); uart_printk("\n\n");
        }
        else if (cmp_strings(com, "reboot")) {
            volatile uint32_t *sifive_finisher = (volatile uint32_t *)0x100000;
            *sifive_finisher = 0x5555;

            volatile uint32_t *syscon_reset = (volatile uint32_t *)0x1000000;
            *syscon_reset = 0x7777; 

            volatile uint32_t *watchdog_ctrl = (volatile uint32_t *)0x10007000;
            if (watchdog_ctrl) {
                *watchdog_ctrl = 0x1; 
            }

            asm volatile("csrw mtvec, zero");
    
            asm volatile("ebreak");
            
            while(1) {
                asm volatile("wfi"); 
            }
        }
        else if (cmp_strings(com, "devices")) {
            pci_print_devices();
        }
		else if (cmp_strings(com, "lifetime")) {
			itoa2(timer_ticks, buffer);
			uart_printk(buffer);
            uart_printk("\n");
        }
		else {
			if (com[0] != '\0') {
				uart_printk("Unknown command. Type 'help'\n");
			}
		}
	};
}
 
