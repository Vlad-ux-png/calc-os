[bits 32]
section .text
global get_scancode

get_scancode:
    push ebp
    mov ebp, esp

.wait:
    in al, 0x64
    test al, 1
    jz .wait
    
    in al, 0x60
    
    test al, 0x80
    jnz .wait

    movzx eax, al
    
    pop ebp
    ret