#include <stdint.h>
#include <sparc.h>

unsigned int timer_ticks = 0;

void init_timer() {
    *LEON3_TIMER_CTRL = 0;

    *LEON3_TIMER_RELOAD = TIMER_FREQUENCY_HZ;
    *LEON3_TIMER_VAL = TIMER_FREQUENCY_HZ;

    *LEON3_IRQMASK = *LEON3_IRQMASK | (1 << 6);

    uint32_t psr;
    __asm__ __volatile__(
        "rd %%psr, %0\n\t"
        "or %0, 0x20, %0\n\t" 
        "wr %0, 0x0, %%psr\n\t"
        "nop; nop; nop;\n\t"   
        : "=r"(psr)
    );

    *LEON3_TIMER_CTRL = TIMER_CTRL_ENABLE | TIMER_CTRL_RELOAD | TIMER_CTRL_IRQEN | TIMER_CTRL_LOAD;
}

void timer_handler() {
    timer_ticks++;
}
