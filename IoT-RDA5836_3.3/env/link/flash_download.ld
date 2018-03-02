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

ENTRY (boot_Sector)
EXTERN (boot_LoaderEnter)
EXTERN (boot_Sector)
EXTERN (boot_Sector_Nand)


SECTIONS
{
    _bb_sram_flash_start_location = . ;

    _bb_sram_flash_end_location  = _bb_sram_flash_start_location;

    /* ===================================================== */
    /* in DUALPORT SRAM                                           */
    /* ===================================================== */
    _BB_DUALPORT_SRAM_flash_start_location = _bb_sram_flash_end_location;

#if (CHIP_ASIC_ID != 15) /* CHIP_ASIC_ID_5856 */

    _BB_DUALPORT_SRAM_size = 0;

#else /* 8808 or later */

    /* NAND_FLASH: No need to reserve a nand boot code space for flash programmer */
    . = BB_DUALPORT_SRAM_BASE;

    _BB_DUALPORT_SRAM_start = .;

    .BB_DUALPORT_SRAM_text . :
        AT (ADDR(.BB_DUALPORT_SRAM_text))
    {
        _BB_DUALPORT_SRAM_text_start = .;
        *(.BB_DUALPORT_SRAM_text)
        . = ALIGN(0x10);
        _BB_DUALPORT_SRAM_text_end = .;
    }
    _BB_DUALPORT_SRAM_text_size = _BB_DUALPORT_SRAM_text_end - _BB_DUALPORT_SRAM_text_start;

    .BB_DUALPORT_SRAM_data . :
        AT (ADDR(.BB_DUALPORT_SRAM_data))
    {
        _BB_DUALPORT_SRAM_cached_data_start = .;
        *(.BB_DUALPORT_SRAM_data)
        . = ALIGN(0x10);
        _BB_DUALPORT_SRAM_cached_data_end = .;
    }
    _BB_DUALPORT_SRAM_cached_data_size = _BB_DUALPORT_SRAM_cached_data_end - _BB_DUALPORT_SRAM_cached_data_start;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .BB_DUALPORT_SRAM_ucdata . :
        AT (ADDR(.BB_DUALPORT_SRAM_ucdata))
    {
        _BB_DUALPORT_SRAM_uncached_data_start = .;
        *(.BB_DUALPORT_SRAM_ucdata)
        . = ALIGN(0x10);
        _BB_DUALPORT_SRAM_uncached_data_end = .;
    }
    _BB_DUALPORT_SRAM_uncached_data_size = _BB_DUALPORT_SRAM_uncached_data_end - _BB_DUALPORT_SRAM_uncached_data_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _BB_DUALPORT_SRAM_end =  .;
    _BB_DUALPORT_SRAM_size = _BB_DUALPORT_SRAM_end - _BB_DUALPORT_SRAM_start;

    /*----------------------- */
    /* DUALPORT SRAM BSS           */
    /*----------------------- */
    _dualport_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .BB_DUALPORT_SRAM_ucbss . (NOLOAD) : AT (ADDR(.BB_DUALPORT_SRAM_ucbss))
    {
        *(.sramucbss)
        *(.BB_DUALPORT_SRAM_ucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .BB_DUALPORT_SRAM_bss . (NOLOAD) : AT (ADDR(.BB_DUALPORT_SRAM_bss))
    {
        *(.BB_DUALPORT_SRAM_bss)
        . = ALIGN(0x10);
    }

    _dualport_bss_end = .;
    _dualport_bss_size = _dualport_bss_end - _dualport_bss_start;

    LD_ASSERT(ABSOLUTE(BB_DUALPORT_SRAM_BASE + BB_DUALPORT_SRAM_SIZE - RESERVED_MBX_SIZE) >= ABSOLUTE(_dualport_bss_end), "DPOV", "DUALPORT SRAM overflow.")

#endif /* (CHIP_ASIC_ID != 6) */

    _BB_DUALPORT_SRAM_flash_end_location = _BB_DUALPORT_SRAM_flash_start_location;

    /* ===================================================== */
    /* in SYS SRAM                                           */
    /* ===================================================== */

    /*----------------- */
    /* SYS SRAM UNCACHED */
    /*----------------- */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    /* For ramruns, this section loads an execution context
     * content allowing to directly executed the 
     * boot sector (preloaded with PC=boot_Sector and SP=0,
     * cf boot_sector.c) */
    .executor_ctx ROM_MISC_UBASE : AT (ADDR(.executor_ctx) )
    { 
        /* For ramruns, avoid removing .executor_ctx even when option
         * --gc-sections is enabled, as its content must be loaded from
         * the ramrun lod */
        KEEP(*(.executor_ctx))
    }
   
    /*----------------- */
    /* SYS SRAM CACHED  */
    /*----------------- */

    LD_ASSERT(ABSOLUTE(__int_rom_reserved_end) <= ROM_MISC_BASE + ROM_MISC_SIZE, "RRRO", "Reserved Ram Reserved for Rom Overflowed")

    . = SYS_SRAM_BASE;

    /* HAL Boot Sector reload structure, contains information written by
     * HAL during the normal execution of the code and used by the
     * Boot Sector to reload the ram image when the phone boots.
     * It is mapped in the Internal SRAM because we don't want the Boot
     * Sector code to access the External RAM. And it is mapped at a fixed
     * location (after the last patch for the chip) because we
     * don't want the RAMRUN to overwrite it.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process. */
    .hal_boot_sector_reload_struct . (NOLOAD) :
        AT (ADDR(.hal_boot_sector_reload_struct))
    {
        KEEP(*(.hal_boot_sector_reload_struct))
    }

    /* HAL Boot Sector structure, contains information written by
     * HAL during the normal execution of the code and used by the
     * Boot Sector when the phone boots.
     * It is mapped in the Internal SRAM because we don't want the Boot
     * Sector code to access the External RAM. And it is mapped at a fixed
     * location (after the last patch for the chip) because we
     * don't want the RAMRUN to overwrite it.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process. */
    .hal_boot_sector_struct . (NOLOAD) :
        AT (ADDR(.hal_boot_sector_struct))
    {
        KEEP(*(.hal_boot_sector_struct))
    }

    /* Boot Sector reload structure pointer, used to retrieve the
     * structure filled by HAL and used by the Boot Sector code.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process. */
    .boot_sector_reload_struct_ptr (SYS_SRAM_AVAILABLE_BASE-4-4-4) (NOLOAD) :
        AT (ADDR(.boot_sector_reload_struct_ptr))
    {
        KEEP(*(.boot_sector_reload_struct_ptr))
    }

    /* Boot Sector structure pointer, used to retrieve the command
     * structure filled by HAL and used by the Boot Sector code.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process. */
    .boot_sector_struct_ptr (SYS_SRAM_AVAILABLE_BASE-4-4) (NOLOAD) :
        AT (ADDR(.boot_sector_struct_ptr))
    {
        KEEP(*(.boot_sector_struct_ptr))
    }

    /* Fixed pointer, used to retrieve the map structure. */
    .fixptr (SYS_SRAM_AVAILABLE_BASE-4+MEM_CACHED_UNCACHED_OFFSET) : AT (ADDR(.fixptr))
    {
        KEEP(*(.fixptr))
    }

    . = SYS_SRAM_AVAILABLE_BASE;

    /* ------------------------------------------------ */
    /* SYS SRAM IRQ - IRQ HANDLER IN SYS SRAM */
    /* ------------------------------------------------ */
    _irq_sys_sram_flash_start_location = _BB_DUALPORT_SRAM_flash_end_location;

    _irq_sys_sram_start = .;
    .irqsram . : AT (ADDR(.irqsram))
    {
        . = ALIGN(0x10);
    }
    _irq_sys_sram_end = .;
    _irq_sys_sram_size = _irq_sys_sram_end - _irq_sys_sram_start;

    _irq_sys_sram_flash_end_location = _irq_sys_sram_flash_start_location;

    /* ------------------------------------------------ */
    /* BOOTING TEXT AND DATA SECTIONS IN SYS SRAM */
    /* ------------------------------------------------ */
    _boot_sys_sram_flash_start_location = _irq_sys_sram_flash_end_location;

    _boot_sys_sram_start = .;

    _boot_sys_sram_txt_start = .;
    .bootsramtext . : AT (ADDR(.bootsramtext))
    {
        . = ALIGN(0x10);
    }
    _boot_sys_sram_txt_end = .;
    _boot_sys_sram_txt_size = _boot_sys_sram_txt_end - _boot_sys_sram_txt_start;

    _boot_sys_sram_data_start = .;
    .bootsramdata . : AT (ADDR(.bootsramdata))
    {
        . = ALIGN(0x10);
    }
    _boot_sys_sram_txt_end = .;
    _boot_sys_sram_txt_size = _boot_sys_sram_txt_end - _boot_sys_sram_txt_start;

    _boot_sys_sram_end = .;
    _boot_sys_sram_size = _boot_sys_sram_end - _boot_sys_sram_start;

    _boot_sys_sram_flash_end_location = _boot_sys_sram_flash_start_location;

    /* ------------------------------------------------ */
    /* NON-BOOTING TEXT AND DATA SECTIONS IN SYS SRAM */
    /* ------------------------------------------------ */
    _sys_sram_flash_start_location = _boot_sys_sram_flash_end_location;

    _sys_sram_start = .;
    _sys_sram_cached_start = .;
	_sys_sram_txt_start = .;
	
	.sramtext . : AT (ADDR(.sramtext))
    {
        

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
        *(.sramtext)
        *(.ram)
        *(.text .text.*)

        . = ALIGN(0x10);
    }
	
	_sys_sram_txt_end = .;
    _sys_sram_txt_size = _sys_sram_txt_end - _sys_sram_start;

    .sramdata _sys_sram_txt_end : AT (ADDR(.sramdata))
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
        *(.data)
        . = ALIGN(0x10);
    }
    _sys_sram_data_end = .;
    _sys_sram_cached_end = .;
    _sys_sram_data_size = _sys_sram_data_end - _sys_sram_data_start;
    _sys_sram_cached_size = _sys_sram_cached_end - _sys_sram_cached_start;

    /*-------------------- */
    /* SYS SRAM UNCACHED   */
    /*-------------------- */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .sramucdata . : AT (ADDR(.sramucdata))
    {
        _sys_sram_uncached_start = .;
        *(.sramucdata)
        *(.srroucdata)
        *(.ucdata)
        *(.backup)
        /* FIXME We don't know what this is for but
         * we put it there for now. */
        *(.rel.dyn)
        . = ALIGN(0x10);
    }
    _sys_sram_uncached_end = .;
    _sys_sram_uncached_size = _sys_sram_uncached_end - _sys_sram_uncached_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _sys_sram_end =  .;
    _sys_sram_size = _sys_sram_end - _sys_sram_start;

    _sys_sram_flash_end_location = _sys_sram_flash_start_location;

    /*----------------------- */
    /* SYS SRAM BSS */
    /*----------------------- */
    _sys_sram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .sramucbss . (NOLOAD) : AT (ADDR(.sramucbss))
    {
        *(.ucbss)
        *(.sramucbss)
#if (RAM_SIZE==0)
	*(.ramucbss)
#endif
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

    _sys_sram_bss_end = .;
    _sys_sram_bss_size = _sys_sram_bss_end - _sys_sram_bss_start;

    /*----------------------- */
    /* SYS SRAM HEAP          */
    /*----------------------- */    
    /* heap in sys sram */
    .heap . (NOLOAD) : AT (ADDR(.heap))
    {
        *cos_mem.o(.mempool)
		    _sys_sram_heap_start = .;
    }
    _sys_sram_top = SYS_SRAM_END; /* End of internal sram */
    /*_sys_sram_heap_start = _board_config_end;*/
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


    /*---------------- */
    /* End of SYS SRAM */
    /*---------------- */

#if (RAM_SIZE>0)

    . = EXT_RAM_BASE;
    
    /*-------------------------------------------------------  */
    /* RAM BSS (content reset to 0 )                           */
    /*-------------------------------------------------------  */
    _ram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .ucbss    . (NOLOAD) : AT (ADDR(.ucbss))
    {
        *(.ramucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .bss . (NOLOAD) : AT (ADDR(.bss))
    {
        *(.rambss)
        . = ALIGN(0x10);
    }

    _ram_bss_end = .;
    _ram_bss_size = _ram_bss_end - _ram_bss_start;

    /*----------- */
    /* End of RAM */
    /*----------- */
    _ram_top = EXT_RAM_BASE + RAM_SIZE;
    _heap_size = _ram_top -_ram_bss_end;
    _heap_start =  _ram_bss_end;
    _heap_end =  _ram_bss_end + _heap_size;

    /* Check that the external RAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_ram_top) > ABSOLUTE(_ram_bss_end), "EXTS", "External RAM overflow !")
#endif /* (RAM_SIZE>0) */
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

