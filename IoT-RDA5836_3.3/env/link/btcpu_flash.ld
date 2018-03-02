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

#ifdef __NATIVE_JAVA__
#define EXCLUDE_JAVA_OBJ EXCLUDE_FILE(gcj_*.o *.oo)
#else
#define EXCLUDE_JAVA_OBJ
#endif


/*Mandatory to have the BT side irq handler linked */
ENTRY (BT_CPU_Main)
EXTERN (_bt_Irq_handler)



SECTIONS
{
    /* ========================================== */
    /* set current location counter to flash base */
    /* ========================================== */

    . = FLASH_BASE;  /*reserved 0x30 Byte for flash header*/

    _bt_flash_start = . + 0x30;
    /* ============================= */
    /* boot sector in flash          */
    /* ============================= */
    .bt_boot_code _bt_flash_start : AT (ADDR(.bt_boot_code))
    {
        KEEP(*btcpu_main.o(.rodata.*))
        . = ALIGN(0x10);
        KEEP(*btcpu_main.o(.text.BT_CPU_Main))
    }


    _bt_rom_flash_start_location = .;

    /* ================================================= */
    /* in BB SRAM                                        */
    /* ================================================= */

    . = BT_SRAM_BASE;

    _bt_irq_flash_start_location = _bt_rom_flash_start_location;

    .btirq_text . : AT (_bt_irq_flash_start_location)
    {
        _bt_irq_sram_txt_start = .;
        *btcpu_irq.o(.text)
    }
    _bt_irq_sram_txt_end = .;
    _bt_irq_sram_txt_size = _bt_irq_sram_txt_end - _bt_irq_sram_txt_start;
    _bt_irq_flash_end_location = _bt_irq_flash_start_location +  _bt_irq_sram_txt_size;
    LD_ASSERT(ABSOLUTE(_btrom_data_start) > ABSOLUTE(_bt_irq_sram_txt_end),
                "BTIS", "BT CPU SRAM IRQ code overwite !")

#if defined(LOAD_ROM_ELF)
    /* ======================================== */
    /* BTCPU rom in flash elf */
    /* ======================================== */
    #include "link_rom_elf.ld"
#endif

    /* ------------------------------------------------ */
    /* NON-BOOTING TEXT AND DATA SECTIONS IN SYS SRAM */
    /* ------------------------------------------------ */
    . = _btrom_sram_end;
    _bt_sram_flash_start_location = _bt_irq_flash_end_location;

    _bt_sram_start = .;
    _bt_sram_cached_start = .;

    .bt_sramtext . : AT (_bt_sram_flash_start_location )
    {
        _bt_sram_text_start = .;
        *(.btsramtext)
        . = ALIGN(0x10);
    }
    _bt_sram_text_end = .;
    _bt_sram_text_size = _bt_sram_text_end - _bt_sram_text_start;

    .bt_sramdata . : AT (_bt_sram_flash_start_location + _bt_sram_text_size)
    {
         _bt_sram_data_start = .;

         *(.sramdata)

    /* NOTE: globalvars.c sections are to be removed.
    * They are kept here for backward compatibility of SVC/VPP codes.
    * These codes are linked from Total_BJ branch and the special global
    * variable sections will be removed in the future. */
        *(*.globalvars.c *.globalvars.c.*)

        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        *(.data1)
        _gp = ALIGN(16) + 0x7ff0;
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
        *(.lit8)
        *(.lit4)
        . = ALIGN(0x10);
    }
    _bt_sram_data_end = .;
    _bt_sram_cached_end = .;
    _bt_sram_data_size = _bt_sram_data_end - _bt_sram_data_start;
    _bt_sram_cached_size = _bt_sram_cached_end - _bt_sram_cached_start;

    /*-------------------- */
    /* SYS SRAM UNCACHED   */
    /*-------------------- */
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

#ifndef PRELOADED_INT_SRAM
    .bt_sramucdata . : AT ( _bt_sram_flash_start_location + _bt_sram_cached_size )
#else
    .bt_sramucdata . : AT (ADDR(.bt_sramucdata))
#endif
    {
        _bt_sram_uncached_start = .;
        *(.sramucdata)
        *(.srroucdata)
        *(.ucdata)
        *(*.globalvars.uc)
        . = ALIGN(0x10);
    }
    _bt_sram_uncached_end = .;
    _bt_sram_uncached_size = _bt_sram_uncached_end - _bt_sram_uncached_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _bt_sram_end =  .;
    _bt_sram_size = _bt_sram_end - _bt_sram_start;

    _bt_sram_flash_end_location = _bt_sram_flash_start_location + _bt_sram_size;


    /*----------------------- */
    /* SYS SRAM BSS           */
    /*----------------------- */
    /*. = 0x81c10000;    5855_TODO: remove after metal change */

    _bt_sram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .bt_sramucbss . (NOLOAD) : AT (ADDR(.bt_sramucbss))
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

    .bt_srambss . (NOLOAD) : AT (ADDR(.bt_srambss))
    {
        *(.srambss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(EXCLUDE_FILE(ime_*.o) .bss.* .bss  .gnu.linkonce.b.*)
        *(EXCLUDE_FILE(ime_*.o) COMMON)
        *(.rambss)
        . = ALIGN(0x10);
    }

    _bt_sram_bss_end = .;
    _bt_sram_bss_size = _bt_sram_bss_end - _bt_sram_bss_start;


    /*-------------------------------------------------------  */
    /* Uninitialized section in RAM (content kept after reboot)*/
    /*-------------------------------------------------------  */
    .bt_backup . (NOLOAD) : AT (ADDR(.bt_backup))
    {
        *(.backup)
        . = ALIGN(0x10);
    }

    _bt_sram_end = .;
    _bt_sram_top = BT_SRAM_BASE + BT_SRAM_SIZE; /* End of internal sram */
  /*  _bt_sram_available_end = _tflash_bt_sram_start;*/
        _bt_sram_available_end = _bt_sram_top - (BTCPU_IRQ_STACK_SIZE);

    /*----------------------- */
    /* SYS SRAM HEAP          */
    /*----------------------- */
    /* heap in sys sram */
    _bt_sram_heap_start = .;
    _bt_sram_heap_size = _bt_sram_available_end - _bt_sram_heap_start;

    /* Check that the INT SRAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_bt_sram_available_end) > ABSOLUTE(_bt_sram_end),
		"BTS", "BT CPU SRAM overflow !")

    /*---------------- */
    /* End of BT SRAM */
    /*---------------- */

    /* ================================================= */
    /* in DUALPORT SRAM                                  */
    /* ================================================= */
#if (NAND_FLASH == 1)
    . = BB_DUALPORT_SRAM_BASE + NAND_FLASH_BOOT_CODE_SIZE;
#else
    . = BB_DUALPORT_SRAM_BASE;
#endif

    _bt_dualport_bss_start = .;

    . = _rom_dualport_bss_end;

    .bt_dualport_sram_bss . (NOLOAD) : AT (ADDR(.bt_dualport_sram_bss))
    {
        *hcit_sharemem.o(.dualucbss)
        *rdabt_sharemem.o(.dualucbss)
        *hw_spi_flash.o(.dualucbss)
        *(.dualucbss)
        *tra_queue.o(.bss .bss.*)

        . = ALIGN(0x10);
    }

    _bt_dualport_bss_end = .;
    _bt_dualport_bss_size = _bt_dualport_bss_end - _bt_dualport_bss_start;

    _bt_dualport_heap_start = _bt_dualport_bss_end;
        _bt_dualport_heap_size = BB_DUALPORT_SRAM_SIZE - _bt_dualport_bss_size;

    LD_ASSERT(ABSOLUTE(BB_DUALPORT_SRAM_BASE + BB_DUALPORT_SRAM_SIZE) >=
                          ABSOLUTE(_bt_dualport_bss_end), "DPOV", "DUALPORT SRAM overflow.")



    /*========================================================= */
    /* In FLASH                                                 */
    /*========================================================= */

    _btrom_flash_start_location = _bt_sram_flash_end_location;

    /* .crossrefs section can be used to list functions which is share by other application */
    .crossrefs _btrom_flash_start_location : AT (ADDR(.crossrefs))
    {
        *(.crossref)
        . = ALIGN(0x10);
    }

#ifndef COMPRESS_PROCESS_ENABLE
    .init . : AT(ADDR(.init))
    {
        *(.init)
        . = ALIGN(0x10);
    }
#endif

    .bt_rotext . : AT (ADDR(.bt_rotext))
    {
        *(.text)
        *(.text.*)
        *(.stub .gnu.linkonce.t.*)
        . = ALIGN(0x20);
    }

    .btrom . :AT (ADDR(.btrom))
    {
        #include "btcore_code_content"
        . = ALIGN(0x10);
    }

    .bt_rodata . : AT (ADDR(.bt_rodata))
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
        . = ALIGN(0x10);
    }


    /*=================================================== */
    /* Placeholder for misc hacks, not used for real apps */
    /*=================================================== */

    . = ALIGN(0x40);
    _bt_flash_end = .;

    #include "discard.ld"
}

#ifdef COMPRESS_DATA_SECTION_NOCROSSREFS
COMPRESS_DATA_SECTION_NOCROSSREFS
#endif

