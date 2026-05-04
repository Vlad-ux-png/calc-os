ENTRY(kernel_main)

SECTIONS
{
    . = 0x1000;

    .text : {
        *(.text.entry)
        *(.text)
    }

    .data : {
        *(.data)
        *(.rdata)
    }

    .bss : {
        *(.bss)
    }

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.reloc)
    }
}