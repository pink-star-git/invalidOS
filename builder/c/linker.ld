OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 0xA000;
    .text :
    {
        *(.text);
    }
    .data :
    {
        *(.data);
        *(.bss);
        *(.rodata);
    }
}