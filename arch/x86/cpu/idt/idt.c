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

struct idt_entry   idt_entry[256];
struct idt_pointer idtp;

unsigned int timer_ticks = 0;
unsigned char *timer_str[16];

void set_idt_gate(uint8_t number, uint32_t base, uint16_t selector, uint8_t flags) {
    idt_entry[number].base_low = (base & 0xFFFF);
    idt_entry[number].base_high = (base >> 16) & 0xFFFF;
    idt_entry[number].selector = selector;
    idt_entry[number].always0 = 0;
    idt_entry[number].flags = flags;
}

void pic_remap() {
    outb(0x20, 0x11); 
    outb(0xA0, 0x11);
    
    outb(0x21, 0x20);
    outb(0xA1, 0x28); 
    
    outb(0x21, 0x04); 
    outb(0xA1, 0x02); 
    
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    outb(0x21, 0xF8); 
    outb(0xA1, 0x2F); 
}

void delay_ticks(uint32_t ticks) {
    uint32_t target_ticks = timer_ticks + ticks;
    while (timer_ticks < target_ticks) {
        __asm__ __volatile__("hlt");
    }
}

void init_idt() {
    idtp.idt_ptr = (uint32_t)&idt_entry;
    idtp.idt_size = (sizeof(struct idt_entry) * 256) - 1;

    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, 0, 0, 0);
    }

    pic_remap();
    init_timer();

    set_idt_gate(0,  (uint32_t)isr0,  0x08, 0x8E);
    set_idt_gate(1,  (uint32_t)isr1,  0x08, 0x8E);
    set_idt_gate(2,  (uint32_t)isr2,  0x08, 0x8E);
    set_idt_gate(3,  (uint32_t)isr3,  0x08, 0x8E);
    set_idt_gate(4,  (uint32_t)isr4,  0x08, 0x8E);
    set_idt_gate(5,  (uint32_t)isr5,  0x08, 0x8E);
    set_idt_gate(6,  (uint32_t)isr6,  0x08, 0x8E);
    set_idt_gate(7,  (uint32_t)isr7,  0x08, 0x8E);
    set_idt_gate(8,  (uint32_t)isr8,  0x08, 0x8E);
    set_idt_gate(9,  (uint32_t)isr9,  0x08, 0x8E);
    set_idt_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    set_idt_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    set_idt_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    set_idt_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    set_idt_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    set_idt_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    set_idt_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    set_idt_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    set_idt_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    set_idt_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    set_idt_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    set_idt_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    set_idt_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    set_idt_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    set_idt_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    set_idt_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    set_idt_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    set_idt_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    set_idt_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    set_idt_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    set_idt_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    set_idt_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    set_idt_gate(32, (uint32_t)timer_wrapper, 0x08, 0x8E);
    set_idt_gate(33, (uint32_t)keyboard_wrapper, 0x08, 0x8E);
    set_idt_gate(44, (uint32_t)mouse_wrapper, 0x08, 0x8E);
    set_idt_gate(46, (uint32_t)ata_wrapper, 0x08, 0x8E);

    set_idt_gate(128, (uint32_t)syscall_wrapper, 0x08, 0x8E); 

    prepare_task2();
    prepare_task3();
    prepare_task4();
    
    current_task = 0; 
    task_list[0].id = 0;

    task_list[0].is_active = 1; 

    task_list[1].is_active = 1;
    task_list[2].is_active = 0;
    task_list[3].is_active = 0;

    __asm__ __volatile__("lidt (%0)" : : "r" (&idtp));

    for (int i = 0; i < 50; i++) {
        if (inb(0x64) & 0x01) {
            inb(0x60);
        }
    }

    init_mouse();

    mouse_cycle = 0; 
    mouse_ready = 0;
    
    __asm__ __volatile__("sti");
}
