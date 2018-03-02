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

#ifdef CT_COMPRESS_CODE_SECTION
#include "exclude_file_list.ld"
#endif

/*Mandatory to have the BB side irq handler linked */
ENTRY (boot_Sector)
EXTERN (__bb_int_handler)
EXTERN (_sxr_Irq_handler)
EXTERN (boot_Loader)
EXTERN (boot_Sector)
EXTERN (__flash_start)

SECTIONS
{
    /* ========================================== */
    /* set current location counter to flash base */
    /* ========================================== */
     . = 0xffffffff81e00200;
    .boot_irqcode  . : AT (ADDR(.boot_irqcode))
    {
	    *hal_irq_handler.o(.irqtext)
	    . = ALIGN(0x10);
		}
		
    . = INT_ROM_BASE;

    _flash_start = .;
    /* ============================= */
    /* boot sector in flash          */
    /* ============================= */
    .boot_code _flash_start : AT (ADDR(.boot_code))
    {
        . = 0x0000;
        *boot_rom.o(.text .text.*)
        *boot_rom.o(.rdata)
        *boot_rom.o(RODATA)
    }
    
	
		. += 0x300;
    .boot_code2  . : AT (ADDR(.boot_code2))
    {
        *boot_monitor.o(.text .text.*)
        *boot_monitor.o(.rdata)
        *boot_monitor.o(RODATA)
        *boot_host.o(.text .text.*)
        *boot_host.o(.rdata)
        *boot_host.o(RODATA)


#ifdef ROMULATOR
        LONG(0xD9EF0045)
#else /* ROMULATOR */
        LONG(0xFFFFFFFF)
#endif /* ROMULATOR */

#ifndef RAMRUN

        . = ALIGN(0x10);
        _boot_sector = .;
        *(.boot_sector_start)
        *boot_sector.o(.text .text.*)
        *boot_sector_chip.o(.text .text.*)
#endif /* RAMRUN */
        . = ALIGN(0x10);
    }

    /* ============================== */
    /* Main entry point after monitor */
    /* ============================== */
#ifndef RAMRUN
#if (CODE_BASE != FLASH_BASE)
    /* ========================================== */
    /* set current location counter to code base if it differs from flash base */
    /* ========================================== */
    . = CODE_BASE;
#endif /* CODE_BASE != FLASH_BASE */
#endif /* RAMRUN */

    .main_entry_section . : AT (ADDR(.main_entry_section))
    {
#ifndef RAMRUN
        _code_base = .;
        *boot_loader.o(.text.boot_loader_start)
        *boot_loader.o(.text .text.* .rdata .rodata .rodata.*)
#endif /* RAMRUN */

        _board_config_start = .;
        *(.board_config)
        . = ALIGN(0x10);
        _board_config_end = .;
#ifndef RAMRUN
        . = ALIGN(0x10);
#endif /* RAMRUN */
    }
    _board_config_size = _board_config_end - _board_config_start;

    _int_rom_tmp_flash_start_location = .;
    _int_rom_tmp_flash_end_location = _int_rom_tmp_flash_start_location;
    _sramtext_flash_start_location = _int_rom_tmp_flash_end_location;


    /* ================================================= */
    /* in BT Share SRAM                                        */
    /* ================================================= */
    . = BT_SRAM_BASE;
    _bt_sram_start = .;
    
    /* Go Uncached */
    /*. += 0x20000000;*/

    .btsram . : AT (ADDR(.btsram))
    {
        _bt_sram_uncached_start = .;
        *(BT_TRSPORT_SECTION)
        . = ALIGN(0x10);
    }
    _bt_sram_uncached_end = .;
    _bt_sram_uncached_size = _bt_sram_uncached_start - _bt_sram_uncached_end;

    _bt_sram_size = _bt_sram_uncached_size;
    _bt_sram_end = _bt_sram_start + _bt_sram_size;

    LD_ASSERT(ABSOLUTE(BT_SRAM_BASE + BT_SRAM_SIZE) > ABSOLUTE(_bt_sram_end), "BTGS", "BTSRAM overflow out.")

    /* ===================================================== */
    /* in SYS SRAM                                           */
    /* ===================================================== */
    _sys_sram_flash_start_location = _sramtext_flash_start_location;

#if (defined(ROM_IN_FLASH_ELF) && !defined(RAMRUN))
    /* ============================================= */
    /* rom sys sram data in flash elf (for GDB only) */
    /* ============================================= */
    .rom_entries_cached ROM_MISC_BASE : AT(ADDR(.rom_entries_cached))
    {
        KEEP(*.elf(.rom_entries_cached))
    }
#endif /* ROM_IN_FLASH_ELF && !RAMRUN */

    /*----------------- */
    /* SYS SRAM UNCACHED */
    /*----------------- */

    /* HAL Boot Sector structure, contains information written by
     * HAL during the normal execution of the code and used by the
     * Boot Sector when the phone boots.
     * It is mapped in the Internal SRAM because we don't want the Boot
     * Sector code to access the External RAM. And it is mapped at a fixed
     * location (after the last patch for the chip) because we
     * don't want the RAMRUN to overwrite it.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process.
     * For now, the size of the structure g_halBootSectorStruct is 4*4 bytes. */
    /* Use the last patch for the structure */
    .hal_boot_sector_struct (BOOT_SECTOR_STRUCT_BASE) (NOLOAD) :
        AT (ADDR(.hal_boot_sector_struct))
    {
        KEEP(*(.hal_boot_sector_struct))
    }

#ifdef RAMRUN
    /* For ramruns, this section loads an execution context
     * content allowing to directly executed the 
     * boot sector (preloaded with PC=boot_Sector and SP=0,
     * cf boot_sector.c) */
    .executor_ctx ROM_MISC_UBASE : AT (ADDR(.executor_ctx))
    { 
        /* For ramruns, avoid removing .executor_ctx even when option
         * --gc-sections is enabled, as its content must be loaded from
         * the ramrun lod */
        KEEP(*(.executor_ctx))
    }
#endif /* RAMRUN */

    /*----------------- */
    /* SYS SRAM CACHED  */
    /*----------------- */

    /* Boot Sector reload structure, contains information written by
     * HAL during the normal execution of the code and used by the
     * Boot Sector to reload the ram image when the phone boots.
     * It is mapped in the Internal SRAM because we don't want the Boot
     * Sector code to access the External RAM. And it is mapped at a fixed
     * location (after the last patch for the chip) because we
     * don't want the RAMRUN to overwrite it.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process. */
    .boot_sector_reload_struct ( SYS_SRAM_AVAILABLE_BASE-4-4
                                 -SIZEOF(.hal_boot_sector_struct)
                                 -(2*4+3*3*4) ) (NOLOAD) :
        AT (ADDR(.boot_sector_reload_struct))
    {
        KEEP(*(.boot_sector_reload_struct))
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

    .fixptr (SYS_SRAM_AVAILABLE_BASE-4) : AT (ADDR(.fixptr))
    {
        KEEP(*(.fixptr))
    }

#ifdef NO_SRAM_TEXT
    . = _sramtext_flash_start_location;
    .sramtext . : AT (ADDR(.sramtext))
#else
#ifndef PRELOADED_INT_SRAM
    .sramtext (SYS_SRAM_AVAILABLE_BASE) : AT (_sys_sram_flash_start_location)
#else
    .sramtext (SYS_SRAM_AVAILABLE_BASE) : AT (ADDR(.sramtext))
#endif
#endif
    {
    _sys_sram_start = .;
    _sys_sram_cached_start = .;
    _sys_sram_txt_start = .;


    
    }
    _sys_sram_txt_end = .;
    _sys_sram_txt_size = _sys_sram_txt_end - _sys_sram_start;
#ifdef NO_SRAM_TEXT
    _sys_flash_start_location = .;
    . = SYS_SRAM_AVAILABLE_BASE;
#else    
    _sys_flash_start_location = _sys_sram_flash_start_location;    
#endif    

#ifndef PRELOADED_INT_SRAM
    .sramdata . : AT (_sys_sram_flash_start_location + _sys_sram_txt_size )
#else
    .sramdata . : AT (ADDR(.sramdata))
#endif

    {
        _sys_sram_data_start = .;
        *(.sramdata)

        *pal_gsmr.o(.data .data.* .gnu.linkonce.d.* .data1)
        *pal_gsmr.o(.sdata .sdata.* .gnu.linkonce.s.*)
        *pal_gsmr.o(.sdata2 .sdata2.* .gnu.linkonce.s2.*)

        *(.srrodata)
    }
    _sys_sram_data_end = .;
    _sys_sram_cached_end = .;
    _sys_sram_data_size = _sys_sram_data_end - _sys_sram_data_start;
    _sys_sram_cached_size = _sys_sram_cached_end - _sys_sram_cached_start;

    /*-------------------- */
    /* SYS SRAM UNCACHED   */
    /*-------------------- */
    /* Go Uncached */
    /*. += 0x20000000;*/

#ifndef PRELOADED_INT_SRAM
    .sramucdata . : AT ( _sys_sram_flash_start_location + _sys_sram_cached_size )
#else
    .sramucdata . : AT (ADDR(.sramucdata))
#endif
    {
        _sys_sram_uncached_start = .;
        *(.sramucdata)
        *(.srroucdata)
        . = ALIGN(0x10);
    }
    _sys_sram_uncached_end = .;
    _sys_sram_uncached_size = _sys_sram_uncached_end - _sys_sram_uncached_start;
    
    _ram_flash_start_location = _sys_sram_flash_start_location + _sys_sram_cached_size + _sys_sram_uncached_size;
  
    /* ------------------------------------------------ */
    /* Ext RAM CACHED  - SECTIONS ALWAYS IN RAM */
    /* ------------------------------------------------ */
    /* Go Cached */
    /*. -= 0x20000000;*/
        
#ifndef PRELOADED_RAM
    .ram . : AT (_ram_flash_start_location )
#else
    .ram . : AT (ADDR(.ram))
#endif
    {
        _ram_start = .;
        _ram_special_cached_start = .;
        _ram_txt_start = .;
        *(.ram)
    }
    _ram_special_cached_end = .;
    _ram_special_cached_size = _ram_special_cached_end - _ram_special_cached_start;

    _ram_cached_txt_end = . ;
    ram_cached_txt_size = 0;

    _ram_txt_end = .;

    /*---------------------- */
    /* Ext RAM UNCACHED Data */
    /*---------------------- */
    /* Go Uncached */
    /*. += 0x20000000;*/

#ifndef PRELOADED_RAM
    .ucdata  . : AT ( _ram_flash_start_location + _ram_special_cached_size + ram_cached_txt_size )
#else
    .ucdata  . : AT (ADDR(.ucdata))
#endif
    {
        _ram_uncached_data_start = .;
        *(.ucdata)
        *(*.globalvars.uc)
        . = ALIGN(0x10);
    }
    _ram_uncached_data_end = .;
    _ram_uncached_data_size = _ram_uncached_data_end - _ram_uncached_data_start;

    /*---------------------- */
    /* Ext RAM CACHED Data */
    /*---------------------- */
    /* Go Cached */
    /*. -= 0x20000000;*/

#ifndef PRELOADED_RAM
    .data   . : AT ( _ram_flash_start_location + _ram_special_cached_size + ram_cached_txt_size + _ram_uncached_data_size )
#else
    .data   . : AT (ADDR(.data))
#endif
    {
        _ram_cached_data_start = . ;
#ifdef CONST_IN_RAM      
        *(.rdata)    
        *(.rodata .rodata.* .gnu.linkonce.r.*)       
        *(.rodata1)      
        *(.ctors)
        . = ALIGN(0x10);
#endif
        /* The flash driver must always be in RAM
         * because we need to be able to execute its code
         * when writing any sectors (For examplen, the calibration
         * sector which is on the first bank. 
         * And so must be the functions called by the 
         * flash drivers. */
        *drv_flsh.o(.rdata .rodata .rodata.* .gnu.linkonce.r.* .data .data.* .gnu.linkonce.d.* )
        *drv_flsh_map.o(.rdata .rodata .rodata.* .gnu.linkonce.r.* .data .data.* .gnu.linkonce.d.* )
        *hal_ebc.o(.rdata .rodata .rodata.* .gnu.linkonce.r.* .data .data.* .gnu.linkonce.d.* )
        *hal_flash.o(.rdata .rodata .rodata.* .gnu.linkonce.r.* .data .data.* .gnu.linkonce.d.* )
		
        /* NOTE: globalvars.c sections are to be removed.
         * They are kept here for backward compatibility of SVC/VPP codes.
         * These codes are linked from Total_BJ branch and the special global
         * variable sections will be removed in the future. */
        *(*.globalvars.c *.globalvars.c.*)

        *(.data .data.* .gnu.linkonce.d.*)
        *(.data1)
        _gp = ALIGN(16) + 0x7ff0;
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
        *(.lit8)
        *(.lit4)
        . = ALIGN(0x4);
        /***barrier : do not remove or edit */
        . = ALIGN(0x10);
    }
    _ram_cached_data_end = . ;
    _ram_cached_data_size = _ram_cached_data_end - _ram_cached_data_start;
    _ram_end = .;
    _ram_size = _ram_end - _ram_start;
  
    _sys_sram_size = _sys_sram_cached_size + _sys_sram_uncached_size + _ram_size;
    _sys_sram_end = _sys_sram_start + _sys_sram_size;

#ifndef PRELOADED_INT_SRAM
    _sys_sram_flash_end_location = _sys_sram_flash_start_location + _sys_sram_size;
#else
    _sys_sram_flash_end_location = _sys_sram_flash_start_location;
#endif

    /*----------------------- */
    /* SYS SRAM BSS           */
    /*----------------------- */
    .sramucbss . (NOLOAD) : AT (ADDR(.sramucbss))
    {
        _sys_sram_ucbss_start = .;
        *(.sramucbss)
        . = ALIGN(0x10);
    }
    _sys_sram_ucbss_end = .;
    _sys_sram_ucbss_size = _sys_sram_ucbss_end - _sys_sram_ucbss_start;

    .srambss . (NOLOAD) : AT (ADDR(.srambss))
    {
        _sys_sram_bss_start = .;
        *(.srambss)
        . = ALIGN(0x10);
    }
    _sys_sram_bss_end = .;
    _sys_sram_bss_size = _sys_sram_bss_end - _sys_sram_bss_start;

#if (CHIP_HAS_PROXY_MEMORY == 1)
    /* ========================================== */
    /* Proxy Memory                               */
    /* ========================================== */
    /* Standard usage of proxy memory */
    /* Manual placing and copy from flash */
    _proxy_memory_flash_start_location = _sys_sram_overlay_flash_end_location;
#ifndef PRELOADED_PROXY_MEMORY
    .proxy_memory SYS_PROXY_MEM_BASE : AT (_proxy_memory_flash_start_location)
#else
    .proxy_memory SYS_PROXY_MEM_BASE : AT (ADDR(.proxy_memory))
#endif
    {
        _proxy_memory_start = .;
/* Loadable as a ramrun since the proxy memory is enabled
   by the romed monitor.*/ 
        *sxs_rlm.o(.text .text.*)
        *sxr_rlm.o(.text .text.*)
        *hal_timer.o(.text .text.*)
        *sxr_tksd.o(.text .text.*)
        *sxs_io.o(.text .text.*)
        *sxr_tls.o(.text .text.*)
        *sxs_tls.o(.text .text.*)
        *sxr_sbx.o(.text .text.*)
        *sxs_sbx.o(.text .text.*)
        *sxr_tim.o(.text .text.*)
        *hal_trace.o(.text .text.*)
        . = ALIGN(0x10);
    }

    _proxy_memory_end = . ;
    _proxy_memory_size = _proxy_memory_end - _proxy_memory_start ;

#ifndef PRELOADED_PROXY_MEMORY
    _proxy_memory_flash_end_location = _proxy_memory_flash_start_location + _proxy_memory_size;
#else
    _proxy_memory_flash_end_location = _proxy_memory_flash_start_location;
#endif

#else /* CHIP_HAS_PROXY_MEMORY */
    _fast_cpu_memory_overlay_flash_end_location = _sys_sram_flash_end_location;
#endif /* CHIP_HAS_PROXY_MEMORY */


    /* ========================================== */
    /* continued in External RAM                  */
    /* ========================================== */
    /*_ram_flash_start_location = _sys_sram_flash_end_location;*/

#ifndef RAMRUN
    /* . = _ram_available_base; */
#else /* RAMRUN */
    /* In a RAMRUN, all the flash content will be put in the ram, and
     * _ram_flash_start_location is in fact pointing to a ram location. */
    . = _sys_sram_flash_end_location;
#endif /* RAMRUN */

    /*-------------------------------------------------------  */
    /* Uninitialized section in RAM (content kept after reboot)*/
    /*-------------------------------------------------------  */
    .backup . : AT (ADDR(.backup))
    {
        *(.backup)
        . = ALIGN(0x10);
    }

    /*-------------------------------------------------------  */
    /* RAM BSS (content reset to 0 )                           */
    /*-------------------------------------------------------  */
    /* Go Uncached */
    /*. += 0x20000000;*/

    .ucbss    . (NOLOAD) : AT (ADDR(.ucbss))
    {
        _ram_ucbss_start = .;
        *(.ucbss)
        *(.ramucbss)
        
        . = ALIGN(0x10);
    }
    _ram_ucbss_end = .;
    _ram_ucbss_size = _ram_ucbss_end - _ram_ucbss_start;

    /* Go Cached */
    /*. -= 0x20000000;*/

    .bss    . (NOLOAD) : AT (ADDR(.bss))
    {
        _ram_bss_start = .;
        /* The flash driver must always be in RAM
         * because we need to be able to execute its code
         * when writing any sectors (For examplen, the calibration
         * sector which is on the first bank. 
         * And so must be the functions called by the 
         * flash drivers.*/
        *drv_flsh.o(.bss .bss.* .gnu.linkonce.b.* COMMON)
        *drv_flsh_map.o(.bss .bss.* .gnu.linkonce.b.* COMMON)
        *hal_ebc.o(.bss .bss.* .gnu.linkonce.b.* COMMON)
        *hal_flash.o(.bss .bss.* .gnu.linkonce.b.* COMMON)

        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        *(.rambss)
        . = ALIGN(0x10);
    }
    _ram_bss_end = . ;
    _ram_bss_size = _ram_bss_end - _ram_bss_start;

    .heap . (NOLOAD) : AT (ADDR(.heap))
    {
        *cos_mem.o(.mempool)
    }
    _end_of_mempool = . ;


    /*----------------------- */
    /* SYS SRAM HEAP 1        */
    /*----------------------- */    
    _sys_sram_top = SYS_SRAM_AVAILABLE_BASE + SYS_SRAM_SIZE; /* End of internal sram */
    _sys_sram_heap_start = _ram_bss_end;
    _sys_sram_heap_end = _sys_sram_top - (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (XCPU_IRQ_STACK_SIZE);
    _sys_sram_heap_size = _sys_sram_heap_end - _sys_sram_heap_start;
	  _heap_size = _sys_sram_heap_size;
	  
    /* Check that the INT SRAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_sys_sram_heap_end + 0x20000000) > ABSOLUTE(_sys_sram_heap_start), "INTS", "Internal SRAM overflow !")

    /*----------------------- */
    /* SYS SRAM IRQ STACK WATCH */
    /*----------------------- */
    _sys_sram_irq_stackwatch_size = (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (PS_STACK_SIZE) ;
    _sys_sram_irq_stackwatch_start = _sys_sram_heap_end;
    _sys_sram_irq_stackwatch_end = _sys_sram_irq_stackwatch_start + _sys_sram_irq_stackwatch_size;

    /*----------- */
    /* End of RAM */
    /*----------- */
    /* no extern ram */
    _heap_start = _sys_sram_heap_start; /* only for link error */
    /*_ram_top = SYS_SRAM_AVAILABLE_BASE + SYS_SRAM_SIZE;
    _heap_size = _ram_top -_ram_bss_end;
    _heap_start =  _ram_bss_end;
    _heap_end =  _ram_bss_end + _heap_size;*/

#if (PAGE_SPY_LOG == 1)    
    _sys_sram_sp_stack_start = _sys_sram_irq_stackwatch_end;
    _sys_sram_sp_stack_end = . + PS_STACK_SIZE - 4;
#endif

    /* Caution, _sys_sram_non_romed_top is used by the boot ROM code. */
    .sram_romed_top SYS_SRAM_BASE + SYS_SRAM_SIZE - RESERVED_MBX_SIZE - 4 (NOLOAD) :
        AT (ADDR(.sram_romed_top))
    {
        _sys_sram_non_romed_top = .;
    }

#if (defined(ROM_IN_FLASH_ELF) && !defined(RAMRUN))
    /* ============================================ */
    /* rom mailbox data in flash elf (for GDB only) */
    /* ============================================ */
    .mailbox SYS_SRAM_BASE + SYS_SRAM_SIZE - RESERVED_MBX_SIZE + 0x20000000 : AT (ADDR(.mailbox))
    {
        KEEP(*.elf(.mailbox))
    } 
#endif /* ROM_IN_FLASH_ELF && !RAMRUN */

    /*---------------- */
    /* End of SYS SRAM */
    /*---------------- */
    
    /* Check that the external RAM is not over stuffed */
    /*LD_ASSERT(ABSOLUTE(_ram_top) > ABSOLUTE(_ram_bss_end), "EXTS", "External RAM overflow !")*/

    /*========================================================= */
    /* In FLASH                                                 */
    /*========================================================= */
    _rom_flash_start_location =  _sys_sram_flash_end_location;/* + _ram_bss_size;*/

    /* .crossrefs section can be used to list functions which is share by other application */
    .crossrefs _rom_flash_start_location : AT (ADDR(.crossrefs))
    {
    	*(.crossref)
    	. = ALIGN(0x10);
    }

    .sxr . : AT(ADDR(.sxr))
    {
        *hal_irq_handler.o(.text .text.* .sramtext)
        *(.sramtext)
    
    #ifdef RAMRUN
        _boot_sector = .;
        *(.boot_sector_start)
        *boot_sector.o(.text .text.*)
        *boot_sector_chip.o(.text .text.*)
        
        _code_base = .;
        *boot_loader.o(.text.boot_loader_start)
        *boot_loader.o(.text .text.* .rdata .rodata .rodata.*)
    #endif /* RAMRUN */
    
        *pal_gsmr.o(.text .text.* .gnu.linkonce.t.* .stub)
        *sxr_mem.o(.text .text.* .gnu.linkonce.t.* .stub)
        *sxr_jbsd.o(.text .text.* .gnu.linkonce.t.* .stub)
    }

    /*---------------------------- */
    /* Ext RAM CACHED TXT          */
    /*---------------------------- */
    .text  .  : AT (ADDR(.text))
    {
        _ram_cached_txt_start = . ;
        /* The flash driver must always be in RAM
         * because we need to be able to execute its code
         * when writing any sectors (For examplen, the calibration
         * sector which is on the first bank. 
         * And so must be the functions called by the 
         * flash drivers. */
        *drv_flsh.o(.text .text.* .rotext .stub .gnu.linkonce.t.*)
        *hal_ebc.o(.text .text.* .rotext .stub .gnu.linkonce.t.*)
        *hal_flash.o(.text .text.* .rotext .stub .gnu.linkonce.t.*)
#ifdef CODE_IN_RAM
        *(.text .text.* .stub .gnu.linkonce.t.*)
        . = ALIGN(0x10);
#endif
    }

#ifndef CT_COMPRESS_CODE_SECTION
    .init . : AT(ADDR(.init))
    {
    	*(.init)
    	. = ALIGN(0x10);
    }
#endif

    .rotext . : AT (ADDR(.rotext))
    {
        #ifndef CODE_IN_RAM
            *(EXCLUDE_FILE(EXCLUDE_FILE_LIST) .text)
            *(EXCLUDE_FILE(EXCLUDE_FILE_LIST) .text.*)
            *(.stub .gnu.linkonce.t.*)
        #endif
        . = ALIGN(0x10);
    }

    .rodata . : AT (ADDR(.rodata))
    {
        #ifndef CONST_IN_RAM
            *(.rdata)
            *(EXCLUDE_FILE(EXCLUDE_FILE_LIST *feature.o *bigcode.o *index.o *pag1.o) .rodata)
            *(EXCLUDE_FILE(EXCLUDE_FILE_LIST *feature.o *bigcode.o *index.o *pag1.o) .rodata.*)
            *(.gnu.linkonce.r.*) 
            *(.rodata1)
            *(.ctors)
        #endif
        . = ALIGN(0x10);
    }

    .roresdata . : AT (ADDR(.roresdata))
    {
        *(.roresdata)
        *(.robsdata)
#if (EXTRA_CODE_SIZE == 0)
        *(.extra)
#endif
        . = ALIGN(0x10);
    }
#ifdef CONST_IN_RAM
    /* Multiple discard output sections will be combined into one. 
     * Not sure if ld can handle multiple discard sections correctly. */
    LD_ASSERT(SIZEOF(.roresdata) == 0, "RESE", "Resource data cannot be loaded in ram.")
#endif

    .rohandwrite . : AT (ADDR(.rohandwrite))
    {
    	*feature.o(.rodata .rodata.*)
    	*bigcode.o(.rodata .rodata.*)
    	*index.o(.rodata .rodata.*)
    	*page1.o(.rodata .rodata.*)
    	. = ALIGN(0x10);
    }

#ifdef CT_COMPRESS_CODE_SECTION
    _compressed_code_start = . ;
    #include "compress_code_section.ld"
    _compressed_code_end = . ;
#ifdef LINK_STAGE_2
    _flash_end = _compressed_code_start + SIZEOF(.compressed);
#else
    _flash_end = . ;
#endif
#else /* !CT_COMPRESS_CODE_SECTION */
    _flash_end = . ;
#endif /* !CT_COMPRESS_CODE_SECTION */

    /* kept for compatibility */
    _etext = _ram_txt_end;
    _esramtext = _sys_sram_txt_end;
    _end = _ram_bss_end;


    /*=================================================== */
    /* Placeholder for misc hacks, not used for real apps */
    /*=================================================== */
    .misc . : AT (ADDR(.misc))
    {
        *(.misc)
#ifdef CT_COMPRESS_CODE_SECTION
        *(.compress*)
#endif        
    }

#if (EXTRA_CODE_SIZE != 0)
    .extra EXTRA_CODE_BASE : AT(ADDR(.extra))
    {
    	_extra_section_start = .;
        *(.extra)
        . = ALIGN(0x10);
    }
    _extra_section_end = .;
    _extra_section_size = _extra_section_end - _extra_section_start;
    extra_rores_size = EXTRA_CODE_SIZE;
    
    LD_ASSERT(ABSOLUTE(extra_rores_size) > ABSOLUTE(_extra_section_size), "FLAS", "extra section overflow: Please check EXTRA_RORES_SIZE at memd.def and the .extra section size.")
    extra_remain_size = extra_rores_size - _extra_section_size;
#endif


    /*================================================= */
    /* Define user data boundaries based on the defines */
    /*================================================= */
    _user_data_start = USER_DATA_BASE;
    _user_data_end = USER_DATA_BASE+USER_DATA_SIZE;
    _user_data_size = USER_DATA_SIZE;

    /*Check that the data and code in flash do not overwrite
     * the user data space */
    /* ASSERT seems to be boguous as of binutils-2.15 : we are
     * not allowed to used error messages longer than FLAS in that
     * particular case. Don't ask and port a newer version,
     * maybe. FIXME */
#define CHECK_FLASH_OVERFLOW 1

#ifdef CT_COMPRESS_CODE_SECTION
#ifndef LINK_STAGE_2
#undef CHECK_FLASH_OVERFLOW
#endif
#endif

#ifdef CHECK_FLASH_OVERFLOW
    LD_ASSERT(ABSOLUTE(_user_data_start) > ABSOLUTE(_flash_end), "FLAS", "Flash overflow: there is no more space available in flash memory.")
#endif /* CHECK_FLASH_OVERFLOW */


    /* ============================== */
    /* calib sector in flash          */
    /* ============================== */
    
    /* no calib for mp3host */
    /*.calib CALIB_BASE : AT (ADDR(.calib))
    {
        _calib_start = .;
        *(.calib);
        . = ALIGN(0x10);
    }
    _calib_end = .;
    _calib_size = _calib_end - _calib_start;*/


    /* ============================== */
    /* factory settings in flash      */
    /* ============================== */

    .factory_settings FACT_SETTINGS_BASE : AT (ADDR(.factory_settings))
    {
        _factory_start = .;
        *(.factory_settings)
    }
    _factory_end = .;
    _factory_size = _factory_end - _factory_start;

    /* Legal layout patterns:
     * 1) Code (including boot code) -> User Data -> Calib Data -> Factory Data
     * 2) Boot Sector -> Calib Data -> Factory Data -> Code (excluding boot code) -> User Data
     * /

    /*Check that the user data in flash do not overwrite
     * the calib data or factory setting space */
    LD_ASSERT(ABSOLUTE(_factory_start) >= ABSOLUTE(_user_data_end),
              "CFOV", "Calib data is overlapped with user data or factory data is overlapped with code.")

    /*Check that the calib data in flash do not overwrite
     * the factory setting data space */
    /*LD_ASSERT(ABSOLUTE(_factory_start) > ABSOLUTE(_calib_start),
              "FACO", "Factory setting data is overlapped with calib data.")*/

    #include "discard.ld"
}
