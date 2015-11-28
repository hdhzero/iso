ENTRY(mystart)
OUTPUT_FORMAT(binary)
OUTPUT(kernel.img)

SECTIONS {
    .text 0x7C00 : {
        . = 0;
        boot.o(.text);
        FILL(0x00);
        . = 510;
        FILL(0x55);
        . = 511;
        FILL(0xAA);

        . = 512;
        boot2.o(.text);
        FILL(0x00);

        /* gdt */
        . = 1024;
        boot2.o(.data);

        . = 1536;
        *(.text);
        *(.rodata);
        *(.data);

        FILL(0x00);
        . = 1474560;
    }

    /DISCARD/ : {
        *(.eh_frame);
        *(.comment);
    }
}
