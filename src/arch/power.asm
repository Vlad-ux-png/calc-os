[bits 32]
global sys_halt

sys_halt:
    cli
    jmp .loop
.loop:
    hlt