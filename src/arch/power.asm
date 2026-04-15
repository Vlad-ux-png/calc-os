[bits 32]
org 0x100000

start:
    cli
.loop:
    hlt
    jmp .loop