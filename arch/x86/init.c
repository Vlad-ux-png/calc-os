#include <stdint.h>
#include <idt.h>
#include <task.h>
#include <video.h>

void init_paging();
void enable_paging();
void boot();
extern int current_mode;
extern void screen_clear();

void __attribute__((section(".text.entry"))) kernel_main() {
    put_char('C', COLOR_BLACK);
	init_paging();
    enable_paging();
    
    __asm__ __volatile__("cli");
	screen_clear();
    init_idt();
    __asm__ __volatile__("sti");

	put_char('a', COLOR_BLACK);
	
    boot();
    current_mode = 0;

	put_char('l', COLOR_BLACK);
	
	__asm__ __volatile__("cli");
	task_list[3].is_active = 1;
	__asm__ __volatile__("sti");

	put_char('c', COLOR_BLACK);
	
	while (1) {
		__asm__ __volatile__("hlt");
	}
}
