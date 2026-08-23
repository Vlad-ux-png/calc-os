#include <video.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>
#include <idt.h>
#include <task.h>
#include <stdint.h>
#include <sound.h>
#include <forth.h>

void exception_handler(struct registers *regs) {
    if (current_task != 0) {
        printk("\nprocess with PID ", COLOR_WHITE);
        char num_buf[16];
        itoa(current_task, num_buf);
        printk(num_buf, COLOR_WHITE);
        printk(" crashed.\n", COLOR_WHITE);
        
        task_list[current_task].is_active = 0;

        if (current_task == 3) {
            is_crushed = 1;
            prepare_task4();       
            task_list[3].is_active = 1; 
        }

        outb(0x20, 0x20);
        outb(0xA0, 0x20);

        int next_task = current_task;
        while (1) {
            next_task = (next_task + 1) % 4;
            if (next_task == 0 || task_list[next_task].is_active == 1) {
                break;
            }
        }

        current_task = next_task;
        timer_ticks++;

        uint32_t new_stack = (uint32_t)task_list[current_task].esp;

        __asm__ __volatile__("mov %0, %%eax" : : "r"(new_stack));
        return; 
    }
    
    draw_rect(0, 0, 1024, 768, COLOR_BLUE); 

    x = 10;
    y = 10;
    
    printk("KERNEL FATAL: ", COLOR_WHITE);
    
    if (regs->int_no == 0) printk("DIVISION BY ZERO", COLOR_WHITE);
    else if (regs->int_no == 1) printk("DEBUG", COLOR_WHITE);
    else if (regs->int_no == 2) printk("NON MASKABLE INTERRUPT", COLOR_WHITE);
    else if (regs->int_no == 3) printk("BREAKPOINT", COLOR_WHITE);
    else if (regs->int_no == 4) printk("INTO DETECTED OVERFLOW", COLOR_WHITE);
    else if (regs->int_no == 5) printk("OUT OF BOUNDS", COLOR_WHITE);
    else if (regs->int_no == 6) printk("INVALID OPCODE", COLOR_WHITE);
    else if (regs->int_no == 7) printk("NO COPROCESSOR", COLOR_WHITE);
    else if (regs->int_no == 8) printk("DOUBLE FAULT", COLOR_WHITE);
    else if (regs->int_no == 9) printk("COPROCESSOR SEGMENT OVERRUN", COLOR_WHITE);
    else if (regs->int_no == 10) printk("BAD TSS", COLOR_WHITE);
    else if (regs->int_no == 11) printk("SEGMENT NOT PRESENT", COLOR_WHITE);
    else if (regs->int_no == 12) printk("STACK FAULT", COLOR_WHITE);
    else if (regs->int_no == 13) printk("GENERAL PROTECTION FAULT", COLOR_WHITE);
    else if (regs->int_no == 14) printk("PAGE FAULT", COLOR_WHITE);
    else if (regs->int_no == 15) printk("UNKNOWN INTERRUPT (RESERVED)", COLOR_WHITE);
    else if (regs->int_no == 16) printk("FLOATING POINT ERROR", COLOR_WHITE);
    else if (regs->int_no == 17) printk("ALIGNMENT CHECK", COLOR_WHITE);
    else if (regs->int_no == 18) printk("MACHINE CHECK", COLOR_WHITE);
    else if (regs->int_no == 19) printk("SIMD FLOATING POINT EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 20) printk("VIRTUALIZATION EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 21) printk("CONTROL PROTECTION EXCEPTION", COLOR_WHITE);
    else if (regs->int_no >= 22 && regs->int_no <= 27) printk("RESERVED EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 28) printk("HYPERVISOR INJECTION EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 29) printk("VMM COMMUNICATION EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 30) printk("SECURITY EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 31) printk("RESERVED EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 29) printk("VMM COMMUNICATION EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 30) printk("SECURITY EXCEPTION", COLOR_WHITE);
    else if (regs->int_no == 31) printk("RESERVED EXCEPTION", COLOR_WHITE);
    else printk("UNKNOWN EXCEPTION\n", COLOR_WHITE);

    x = 10;
    y = 70;
    printk("TECHICAL INFORMATION: ", COLOR_WHITE);
    char buf[16];
    itoa(regs->int_no, buf);
    printk("\n  INTERRUPT NO: ", COLOR_WHITE);
    printk(buf, COLOR_WHITE); 


    printk("\n  EIP: ", COLOR_WHITE);
    char eip_buf[32];
    htoa(regs->eip, eip_buf);
    printk(eip_buf, COLOR_WHITE);

    printk("\n  CS: ", COLOR_WHITE);
    htoa(regs->cs, buf); 
    printk(buf, COLOR_WHITE);

    printk("\n  ERR CODE: ", COLOR_WHITE);
    itoa(regs->err_code, buf); 
    printk(buf, COLOR_WHITE);
    
    while(1);
}
