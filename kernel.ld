SECTIONS 
{
    /* Define the kernel's higher-half base address */
    KERNEL_BASE = 0xffff000000000000;

    . = KERNEL_BASE + 0x80000;
    .text : {
        KEEP(*(.text.boot))
        *(.text .text.* .gnu.linkonce.t*)
    }
    PROVIDE(_data = .);
    .data : {
        *(.data .data.* .gnu.linkonce.d*)
    }
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    . = ALIGN(4096);
    _end = .;

    /* 5 page tables for bootstraping (physical addresses)*/
    . += 0x5000;
    _pt_end = .;

    /DISCARD/ : { 
        *(.comment) 
        *(.gnu*) 
        *(.note*) 
        *(.eh_frame*) 
    }
}

__bss_size = (__bss_end - __bss_start) >> 3;
