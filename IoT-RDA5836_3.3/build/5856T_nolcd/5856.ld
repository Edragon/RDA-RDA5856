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
/*Mandatory to have the BB side irq handler linked */
ENTRY (_flash_btcpu_start)
EXTERN (__bb_int_handler)
EXTERN (_sxr_Irq_handler)
EXTERN (boot_LoaderEnter)
EXTERN (boot_Sector)
EXTERN (boot_Sector_Nand)
SECTIONS
{
    . = 0xFFFFFFFFa8000000;
    _iota_flash_start = .;
    /* ============================= */
    /* boot sector in flash          */
    /* ============================= */
    .flash_start_code . : AT (ADDR(.flash_start_code))
    {
      *btcpu_flash.o(.text)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        . = ALIGN(0x10);
    }
    /* ======================================== */
    /* BCPU flash elf  */
    /* ======================================== */
.bt_boot_code (0xffffffffa8000030) : AT (0xffffffffa8000030) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_boot_code)) }
.btirq_text (0xffffffffa1c80000) : AT (0xffffffffa8000204) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.btirq_text)) }
.bt_sramtext (0xffffffffa1c83d50) : AT (0xffffffffa8000374) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_sramtext)) }
.bt_sramdata (0xffffffffa1c842a0) : AT (0xffffffffa80008c4) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_sramdata)) }
.bt_sramucdata (0xffffffffa1c843f0) : AT (0xffffffffa8000a14) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_sramucdata)) }
.bt_sramucbss (0xffffffffa1c84400) (NOLOAD) : AT (0xffffffffa1c84400) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_sramucbss)) }
.bt_srambss (0xffffffffa1c84430) (NOLOAD) : AT (0xffffffffa1c84430) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_srambss)) }
.crossrefs (0xffffffffa8000a24) (NOLOAD) : AT (0xffffffffa8000a24) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.crossrefs)) }
.bt_rotext (0xffffffffa8000a30) : AT (0xffffffffa8000a30) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_rotext)) }
.btrom (0xffffffffa803a820) : AT (0xffffffffa803a820) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.btrom)) }
.bt_rodata (0xffffffffa8043190) : AT (0xffffffffa8043190) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_elf_file.elf(.bt_rodata)) }
    /* ======================================== */
    /* XCPU flash elf */
    /* ======================================== */
.boot_code (0xffffffffa804ee80) : AT (0xffffffffa804ee80) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.boot_code)) }
.main_entry_section (0xffffffffa804f2f0) : AT (0xffffffffa804f2f0) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.main_entry_section)) }
.dualport_sram_ubss (0xffffffffa1b00000) (NOLOAD) : AT (0xffffffffa1b00000) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.dualport_sram_ubss)) }
.hal_boot_sector_struct (0xffffffffa1c00000) (NOLOAD) : AT (0xffffffffa1c00000) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.hal_boot_sector_struct)) }
.hal_tflash_upgrade_struct (0xffffffffa1c0001c) (NOLOAD) : AT (0xffffffffa1c0001c) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.hal_tflash_upgrade_struct)) }
.boot_tflash_upgrade_struct_ptr (0xffffffffa1c00270) (NOLOAD) : AT (0xffffffffa1c00270) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.boot_tflash_upgrade_struct_ptr)) }
.boot_sector_reload_struct_ptr (0xffffffffa1c00274) (NOLOAD) : AT (0xffffffffa1c00274) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.boot_sector_reload_struct_ptr)) }
.boot_sector_struct_ptr (0xffffffffa1c00278) (NOLOAD) : AT (0xffffffffa1c00278) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.boot_sector_struct_ptr)) }
.fixptr (0xffffffffa1c0027c) (NOLOAD) : AT (0xffffffffa1c0027c) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.fixptr)) }
.irqsram (0xffffffffa1c00280) : AT (0xffffffffa804f6e0) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.irqsram)) }
.bootsramtext (0xffffffffa1c00660) : AT (0xffffffffa804fac0) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.bootsramtext)) }
.bootsramdata (0xffffffffa1c00b60) : AT (0xffffffffa804ffc0) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.bootsramdata)) }
.bootbss (0xffffffffa1c00b90) (NOLOAD) : AT (0xffffffffa1c00b90) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.bootbss)) }
.sramtext (0xffffffffa1c00bc0) : AT (0xffffffffa804fff0) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.sramtext)) }
.sramdata (0xffffffffa1c00c80) : AT (0xffffffffa80500b0) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.sramdata)) }
.sramucdata (0xffffffffa1c01320) : AT (0xffffffffa8050750) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.sramucdata)) }
.sramucbss (0xffffffffa1c01b50) (NOLOAD) : AT (0xffffffffa1c01b50) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.sramucbss)) }
.srambss (0xffffffffa1c06540) (NOLOAD) : AT (0xffffffffa1c06540) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.srambss)) }
.backup (0xffffffffa1c08e20) (NOLOAD) : AT (0xffffffffa1c08e20) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.backup)) }
.spi_reg_debug (0xffffffffa2000000) (NOLOAD) : AT (0xffffffffa2000000) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.spi_reg_debug)) }
.rotext (0xffffffffa8050f80) : AT (0xffffffffa8050f80) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.rotext)) }
.rodata (0xffffffffa8098300) : AT (0xffffffffa8098300) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.rodata)) }
.misc (0xffffffffa80ca9c0) : AT (0xffffffffa80ca9c0) { KEEP(build/5856T_nolcd/btcpu/RDA5856/XCPU_elf_file.elf-stripped(.misc)) }
    /* ======================================== */
    /* XCPU ROM elf */
    /* ======================================== */
.internal_rom (0xffffffffa1e00000) : AT (0xffffffffa1e00000) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.internal_rom)) }
.sys_rom (0xffffffffa1e03780) : AT (0xffffffffa1e03780) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.sys_rom)) }
.rom_entries_uncached (0xffffffffa1c000a0) (NOLOAD) : AT (0xffffffffa1c000a0) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.rom_entries_uncached)) }
.rom_entries_cached (0xffffffffa1c000b8) (NOLOAD) : AT (0xffffffffa1c000b8) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.rom_entries_cached)) }
.rom_inter_ram (0xffffffffa1c00100) (NOLOAD) : AT (0xffffffffa1c00100) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.rom_inter_ram)) }
.rom_inter_data (0xffffffffa1c00240) : AT (0xffffffffa1e03fd0) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.rom_inter_data)) }
.boot_rom_version_number (0xffffffffa1e03ff8) : AT (0xffffffffa1e03ff8) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.boot_rom_version_number)) }
.internal_rom.crc (0xffffffffa1e03ffc) : AT (0xffffffffa1e03ffc) { KEEP(build/5856T_nolcd/stripped_rom_elf_file.elf(.internal_rom.crc)) }
    /* ======================================== */
    /* BTCPU ROM elf  */
    /* ======================================== */
.btcpu_rom (0xffffffffa1e80000) : AT (0xffffffffa1e80000) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_rom_elf_file.elf(.btcpu_rom)) }
.bt_introm_data (0xffffffffa1c80180) : AT (0xffffffffa1e8fe40) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_rom_elf_file.elf(.bt_introm_data)) }
.bt_rom_uncached (0xffffffffa1c80220) (NOLOAD) : AT (0xffffffffa1c80220) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_rom_elf_file.elf(.bt_rom_uncached)) }
.bt_rom_inter_ram (0xffffffffa1c80324) (NOLOAD) : AT (0xffffffffa1c80324) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_rom_elf_file.elf(.bt_rom_inter_ram)) }
.bt_inter_rom.crc (0xffffffffa1e8fffc) : AT (0xffffffffa1e8fffc) { KEEP(build/5856T_nolcd/btcpu/RDA5856/stripped_BTCPU_rom_elf_file.elf(.bt_inter_rom.crc)) }
  .= _flash_end;
    _res_data_flash_start_location = .;
    .res . : AT (ADDR(.res))
    {
     KEEP(*res.bin.o(.data .data.*))
    }
    _res_data_end = .;
    /*================================================= */
    /* Define user data boundaries based on the defines */
    /*================================================= */
    .user_data 0xFFFFFFFFa8000000+0x000FC000 : AT (ADDR(.user_data))
    {
        _user_data_start = .;
        *user.bin.o(.user_data);
        KEEP(*(.user_data))
    }
    _user_data_end = .;
    _user_data_size = _user_data_end - _user_data_start;
    /*Check that the data and code in flash do not overwrite
     * the user data space */
    /* ASSERT seems to be boguous as of binutils-2.15 : we are
     * not allowed to used error messages longer than FLAS in that
     * particular case. Don't ask and port a newer version,
     * maybe. FIXME */
    ASSERT(ABSOLUTE(_user_data_start) > ABSOLUTE(_res_data_end), "Flash overflow: there is no more space available in flash memory.")






    /* ============================== */
    /* calib sector in flash          */
    /* ============================== */

    .calib 0xFFFFFFFFa8000000+0x000FE000 : AT (ADDR(.calib))
    {
        _calib_start = .;
        *(.calib);
        . = ALIGN(0x10);
    }
    _calib_end = .;
    _calib_size = _calib_end - _calib_start;
    /* ============================== */
    /* factory settings in flash      */
    /* ============================== */
    .factory_settings 0xFFFFFFFFa8000000+0x000FF000 : AT (ADDR(.factory_settings))
    {
        _factory_start = .;
        *(.factory_settings)
        . = ALIGN(0x10);
    }
    _factory_end = .;
    _factory_size = _factory_end - _factory_start;
    /* Legal layout patterns:
     * 1) Code (including boot code) -> User Data -> Calib Data -> Factory Data
     * 2) Boot Sector -> Calib Data -> Factory Data -> Code (excluding boot code) -> User Data
     * /

    /*Check that the user data in flash do not overwrite
     * the calib data or factory setting space */
    ASSERT(ABSOLUTE(_calib_start) >= ABSOLUTE(_user_data_end) || ABSOLUTE(_factory_start) > ABSOLUTE(_code_base), "Calib data is overlapped with user data or factory data is overlapped with code.")
    /*Check that the calib data in flash do not overwrite
     * the factory setting data space */
    ASSERT(ABSOLUTE(_factory_start) >= ABSOLUTE(_calib_start), "Factory setting data is overlapped with calib data.")
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
