ENTRY(mystart)
OUTPUT_FORMAT(binary)
/*INPUT(t.o boot_func.o)*/
INPUT(boot.o)
OUTPUT(kernel.img)

SECTIONS
{
    . = 0;
    .text : { 
        boot.o(.text);
        FILL(0x00);
        . = 510;
        FILL(0x55);
        . = 511;
        FILL(0xAA);
        . = 512;
    }

/*
    . = 0x4000;
    .myfunc : {
        boot_func.o(.data);
        boot_func.o(.text);
    }
*/
}
