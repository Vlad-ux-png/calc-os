[bits 16]
[org 0x7c00]

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    mov [boot_drive], dl

    mov ax, 0x2401
    int 0x15

    mov ax, 0x0013
    int 0x10

    mov ax, 0x0000
    mov es, ax
    mov bx, 0x1000
    mov ah, 0x02
    mov al, 50
    mov ch, 0
    mov dh, 0
    mov cl, 2
	mov dl, [boot_drive]
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
    mov esp, 0x7C00

    jmp 0x1000

align 4
gdt_start:
    dq 0
gdt_code:
    dw 0xffff, 0x0000, 0x9a00, 0x00cf
gdt_data:
    dw 0xffff, 0x0000, 0x9200, 0x00cf
gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1
    dd gdt_start

boot_drive: db 0

times 510-($-$$) db 0
dw 0xAA55