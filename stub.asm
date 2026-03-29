[bits 16]
[org 0x7c00]

start:
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7c00

    mov ah, 0x02
    mov al, 15 
    mov ch, 0
    mov dh, 0
    mov cl, 2 
    mov bx, 0x0000
    mov es, bx
    mov bx, 0x1000
    int 0x13
    jc disk_error

    cli
    lgdt [gdt_ptr]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp 0x08:init_32bit

disk_error:
    jmp $

[bits 32]
init_32bit:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov esp, 0x90000 

    jmp 0x1000          
align 4
gdt_start: 
    dq 0
gdt_code: 
    dw 0xffff  
    dw 0x0      
    db 0x0  
    db 10011010b
    db 11001111b
    db 0x0       

gdt_data:
    dw 0xffff
    dw 0x0     
    db 0x0      
    db 10010010b 
    db 11001111b 
    db 0x0    
gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55

        
