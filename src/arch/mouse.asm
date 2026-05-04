[bits 32]
global init_mouse
global mouse_wait
global mouse_wait_input

mouse_wait:
    in al, 0x64
    and al, 0x02
    jnz mouse_wait
    ret

mouse_wait_input:
    in al, 0x64
    and al, 0x01
    jz mouse_wait_input
    ret

init_mouse:
    call mouse_wait
    mov al, 0xA8
    out 0x64, al

    call mouse_wait
    mov al, 0x20
    out 0x64, al
    call mouse_wait_input
    in al, 0x60
    
    or al, 0x02
    push eax
    call mouse_wait
    mov al, 0x60
    out 0x64, al
    call mouse_wait
    pop eax
    out 0x60, al

    call mouse_wait
    mov al, 0xD4
    out 0x64, al
    call mouse_wait
    mov al, 0xF4
    out 0x60, al
    ret