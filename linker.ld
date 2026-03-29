ENTRY(kernel_main)

SECTIONS
{
    /* Адреса завантаження */
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
        /* Видалили COMMON, щоб не було помилки */
    }

    /* Видаляємо непотрібні секції Windows */
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.reloc)
    }
}