#include <stdint.h>
#include <riscv.h>
#include <mm.h>
#include <coms.h>
#include <keyboard.h>
#include <utils.h>
#include <video.h>

char com[512];
char buffer[4096];

char name[128];
char content[512];

int current_uid = 1; 
const char* root_password = "102030!";

void system_riscv(uint32_t hartid, uint32_t dtb_ptr) {
    init_memory_manager();
    pci_scan();
	init_timer();
    sd_init();
    
    while(1) {
        if (current_uid == 0) {
            print("# ", COLOR_WHITE);
        } else {
            print("$ ", COLOR_WHITE);
        }

        input_wait_string(com);

        printk("\n", COLOR_WHITE);

        char *com2 = strtok(com, " ");

        if (com2 != NULL) {
            if (compare_strings(com, "help")) {
                help();
            }
            else if (compare_strings(com, "status")) {
                uint32_t mstatus = read_mstatus();
                uint32_t mie = read_mie();
                uint32_t hartid = read_mhartid(); 

                char buf[32];
        
                printk("Current Hart (Core) ID: ", COLOR_WHITE);
                itoa(hartid, buf); printk(buf, COLOR_WHITE); printk("\n", COLOR_WHITE);

                printk("mstatus register: ", COLOR_WHITE);
                htoa(mstatus, buf); printk(buf, COLOR_WHITE); printk("\n", COLOR_WHITE);

                printk("Global Interrupts: ", COLOR_WHITE);
                if (mstatus & (1 << 3)) {
                    printk("ENABLED\n", COLOR_WHITE);
                } else {
                    printk("DISABLED\n", COLOR_WHITE);
                }

                uint8_t mpp = (mstatus >> 11) & 0x3;
                printk("Previous Privilege Mode: ", COLOR_WHITE);
                if (mpp == 3) printk("Machine Mode (M-Mode)\n", COLOR_WHITE);
                else if (mpp == 1) printk("Supervisor Mode (S-Mode)\n", COLOR_WHITE);
                else printk("User Mode (U-Mode)\n", COLOR_WHITE);

                printk("Enabled Interrupts (mie): ", COLOR_WHITE);
                htoa(mie, buf); printk(buf, COLOR_WHITE); printk("\n\n", COLOR_WHITE);
            }
            else if (compare_strings(com, "reset")) {
                if (current_uid != 0) {
                    printk("reset: Permission denied\n", COLOR_WHITE);
                    continue;
                }

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
            else if (compare_strings(com, "devices")) {
                pci_print_devices();
            }
            else if (compare_strings(com, "lifetime")) {
                itoa(timer_ticks, buffer);
                printk(buffer, COLOR_WHITE);
                printk("\n", COLOR_WHITE);
            }
            else if (compare_strings(com, "su")) {
               su();
            }
            else if (compare_strings(com, "exit")) {
                exit2();
            }
            else if (compare_strings(com, "whoami")) {
                if (current_uid != 0) {
                    printk("user\n", COLOR_WHITE);
                } else {
                    printk("root\n", COLOR_WHITE);
                }
            }
            else {
                if (com[0] != '\0') {
                    printk("Unknown command. Type 'help'\n", COLOR_WHITE);
                }
            }
        }
	};
}
 
