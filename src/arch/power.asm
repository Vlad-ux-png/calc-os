[bits 32]
org 0x100000

start:
    cli
    
    mov ecx, 1000000

.wait_input:
    in al, 0x64    
    test al, 0x02     
    jz .do_reboot   
    
    dec ecx        
    jnz .wait_input   
    
    ret               

.do_reboot:
    mov al, 0xFE
    out 0x64, al       

.loop:
    hlt             
    jmp .loop