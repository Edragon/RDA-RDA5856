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

ENTRY (test_main)
EXTERN(_sxr_Irq_handler)

SECTIONS
{
		. = CODE_BASE;
		
    .main_entry_section . : AT (ADDR(.main_entry_section))
    {
    		*test_main.o(.text.test_main)
    }
    
    _sys_sram_flash_start_location = . ;

    /* ===================================================== */
    /* in SYS SRAM                                           */
    /* ===================================================== */
    /* ------------------------------------------------ */
    /* SYS SRAM IRQ - IRQ HANDLER IN SYS SRAM */
    /* ------------------------------------------------ */

#if (UNIT_TEST_BT==1)
    . = BT_SRAM_BASE;
    _sys_sram_start = .;
    _irq_sys_sram_start = .;

    .btirq_text . : AT (_sys_sram_flash_start_location)
    {
    	KEEP(*(.bt_irq_text))
    }
#else

    . = SYS_SRAM_AVAILABLE_BASE;

    /* ------------------------------------------------ */
    /* SYS SRAM IRQ - IRQ HANDLER IN SYS SRAM */
    /* ------------------------------------------------ */

		_sys_sram_start = .;
    _irq_sys_sram_start = .;
    .irqsram . : AT (_sys_sram_flash_start_location)
    {
        KEEP(*hal_irq_handler.o(.irqtext))
        *hal_irq_handler.o(.text .text.* .sramtext .rdata .rodata .rodata.*)
        . = ALIGN(0x10);
    }
#endif

    _irq_sys_sram_end = .;
    _irq_sys_sram_size = _irq_sys_sram_end - _irq_sys_sram_start;
    
    /* ------------------------------------------------ */
    /* NON-BOOTING TEXT AND DATA SECTIONS IN SYS SRAM */
    /* ------------------------------------------------ */
		_sramtext_start_location = _sys_sram_flash_start_location + _irq_sys_sram_size;
    .sramtext . : AT (_sramtext_start_location)
    {
        _sys_sram_txt_start = .;

        _boot_sector = .;
        *(.boot_sector_start)
        *boot_sector.o(.text .text.* .rdata .rodata .rodata.*)
        *boot_sector_chip.o(.text .text.* .rdata .rodata .rodata.*)
        *(.boottext .bootrodata)

        _code_base = .;
        *boot_loader.o(.text.boot_loader_start)
        *boot_loader.o(.text .text.* .rdata .rodata .rodata.*)

        *(.bootsramtext)
        *(.bootramtext)
        *(.sram_hst_monitor_ctx)
        *(.sramtext)
        *(.btsramtext)
        *(.ram)

        . = ALIGN(0x10);
    }

    _sys_sram_txt_end = .;
    _sys_sram_txt_size = _sys_sram_txt_end - _sys_sram_start;

    .sramdata . : AT (_sys_sram_flash_start_location + _sys_sram_txt_size)
    {
        _sys_sram_data_start = .;
        *(.bootsramdata)
        *(.bootdata)
        *(.sramdata)
        *(.srrodata)
        *(.rodata.*)
        *(.rodata)
        *(.sdata2.*)
        *(.sdata2)
        *(.sdata.*)
        *(.sdata)
        *(.data.*)
        *test_main.o(.data.* .data) /* replace  *(.bss .bss.* .gnu.linkonce.b.*) for  SIM_TC_STOP at 01c00000(test main there), rather than 01b00000*/
        *(.data)
        . = ALIGN(0x10);
    }
    _sys_sram_data_end = .;
    _sys_sram_data_size = _sys_sram_data_end - _sys_sram_data_start;

    /*-------------------- */
    /* SYS SRAM UNCACHED   */
    /*-------------------- */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .sramucdata . : AT (_sys_sram_flash_start_location + _sys_sram_txt_size + _sys_sram_data_size)
    {
        _sys_sram_uncached_start = .;
        *(.sramucdata)
        *(.srroucdata)
        *(.ucdata)
        *(.backup)
        /* FIXME We don't know what this is for but
         * we put it there for now. */
        /*(.rel.dyn) */
        . = ALIGN(0x10);
    }
    _sys_sram_uncached_end = .;
    _sys_sram_uncached_size = _sys_sram_uncached_end - _sys_sram_uncached_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _sys_sram_end =  .;
    _sys_sram_size = _sys_sram_end - _sys_sram_start;

    _sys_sram_flash_end_location = _sys_sram_flash_start_location + _sys_sram_size;

    /*----------------------- */
    /* SYS SRAM BSS */
    /*----------------------- */
    _sys_sram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .sramucbss . (NOLOAD) : AT (ADDR(.sramucbss))
    {
        *(.sramucbss)
        *(.ucbss)
        *(.ramucbss)
        *(.bt_ucbss)
        *overlay.o(.pcm_big_datamem)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .srambss . (NOLOAD) : AT (ADDR(.srambss))
    {
        *(.srambss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(.bss.* .bss  .gnu.linkonce.b.*)
        *(COMMON)
        *(.rambss)

        . = ALIGN(0x10);
    } 

    _sys_sram_bss_end = .;
    _sys_sram_bss_size = _sys_sram_bss_end - _sys_sram_bss_start;

    /*----------------------- */
    /* SYS SRAM HEAP          */
    /*----------------------- */    
#if (UNIT_TEST_BT==1)
		_sys_sram_top = BT_SRAM_BASE + BT_SRAM_SIZE;
#else
    _sys_sram_top = SYS_SRAM_END; /* End of internal sram */
#endif
    _sys_sram_heap_start = .;
    _sys_sram_heap_end = _sys_sram_top - (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (XCPU_IRQ_STACK_SIZE);
    _sys_sram_heap_size = _sys_sram_heap_end - _sys_sram_heap_start;

    /* Check that the INT SRAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_sys_sram_heap_end) > ABSOLUTE(_sys_sram_heap_start), "INTS", "Internal SRAM overflow !")


    /*----------------------- */
    /* SYS SRAM IRQ STACK WATCH */
    /*----------------------- */
    _sys_sram_irq_stackwatch_size = (SYS_SRAM_IRQ_STACKWATCH_SIZE) ;
    _sys_sram_irq_stackwatch_start = _sys_sram_heap_end;
    _sys_sram_irq_stackwatch_end = _sys_sram_irq_stackwatch_start + _sys_sram_irq_stackwatch_size;

    /* Caution, _sys_sram_non_romed_top is used by the boot ROM code. */
    .sram_romed_top (_sys_sram_top) (NOLOAD) : AT (ADDR(.sram_romed_top))
    {
        _sys_sram_non_romed_top = .;
    }


    /*---------------- */
    /* End of SYS SRAM */
    /*---------------- */


    /*========================================================= */
    /* In FLASH                                                 */
    /*========================================================= */
		_rom_flash_start_location = _sys_sram_flash_end_location;
    .rotext _rom_flash_start_location : AT (ADDR(.rotext))
    {
        *(.text)
        *(.text.*)
        *(.stub .gnu.linkonce.t.*)
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
        *(.extra)
        . = ALIGN(0x20);
    }


    /*=================================================== */
    /* Placeholder for misc hacks, not used for real apps */
    /*=================================================== */
    .misc . : AT (ADDR(.misc))
    {
        *(.misc)
        . = ALIGN(0x10);
        *(.build_date)
        . = ALIGN(0x10);
    }

    _flash_end = .;
    
    /*=================================================== */
    /* Placeholder for misc hacks, not used for real apps */
    /*=================================================== */
    .misc . : AT (ADDR(.misc))
    {
        *(.misc)
        . = ALIGN(0x10);
    }

    #include "discard.ld"
}

