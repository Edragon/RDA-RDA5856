/* ======================================================================= */
/*                  TARGET CONFIGURATION SECTION                           */
/* ======================================================================= */
/* The following values are defined in ff_target.def and exported by the Makefile */
/*  FLASH_SIZE              */
/*  RAM_SIZE                */
/*  CALIB_BASE              */
/*  FACT_SETTINGS_BASE      */
/*  CODE_BASE               */
/*  USER_DATA_BASE          */
/*  USER_DATA_SIZE          */
/* ======================================================================= */
/*                  END CONFIGURATION SECTION                              */
/* ======================================================================= */
/* This linker script is dedicated to the flash_programmer.
 * The linker script depends on the following defines */
/*  - CONST_IN_RAM : all constants are placed in SRAMrun */
/*  - CODE_IN_RAM : All code is executed from SRAM, this is useful for ram_run or code running from RAM on romulator */
/*  - PRELOADED_INT_SRAM : We fill it with a ramrun, so this is always true. */
/*  - PRELOADED_RAM : We don't use any external ram */
/*  - RAMRUN :  The flashprogrammer is always ramruned */

/* Those are constants of the system */


/* ROM content selection: things to include
    WARNING: the DISCARD section uses the opposite -->
    do all inclusion before discard !*/


/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif

#ifdef CHIP_HAS_NOCACHE
#define MEM_CACHED_UNCACHED_OFFSET 0x00000000
#else
#define MEM_CACHED_UNCACHED_OFFSET 0x20000000
#endif

ENTRY (DSM_DO_Upgrade)


SECTIONS
{
    
    . = SYS_SRAM_END  - (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (XCPU_IRQ_STACK_SIZE) - TFLASH_RAM_SIZE;

    /* ------------------------------------------------ */
    /* SYS SRAM Data IN SYS SRAM */
    /* ------------------------------------------------ */

    _tflash_sys_sram_start = .;
		_sys_sram_tflash_start_location = CODE_BASE + FLASH_SIZE - TFLASH_FLASH_SIZE;
 
    .sramdata . : AT (_sys_sram_tflash_start_location)
    {
        _sys_sram_data_start = .;
        *(.bootsramdata)
        *(.bootdata)
        *(.sramdata)
        *(.sramtext)
        *(.sdata2.*)
        *(.sdata2)
        *(.sdata.*)
        *(.sdata)
        *(.data.*)
        *(.data)
        . = ALIGN(0x10);
    }

    _tflash_sys_sram_end =  .;
    _tflash_sys_sram_size = _tflash_sys_sram_end - _tflash_sys_sram_start;
		_sys_sram_tflash_end_location = _sys_sram_tflash_start_location + _tflash_sys_sram_size;
		
    /*----------------------- */
    /* SYS SRAM BSS       */
    /*----------------------- */
    _tflash_sram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .sramucbss . (NOLOAD) : AT (ADDR(.sramucbss))
    {
        *(.sramucbss)
        *(.ucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .srambss . (NOLOAD) : AT (ADDR(.srambss))
    {
        *(.bootbss)
        *(.srambss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(0x10);
    } 

    _tflash_sram_bss_end = .;
    _tflash_sram_bss_size = _tflash_sram_bss_end - _tflash_sram_bss_start;


    /* Check that the INT SRAM is not over stuffed */
    _sys_sram_irq_stackwatch_size = (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (PS_STACK_SIZE) ;
    _sys_sram_irq_stackwatch_start = SYS_SRAM_END - (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (XCPU_IRQ_STACK_SIZE);
    _sys_sram_irq_stackwatch_end = _sys_sram_irq_stackwatch_start + _sys_sram_irq_stackwatch_size;
    LD_ASSERT(ABSOLUTE(_sys_sram_irq_stackwatch_start) > ABSOLUTE(_tflash_sram_bss_end), "TFUS", "TFlash Upgrade SRAM overflow !")

    /*---------------- */
    /* End of SYS SRAM */
    /*---------------- */
    
    . = _sys_sram_tflash_end_location;
    /*========================================================= */
    /* In FLASH                                                 */
    /*========================================================= */

    .rotext . : AT (ADDR(.rotext))
    {
        *(.text)
        *(.text.*)
        *(.stub .gnu.linkonce.t.*)
        *(.boottext)
        . = ALIGN(0x20);
    }

    .rodata . : AT (ADDR(.rodata))
    {
        *(.rdata)
        *(.rodata)
        *(.rodata.*)
        *(.rodata.*_png)
        *(.gnu.linkonce.r.*) 
        *(.rodata1)
        *(.roresdata)
        *(.robsdata)
        *(.bootrodata)
        *(.extra)
        . = ALIGN(0x20);
    }

    _tflash_end = .;
    
    LD_ASSERT(ABSOLUTE(CODE_BASE + FLASH_SIZE) > ABSOLUTE(_tflash_end), "TFFS", "TFlash Upgrade Flash overflow !")
    
    #include "discard.ld"
}

