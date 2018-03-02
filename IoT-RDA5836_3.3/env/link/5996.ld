/****************************************************************************************
* @file map.txt
*
* @brief This file is the map file (gnuarm or armgcc).
*
* Copyright (C) RivieraWaves 2009-2015
*
****************************************************************************************
*/

/* configure the CPU type */
OUTPUT_ARCH(arm)
/* link with the standard c library */
INPUT(-lc)
/* link with the standard GCC library */
INPUT(-lgcc)
/* configure the entry point */
ENTRY(vectors)

MEMORY
{
    program_memory (rx) : ORIGIN = 0x00108000, LENGTH = 280k
    data_memory (!rx) :   ORIGIN = 0x0014e000, LENGTH = 180k
}

SECTIONS
{
    EXEC_RAM_TEXT 0x00108000:
    {
    	 . = ALIGN(8);
        code_base = .;
        /* the address 0 must contain the boot vectors */
        *sys_vctr.o(.text)
        /* immediately followed by the boot handlers */
        *sys_boot.o(.text)
        *sys_fiq_irq.o(.text)
        *sys_main.o(.text)
        *(.text)
        *(.rodata)
        code_end = .;
    } > program_memory
    
    /* Data section */
    RAM_DATA :
    {
        data_base = .;
        *(.data)
        data_end = .;
    } > data_memory
    data_length = data_end - data_base;
    
    /* BSS section */
    RAM_BSS :
    {
        bss_base = .;
        *(.bss)
        *(COMMON)
        bss_end = .;
    } > data_memory
    bss_length = bss_end - bss_base;
}
