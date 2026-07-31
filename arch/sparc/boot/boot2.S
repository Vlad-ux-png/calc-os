.section ".text.trap_table"
.align 4096              
.global trap_table_base

trap_table_base:
    sethi   %hi(_start), %g1
    or      %g1, %lo(_start), %g1
    jmp     %g1
    nop

    .skip 336

    .global trap_irq6
trap_irq6:
    sethi   %hi(real_irq6_handler), %l0
    or      %l0, %lo(real_irq6_handler), %l0
    jmp     %l0
    nop     

    .skip 3728

.section ".text.entry"
.global _start

real_irq6_handler:
    save    %sp, -96, %sp   

    call    timer_handler
    nop

    restore
    jmp     %l1            
    rett    %l2           

_start:
    sethi   %hi(trap_table_base), %g1
    or      %g1, %lo(trap_table_base), %g1
    wr      %g1, 0, %tbr
    nop; nop; nop

    mov     0x1080, %g1      
    wr      %g1, 0, %psr
    nop; nop; nop

    sethi   %hi(_stack_top), %sp
    or      %sp, %lo(_stack_top), %sp

    call    system_sparc
    nop
    