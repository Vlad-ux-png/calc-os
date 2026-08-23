#include <cmos.h>
#include <video.h>
#include <mouse.h>
#include <utils.h>
#include <keyboard.h>
#include <idt.h>
#include <task.h>
#include <stdint.h>
#include <sound.h>
#include <forth.h>

void init_timer() {
    outb(0x43, 0x36); 

    outb(0x40, 0x52); 
    outb(0x40, 0x09);
}

uint32_t timer_handler(struct registers *regs) {
    check_signals(current_task, (uint32_t*)regs);

    if (task_list[current_task].is_active == 0) {
        int next_task = current_task;
        while (1) {
            next_task = (next_task + 1) % 4;
            if (next_task == 0 || task_list[next_task].is_active == 1) {
                break;
            }
        }
        current_task = next_task;
        timer_ticks++;
        outb(0x20, 0x20);
        return (uint32_t)task_list[current_task].esp; 
    }

    task_list[current_task].esp = (void*)regs; 

    int next_task = current_task;
    while (1) {
        next_task = (next_task + 1) % 4;
        if (next_task == 0 || task_list[next_task].is_active == 1) {
            break;
        }
    }

    current_task = next_task;
    timer_ticks++;
    outb(0x20, 0x20); 

    return (uint32_t)task_list[current_task].esp;
}
