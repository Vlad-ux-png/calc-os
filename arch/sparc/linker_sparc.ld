OUTPUT_ARCH("sparc")
ENTRY(_start)

SECTIONS
{
    . = 0x40000000;

    .text : {
        . = ALIGN(4096);
        *(.text.trap_table)

        . = ALIGN(4096);
        *(.text.entry)

        *(.text)
        *(.text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        . = ALIGN(8);
        _stack_bottom = .;    
        . += 4096;            
        _stack_top = .;        
        
        *(.bss .bss.*)
        *(COMMON)
    }

    _end = .;
}
