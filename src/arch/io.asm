[bits 32]
section .text
global get_scancode

get_scancode:
    push ebp
    mov ebp, esp

    in al, 0x64     
    test al, 0x01    
    jz .no_data     

    test al, 0x20    
    jnz .no_data    

    in al, 0x60      
    test al, 0x80    
    jnz .no_data

    movzx eax, al    
    pop ebp
    ret

.no_data:
    xor eax, eax
    pop ebp
    ret