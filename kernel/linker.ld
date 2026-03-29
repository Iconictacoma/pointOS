/* Linker script for pointOS kernel */

ENTRY(_start)

SECTIONS
{
    . = 0x10000;
    
    .text ALIGN(0x1000) : AT(0x1000)
    {
        *(.text)
        *(.text*)
        *(.gnu.linkonce.t*)
    }
    
    .data ALIGN(0x1000) : 
    {
        *(.data)
        *(.data*)
        *(.gnu.linkonce.d*)
    }
    
    .bss ALIGN(0x1000) :
    {
        *(.bss)
        *(.bss*)
        *(.gnu.linkonce.b*)
    } 
    
    .rodata ALIGN(0x1000) :
    {
        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }
}
