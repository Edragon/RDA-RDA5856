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
/* The linker script depends on the following defines */
/*  - CONST_IN_RAM : all constants are placed in RAM, this is useful for ram_run */
/*  - CODE_IN_RAM : All code is executed from RAM, this is useful for ram_run or code running from RAM on romulator */
/*  - PRELOADED_INT_SRAM : The internal SRAM are already preloaded, useful for ram_run */
/*  - PRELOADED_RAM : The external RAM is already preloaded, useful for ram_run or simu */
/*  - __NATIVE_JAVA__ :  The Native Java need  reload it's data and bss section every time when app start*/
/* Those are constants of the system */


/* ROM content selection: things to include
 * WARNING: the DISCARD section uses the opposite -->
 * do all inclusion before discard !*/


/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif

#define MEM_CACHED_UNCACHED_OFFSET 0x20000000


/*Mandatory to have the BB side irq handler linked */
EXTERN (bcpu_main)

EXTERN (g_spcMapAccess)
EXTERN (g_sppMapAccess)

SECTIONS
{
    /* ========================================== */
    /* in External RAM                            */
    /* ========================================== */

    . = EXT_RAM_BASE;

#ifdef SPI_REG_DEBUG
    #define LINK_BCPU_IMAGE
    #include "spi_reg_debug.ld"
#endif

    . = ALIGN(0x100);

    _bcpu_ram_available_base = .;

    /* ============================== */
    /* Main BCPU entry point */
    /* ============================== */

#ifndef RAMRUN
    . = BCPU_CODE_BASE;
#else
    /* We do NOT consider CODE_IN_RAM and CONST_IN_RAM macros for BCPU RAMRUN.
     * All the codes and constants of BCPU RAMRUN will be put in ram space */
    . = BCPU_RAMRUN_CODE_BASE;
#endif

    _bcpu_flash_start = .;

    _bcpu_flash_rotext_start = .;

    .bcpu_rotext . : AT (ADDR(.bcpu_rotext))
    {
        *spc_main.o(.bcpu_main_entry)
        *(.text .text.*)
        . = ALIGN(0x10);
    }

    _bcpu_flash_rotext_end = .;

    _bcpu_flash_rodata_start = .;

    .bcpu_rodata . : AT (ADDR(.bcpu_rodata))
    {
        *(.rdata .rodata .rodata.str1.4 .rodata.* .rodata1)
        . = ALIGN(0x10);
    }

    _bcpu_flash_rodata_end = .;


    _bcpu_rom_flash_start_location = .;

#if defined(LOAD_ROM_ELF)
    /* ======================================== */
    /* BCPU rom in flash elf */
    /* ======================================== */
    #include "link_rom_elf.ld"
#endif

#if (defined(REWRITE_ROM_AT_BOOT) && defined(FPGA))
    _bcpu_rom_flash_end_location = .;
#else
    _bcpu_rom_flash_end_location = _bcpu_rom_flash_start_location;
#endif

    /* ================================================= */
    /* in BB SRAM                                        */
    /* ================================================= */
    _bb_sram_flash_start_location = _bcpu_rom_flash_end_location;

    . = BB_SRAM_BASE;
    _bb_sram_start = .;

#if defined(LOAD_ROM_ELF)

    _bb_sram_cached_size = 0;
    _bb_sram_uncached_size = 0;

#else /* !LOAD_ROM_ELF */

    /*------------------ */
    /* BB SRAM CACHED    */
    /*------------------ */
#ifndef PRELOADED_INT_SRAM
    .bbsram . : AT (_bb_sram_flash_start_location)
#else
    .bbsram . : AT (ADDR(.bbsram))
#endif
    {
        _bb_sram_cached_start = .;
        *(BB_ITLV_SECTION)
        . = ALIGN(0x4);
/*     _bb_sram_itlv_buf_end = .;*/
        *(BB_SPC_CTX_SECTION)
        . = ALIGN(0x10);
    }
    _bb_sram_cached_end = .;
    _bb_sram_cached_size = _bb_sram_cached_end - _bb_sram_cached_start;

    /*------------------ */
    /* BB SRAM UNCACHED  */
    /*------------------ */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

#ifndef PRELOADED_INT_SRAM
    .bbsramu . : AT (_bb_sram_flash_start_location + _bb_sram_cached_size)
#else
    .bbsramu . : AT (ADDR(.bbsramu))
#endif
    {
        _bb_sram_uncached_start = .;
        . = ALIGN(0x10);
        _mbbsramu = .;
        *(BB_STATIC_SECTION)
        . = ALIGN(0x10);
    }
    _bb_sram_uncached_end = .;
    _bb_sram_uncached_size = _bb_sram_uncached_end - _bb_sram_uncached_start;

#endif /* !LOAD_ROM_ELF */

    /* Read the symbol address from BCPU ROM */
    . = _bb_sram_romed_globals_end;

    .bbsram_globals_not_romed . (NOLOAD) : AT (ADDR(.bbsram_globals_not_romed))
    {
        _bb_sram_not_romed_globals_start = .;
        *(BB_SRAM_GLOBALS)
        *(.bbsram_globals)
        _bb_sram_not_romed_globals_end = .;
    }

    LD_ASSERT(ABSOLUTE(BB_SRAM_BASE + BB_SRAM_SIZE - RESERVED_PATCH_SIZE)
                        >= ABSOLUTE(_bb_sram_not_romed_globals_end),
                        "BBGS", "BBSRAM not romed globals overflow out of BBSRAM.")

    _bb_sram_var_size = _bb_sram_cached_size + _bb_sram_uncached_size;
    _bb_sram_var_end = _bb_sram_start + _bb_sram_var_size;

    /*----------------------- */
    /* BB SRAM Patch           */
    /*----------------------- */

#ifndef PRELOADED_INT_SRAM
    _bb_sram_patch_flash_start = _bb_sram_flash_start_location + _bb_sram_var_size;
#else
    _bb_sram_patch_flash_start = _bb_sram_flash_start_location;
#endif

    . = BB_SRAM_BASE + BB_SRAM_SIZE - RESERVED_PATCH_SIZE;
    _bb_sram_patch_start = .;

    .bbsram_patch_text . :
#ifndef PRELOADED_INT_SRAM
        AT (_bb_sram_patch_flash_start)
#else
        AT (ADDR(.bbsram_patch_text))
#endif
    {
        _bb_sram_patch_text_start = .;
        *(.bbsram_patch_text)
        *(.bbsram_patch_rodata)
        . = ALIGN(0x10);
        _bb_sram_patch_text_end = .;
    }
    _bb_sram_patch_text_size = _bb_sram_patch_text_end - _bb_sram_patch_text_start;

    .bbsram_patch_data . :
#ifndef PRELOADED_INT_SRAM
        AT (_bb_sram_patch_flash_start + _bb_sram_patch_text_size)
#else
        AT (ADDR(.bbsram_patch_data))
#endif
    {
        _bb_sram_patch_data_start = .;
        *(.bbsram_patch_data)
        . = ALIGN(0x10);
        _bb_sram_patch_data_end = .;
    }
    _bb_sram_patch_data_size = _bb_sram_patch_data_end - _bb_sram_patch_data_start;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bbsram_patch_ucdata . :
#ifndef PRELOADED_INT_SRAM
        AT (_bb_sram_patch_flash_start + _bb_sram_patch_text_size + _bb_sram_patch_data_size)
#else
        AT (ADDR(.bbsram_patch_ucdata))
#endif
    {
        _bb_sram_patch_ucdata_start = .;
        *(.bbsram_patch_ucdata)
        . = ALIGN(0x10);
        _bb_sram_patch_ucdata_end = .;
    }
    _bb_sram_patch_ucdata_size = _bb_sram_patch_ucdata_end - _bb_sram_patch_ucdata_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _bb_sram_patch_end = .;

    _bb_sram_patch_size = _bb_sram_patch_end - _bb_sram_patch_start;

#ifndef PRELOADED_INT_SRAM
    _bb_sram_patch_flash_end = _bb_sram_patch_flash_start + _bb_sram_patch_size;
#else
    _bb_sram_patch_flash_end = _bb_sram_patch_flash_start;
#endif

    _bb_sram_flash_end_location = _bb_sram_patch_flash_end;

    /*----------------------- */
    /* BB SRAM Patch BSS           */
    /*----------------------- */
    _bb_sram_patch_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bbsram_patch_ucbss . (NOLOAD) : AT (ADDR(.bbsram_patch_ucbss))
    {
        *(.bbsram_patch_ucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .bbsram_patch_bss . (NOLOAD) : AT (ADDR(.bbsram_patch_bss))
    {
        *(.bbsram_patch_bss)
        . = ALIGN(0x10);
    }

    _bb_sram_patch_bss_end = .;
    _bb_sram_patch_bss_size = _bb_sram_patch_bss_end - _bb_sram_patch_bss_start;

    LD_ASSERT(ABSOLUTE(BB_SRAM_BASE + BB_SRAM_SIZE) >= ABSOLUTE(_bb_sram_patch_bss_end),
                        "BBPC", "BBSRAM patch code overflow out of BBSRAM.")

    /* ===================================================== */
    /* in BB DUALPORT SRAM                                           */
    /* ===================================================== */
    _bcpu_dualport_sram_flash_start_location = _bb_sram_flash_end_location;

    . = BB_DUALPORT_SRAM_BASE;

    _bcpu_dualport_sram_start = .;

    .bcpu_dualport_sram_text . :
#ifndef PRELOADED_INT_SRAM
        AT (_bcpu_dualport_sram_flash_start_location)
#else
        AT (ADDR(.bcpu_dualport_sram_text))
#endif
    {
        _bcpu_dualport_sram_text_start = .;
        *(.dualport_sram_text)
        *(.dualport_sram_rodata)
        . = ALIGN(0x10);
        _bcpu_dualport_sram_text_end = .;
    }
    _bcpu_dualport_sram_text_size = _bcpu_dualport_sram_text_end - _bcpu_dualport_sram_text_start;

    .bcpu_dualport_sram_data . :
#ifndef PRELOADED_INT_SRAM
        AT (_bcpu_dualport_sram_flash_start_location + _bcpu_dualport_sram_text_size)
#else
        AT (ADDR(.bcpu_dualport_sram_data))
#endif
    {
        _bcpu_dualport_sram_cached_data_start = .;
        *(.dualport_sram_data)
        . = ALIGN(0x10);
        _bcpu_dualport_sram_cached_data_end = .;
    }
    _bcpu_dualport_sram_cached_data_size = _bcpu_dualport_sram_cached_data_end - _bcpu_dualport_sram_cached_data_start;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_dualport_sram_ucdata . :
#ifndef PRELOADED_INT_SRAM
        AT (_bcpu_dualport_sram_flash_start_location + _bcpu_dualport_sram_text_size + _bcpu_dualport_sram_cached_data_size)
#else
        AT (ADDR(.bcpu_dualport_sram_ucdata))
#endif
    {
        _bcpu_dualport_sram_uncached_data_start = .;
        *(.dualport_sram_ucdata)
        . = ALIGN(0x10);
        _bcpu_dualport_sram_uncached_data_end = .;
    }
    _bcpu_dualport_sram_uncached_data_size = _bcpu_dualport_sram_uncached_data_end - _bcpu_dualport_sram_uncached_data_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _bcpu_dualport_sram_end = .;
    _bcpu_dualport_sram_size = _bcpu_dualport_sram_end - _bcpu_dualport_sram_start;

#ifndef PRELOADED_INT_SRAM
    _bcpu_dualport_sram_flash_end_location = _bcpu_dualport_sram_flash_start_location + _bcpu_dualport_sram_size;
#else
    _bcpu_dualport_sram_flash_end_location = _bcpu_dualport_sram_flash_start_location;
#endif

    _bcpu_dualport_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_dualport_sram_ucbss . (NOLOAD) : AT (ADDR(.bcpu_dualport_sram_ucbss))
    {
        *(.dualport_sram_ucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_dualport_sram_bss . (NOLOAD) : AT (ADDR(.bcpu_dualport_sram_bss))
    {
        *(.dualport_sram_bss)
        . = ALIGN(0x10);
    }

    _bcpu_dualport_bss_end = .;
    _bcpu_dualport_bss_size = _bcpu_dualport_bss_end - _bcpu_dualport_bss_start;

    LD_ASSERT(ABSOLUTE(BB_DUALPORT_SRAM_BASE + BB_DUALPORT_SRAM_SIZE - RESERVED_MBX_SIZE)
                        >= ABSOLUTE(_bcpu_dualport_bss_end), "BDPO", "BCPU DUALPORT SRAM overflow.")

    /* ===================================================== */
    /* in SYS SRAM                                           */
    /* ===================================================== */
    _bcpu_sys_sram_flash_start_location = _bcpu_dualport_sram_flash_end_location;

    . = BCPU_SYS_SRAM_BASE;
    _bcpu_sys_sram_start = .;

#ifndef PRELOADED_INT_SRAM
    .bcpu_sramtext . : AT (_bcpu_sys_sram_flash_start_location)
#else
    .bcpu_sramtext . : AT (ADDR(.bcpu_sramtext))
#endif
    {
        _bcpu_sys_sram_txt_start = .;
        *(.sramtext .sramrodata)
        . = ALIGN(0x10);
    }
    _bcpu_sys_sram_txt_end = .;
    _bcpu_sys_sram_txt_size = _bcpu_sys_sram_txt_end - _bcpu_sys_sram_start;

#ifndef PRELOADED_INT_SRAM
    .bcpu_sramdata _bcpu_sys_sram_txt_end : AT (_bcpu_sys_sram_flash_start_location + _bcpu_sys_sram_txt_size )
#else
    .bcpu_sramdata _bcpu_sys_sram_txt_end : AT (ADDR(.bcpu_sramdata))
#endif
    {
        _bcpu_sys_sram_cached_start = .;
        *(.sramdata)
        . = ALIGN(0x10);
    }
    _bcpu_sys_sram_cached_end = .;
    _bcpu_sys_sram_cached_size = _bcpu_sys_sram_cached_end - _bcpu_sys_sram_cached_start;

    /*-------------------- */
    /* SYS SRAM UNCACHED   */
    /*-------------------- */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

#ifndef PRELOADED_INT_SRAM
    .bcpu_sramucdata . : AT (_bcpu_sys_sram_flash_start_location + _bcpu_sys_sram_txt_size + _bcpu_sys_sram_cached_size)
#else
    .bcpu_sramucdata . : AT (ADDR(.bcpu_sramucdata))
#endif
    {
        _bcpu_sys_sram_uncached_start = .;
        *(.sramucdata)
        . = ALIGN(0x10);
    }
    _bcpu_sys_sram_uncached_end = .;
    _bcpu_sys_sram_uncached_size = _bcpu_sys_sram_uncached_end - _bcpu_sys_sram_uncached_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _bcpu_sys_sram_end = .;
    _bcpu_sys_sram_size = _bcpu_sys_sram_end - _bcpu_sys_sram_start;

#ifndef PRELOADED_INT_SRAM
    _bcpu_sys_sram_flash_end_location = _bcpu_sys_sram_flash_start_location + _bcpu_sys_sram_size;
#else
    _bcpu_sys_sram_flash_end_location = _bcpu_sys_sram_flash_start_location;
#endif

    /*----------------------- */
    /* SYS SRAM BSS           */
    /*----------------------- */
    _bcpu_sys_sram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_sramucbss . (NOLOAD) : AT (ADDR(.bcpu_sramucbss))
    {
        *(.sramucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_srambss . (NOLOAD) : AT (ADDR(.bcpu_srambss))
    {
        *(.srambss)
        . = ALIGN(0x10);
    }

    _bcpu_sys_sram_bss_end = .;
    _bcpu_sys_sram_bss_size = _bcpu_sys_sram_bss_end - _bcpu_sys_sram_bss_start;

    _bcpu_sys_sram_end = _bcpu_sys_sram_bss_end;

    /* Check that the INT SRAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(SYS_SRAM_END - SYS_SRAM_IRQ_STACKWATCH_SIZE - XCPU_IRQ_STACK_SIZE)
                        > ABSOLUTE(_bcpu_sys_sram_end), "BINT", "BCPU Internal SRAM overflow !")

    /*---------------- */
    /* End of SYS SRAM */
    /*---------------- */

    /* ===================================================== */
    /* in External RAM                                           */
    /* ===================================================== */
    _bcpu_ram_flash_start_location = _bcpu_sys_sram_flash_end_location;

#if !defined(RAMRUN) && !defined(CHIP_HAS_AP)

    . = _bcpu_ram_available_base;

#else /* RAMRUN || CHIP_HAS_AP */

    /* In a RAMRUN or CHIP_HAS_AP, all the flash content will be put in the
     * ram, and _bcpu_ram_flash_start_location is in fact pointing to a ram
     * location. */
    . = _bcpu_ram_flash_start_location;

#endif /* RAMRUN || CHIP_HAS_AP */

    _bcpu_ram_start = .;

#ifndef PRELOADED_RAM
    .bcpu_ramtext . : AT (_bcpu_ram_flash_start_location)
#else
    .bcpu_ramtext . : AT (ADDR(.bcpu_ramtext))
#endif
    {
        _bcpu_ram_txt_start = .;
        *(.ram)
        . = ALIGN(0x10);
    }
    _bcpu_ram_txt_end = .;
    _bcpu_ram_txt_size = _bcpu_ram_txt_end - _bcpu_ram_start;

#ifndef PRELOADED_RAM
    _bcpu_ram_data_load_start = _bcpu_ram_flash_start_location + _bcpu_ram_txt_size;
#else
    _bcpu_ram_data_load_start = .;
#endif

#ifdef CHIP_HAS_AP
#ifdef BCPU_EXT_RAM_DATA_SIZE
    . += BCPU_EXT_RAM_DATA_SIZE;
#endif
#endif

    _bcpu_ram_data_start = .;

    .bcpu_data . : AT (_bcpu_ram_data_load_start)
    {
        _bcpu_ram_cached_start = .;
        *(.data .data.* .sdata .sdata.* .data1)
        . = ALIGN(0x10);
    }
    _bcpu_ram_cached_end = .;
    _bcpu_ram_cached_size = _bcpu_ram_cached_end - _bcpu_ram_cached_start;

    /*-------------------- */
    /* External RAM UNCACHED   */
    /*-------------------- */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_ucdata . : AT (_bcpu_ram_data_load_start + _bcpu_ram_cached_size)
    {
        _bcpu_ram_uncached_start = .;
        *(.ucdata)
        . = ALIGN(0x10);
    }
    _bcpu_ram_uncached_end = .;
    _bcpu_ram_uncached_size = _bcpu_ram_uncached_end - _bcpu_ram_uncached_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _bcpu_ram_data_end = .;
    _bcpu_ram_data_size = _bcpu_ram_data_end - _bcpu_ram_data_start;

    _bcpu_ram_data_load_end = _bcpu_ram_data_load_start + _bcpu_ram_data_size;

#ifdef CHIP_HAS_AP
#if defined(BCPU_EXT_RAM_DATA_SIZE) && (BCPU_EXT_RAM_DATA_SIZE > 0)
    /* Check that the bcpu ram data is not over overflowed */
    LD_ASSERT(ABSOLUTE(_bcpu_ram_data_start) >= ABSOLUTE(_bcpu_ram_data_load_end), "BDOV", "BCPU ram data overflow (BCPU_EXT_RAM_DATA_SIZE too small) !")
#endif
#endif

    _bcpu_ram_end = .;
    _bcpu_ram_size = _bcpu_ram_end - _bcpu_ram_start;

#if !defined(PRELOADED_RAM) && !defined(CHIP_HAS_AP)
    _bcpu_ram_flash_end_location = _bcpu_ram_flash_start_location + _bcpu_ram_size;
#else
    _bcpu_ram_flash_end_location = _bcpu_ram_flash_start_location;
#endif

    /*----------------------- */
    /* External RAM BSS           */
    /*----------------------- */
    _bcpu_ram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_ucbss . (NOLOAD) : AT (ADDR(.bcpu_ucbss))
    {
        *(.ucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .bcpu_bss . (NOLOAD) : AT (ADDR(.bcpu_bss))
    {
        *(.bss .bss.* .sbss .sbss.*)
        . = ALIGN(0x10);
    }

    _bcpu_ram_bss_end = .;
    _bcpu_ram_bss_size = _bcpu_ram_bss_end - _bcpu_ram_bss_start;

    /* Check that the External RAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(EXT_RAM_BASE + RAM_SIZE)
                        > ABSOLUTE(_bcpu_ram_bss_end), "BEXT", "BCPU External RAM overflow !")

    /*---------------- */
    /* End of External RAM */
    /*---------------- */

    /*========================================================= */
    /* Back in FLASH                                                 */
    /*========================================================= */

#if !defined(RAMRUN) && !defined(CHIP_HAS_AP)
    . = _bcpu_ram_flash_end_location;
#else
    . = _bcpu_ram_bss_end;
#endif

    /* WORKAROUND:
     * Keep the following section as the last section to remove the LD warning,
     * "access beyond end of merged section", when linking XCPU image. This warning might
     * be ejected due to introducing some BCPU LD symbols defined above to the XCPU image.
     */
    .bcpu_flash_end . : AT(ADDR(.bcpu_flash_end))
    {
        LONG(0)
        . = ALIGN(0x10);
    }

    _bcpu_flash_end = .;

    /* This is a rough check. XCPU image linking process will check flash/ram usage in detail. */
#if !defined(RAMRUN) && !defined(CHIP_HAS_AP)
    LD_ASSERT(ABSOLUTE(FLASH_BASE + FLASH_SIZE) > ABSOLUTE(_bcpu_flash_end),
              "BFLA",
              "BCPU Flash overflow: there is no more space available in flash memory.")
#else
    LD_ASSERT(ABSOLUTE(EXT_RAM_BASE + RAM_SIZE) > ABSOLUTE(_bcpu_flash_end),
              "BRAM",
              "BCPU RAM overflow: there is no more space available in RAM.")
#endif


    #include "discard.ld"
}
