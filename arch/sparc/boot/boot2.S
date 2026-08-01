.section ".text.entry"
.align 4096              
.global _start

.extern system_sparc

_start:
    mov     0x0f20, %g1      
    wr      %g1, 0, %psr
    nop; nop; nop

    sethi   %hi(_stack_top), %sp
    or      %sp, %lo(_stack_top), %sp
    sub     %sp, 96, %sp    

    /* 3. Виклик C-ядра */
    call    system_sparc
    clr     %o0           

