[bits 32]

global outb
global inb
global inw
global outw

outb:
    mov edx, [esp + 4]   
    mov al, [esp + 8]    
    out dx, al
    ret

inb:
    mov edx, [esp + 4]    
    in al, dx             
    ret

inw:
    mov edx, [esp + 4]    
    in ax, dx            
    ret

outw:
    mov edx, [esp + 4]   
    mov ax, [esp + 8]     
    out dx, ax
    ret