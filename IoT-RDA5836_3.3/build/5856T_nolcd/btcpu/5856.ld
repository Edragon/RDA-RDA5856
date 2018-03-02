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
/*Mandatory to have the BT side irq handler linked */
ENTRY (BT_CPU_Main)
EXTERN (_bt_Irq_handler)
SECTIONS
{
    /* ========================================== */
    /* set current location counter to flash base */
    /* ========================================== */
    . = 0xFFFFFFFFa8000000; /*reserved 0x30 Byte for flash header*/
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
    . = 0xFFFFFFFFa1C80000;
    _bt_irq_flash_start_location = _bt_rom_flash_start_location;
    .btirq_text . : AT (_bt_irq_flash_start_location)
    {
        _bt_irq_sram_txt_start = .;
        *btcpu_irq.o(.text)
    }
    _bt_irq_sram_txt_end = .;
    _bt_irq_sram_txt_size = _bt_irq_sram_txt_end - _bt_irq_sram_txt_start;
    _bt_irq_flash_end_location = _bt_irq_flash_start_location + _bt_irq_sram_txt_size;
    ASSERT(ABSOLUTE(_btrom_data_start) > ABSOLUTE(_bt_irq_sram_txt_end), "BT CPU SRAM IRQ code overwite !")
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
    . += 0x00000000;
    .bt_sramucdata . : AT ( _bt_sram_flash_start_location + _bt_sram_cached_size )
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
    . -= 0x00000000;
    _bt_sram_end = .;
    _bt_sram_size = _bt_sram_end - _bt_sram_start;
    _bt_sram_flash_end_location = _bt_sram_flash_start_location + _bt_sram_size;
    /*----------------------- */
    /* SYS SRAM BSS           */
    /*----------------------- */
    /*. = 0x81c10000;    5855_TODO: remove after metal change */
    _bt_sram_bss_start = .;
    /* Go Uncached */
    . += 0x00000000;
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
    . -= 0x00000000;
    .bt_srambss . (NOLOAD) : AT (ADDR(.bt_srambss))
    {
        *(.srambss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(EXCLUDE_FILE(ime_*.o) .bss.* .bss .gnu.linkonce.b.*)
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
    _bt_sram_top = 0xFFFFFFFFa1C80000 + 0x8000; /* End of internal sram */
  /*  _bt_sram_available_end = _tflash_bt_sram_start;*/
        _bt_sram_available_end = _bt_sram_top - (0xc00);
    /*----------------------- */
    /* SYS SRAM HEAP          */
    /*----------------------- */
    /* heap in sys sram */
    _bt_sram_heap_start = .;
    _bt_sram_heap_size = _bt_sram_available_end - _bt_sram_heap_start;
    /* Check that the INT SRAM is not over stuffed */
    ASSERT(ABSOLUTE(_bt_sram_available_end) > ABSOLUTE(_bt_sram_end), "BT CPU SRAM overflow !")
    /*---------------- */
    /* End of BT SRAM */
    /*---------------- */
    /* ================================================= */
    /* in DUALPORT SRAM                                  */
    /* ================================================= */
    . = 0xFFFFFFFFa1B00000;
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
        _bt_dualport_heap_size = 0x4000 - _bt_dualport_bss_size;
    ASSERT(ABSOLUTE(0xFFFFFFFFa1B00000 + 0x4000) >= ABSOLUTE(_bt_dualport_bss_end), "DUALPORT SRAM overflow.")
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
    .init . : AT(ADDR(.init))
    {
        *(.init)
        . = ALIGN(0x10);
    }
    .bt_rotext . : AT (ADDR(.bt_rotext))
    {
        *(.text)
        *(.text.*)
        *(.stub .gnu.linkonce.t.*)
        . = ALIGN(0x20);
    }
    .btrom . :AT (ADDR(.btrom))
    {
     *lslc*.o(.text .text.* .rodata .rodata.*)
        *tra_*.o(.text .text.* .rodata .rodata.*)
        *lmp_*.o(.text .text.* .rodata .rodata.*)
        *uslc_*.o(.text .text.* .rodata .rodata.*)
        *dl_dev*.o(.text .text.* .rodata .rodata.*)
        *hw_*.o(.text .text.* .rodata .rodata.*)
        *hc_*.o(.text .text.* .rodata .rodata.*)
        *tc_*.o(.text .text.* .rodata .rodata.*)
        *lc_*.o(.text .text.* .rodata .rodata.*)
        *hci_*.o(.text .text.* .rodata .rodata.*)
        *bt_addr*.o(.text .text.* .rodata .rodata.*)
        *bt_codec*.o(.text .text.* .rodata .rodata.*)
        *bt_fhs*.o(.text .text.* .rodata .rodata.*)
        *bt_init*.o(.text .text.* .rodata .rodata.*)
        *bt_mini*.o(.text .text.* .rodata .rodata.*)
        *bt_pump*.o(.text .text.* .rodata .rodata.*)
        *bt_test*.o(.text .text.* .rodata .rodata.*)
        *bt_timer*.o(.text .text.* .rodata .rodata.*)
        *hcit_*.o(.text .text.* .rodata .rodata.*)
        *sys_config.o(.text .text.* .rodata .rodata.*)
        *sys_hal_config.o(.text .text.* .rodata .rodata.*)
        *sys_rand_num_gen.o(.text .text.* .rodata .rodata.*)
        *sys_init.o(.text .text.* .rodata .rodata.*)
        *sys_power.o(.text .text.* .rodata .rodata.*)
        *sys_main.o(.text .text.* .rodata .rodata.*)
        *sys_irq.o(.text .text.* .rodata .rodata.*)
        *bt_irq.o(.text .text.* .rodata .rodata.*)
        *btcpu_irq.o(.text .text.* .rodata .rodata.*)
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
/* All sections we want to discard from the final linked elf.*/
    /*
     * DWARF debug sections.
     * Symbols in the DWARF debugging sections are relative to
     * the beginning of the section so we begin them at 0.
     */
    /* DWARF 1 */
    .debug (0) : { *(EXCLUDE_FILE(*.elf) .debug ) }
    .line (0) : { *(EXCLUDE_FILE(*.elf) .line ) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo (0) : { *(EXCLUDE_FILE(*.elf) .debug_srcinfo ) }
    .debug_sfnames (0) : { *(EXCLUDE_FILE(*.elf) .debug_sfnames ) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges (0) : { *(EXCLUDE_FILE(*.elf) .debug_aranges ) }
    .debug_pubnames (0) : { *(EXCLUDE_FILE(*.elf) .debug_pubnames ) }
    /* DWARF 2 */
    .debug_info (0) : { *(EXCLUDE_FILE(*.elf) .debug_info .gnu.linkonce.wi.* ) }
    .debug_abbrev (0) : { *(EXCLUDE_FILE(*.elf) .debug_abbrev ) }
    .debug_line (0) : { *(EXCLUDE_FILE(*.elf) .debug_line ) }
    .debug_frame (0) : { *(EXCLUDE_FILE(*.elf) .debug_frame ) }
    .debug_str (0) : { *(EXCLUDE_FILE(*.elf) .debug_str ) }
    .debug_loc (0) : { *(EXCLUDE_FILE(*.elf) .debug_loc ) }
    .debug_macinfo (0) : { *(EXCLUDE_FILE(*.elf) .debug_macinfo ) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames (0) : { *(EXCLUDE_FILE(*.elf) .debug_weaknames ) }
    .debug_funcnames (0) : { *(EXCLUDE_FILE(*.elf) .debug_funcnames ) }
    .debug_typenames (0) : { *(EXCLUDE_FILE(*.elf) .debug_typenames ) }
    .debug_varnames (0) : { *(EXCLUDE_FILE(*.elf) .debug_varnames ) }
    /* Stabs debugging sections.  */
    .stab (0) : { *(EXCLUDE_FILE(*.elf) .stab ) }
    .stabstr (0) : { *(EXCLUDE_FILE(*.elf) .stabstr ) }
    .stab.excl (0) : { *(EXCLUDE_FILE(*.elf) .stab.excl ) }
    .stab.exclstr (0) : { *(EXCLUDE_FILE(*.elf) .stab.exclstr ) }
    .stab.index (0) : { *(EXCLUDE_FILE(*.elf) .stab.index ) }
    .stab.indexstr (0) : { *(EXCLUDE_FILE(*.elf) .stab.indexstr ) }
    .comment (0) : { *(EXCLUDE_FILE(*.elf) .comment ) }
    /* These must appear regardless of  .  */
    .gptab.sdata :
    {
        *(.gptab.data)
        *(.gptab.sdata)
    }
    .gptab.sbss :
    {
        *(.gptab.bss)
        *(.gptab.sbss)
    }
    /DISCARD/ :
    {
        /* Error if a required input section is not listed in any output section */
        /* Useful to check the missing section descriptions */
        *(*)
    }
}
