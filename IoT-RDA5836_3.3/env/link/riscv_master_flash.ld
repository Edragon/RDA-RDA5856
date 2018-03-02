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

#define BSS()  .bss .bss.* COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.* .rambss .gnu.linkonce.b.*
#define DATA()  .data .data.*
#define RODATA() .rdata .rodata .rodata.str1.4 .rodata.*
#define ROTEXT() .text .text.*
#define INPUT_SECTIONS() ROTEXT() RODATA()

#ifdef __NATIVE_JAVA__
#define EXCLUDE_JAVA_OBJ EXCLUDE_FILE(gcj_*.o *.oo)
#else
#define EXCLUDE_JAVA_OBJ
#endif


/*Mandatory to have the BB side irq handler linked */
#if CHIP_HAS_BTCPU==1
ENTRY (boot_Sector)
#else
ENTRY (_flash_xcpu_start)
EXTERN (boot_Sector)
#endif
EXTERN (__bb_int_handler)
EXTERN (_sxr_Irq_handler)
EXTERN (boot_LoaderEnter)
EXTERN (boot_Sector)
EXTERN (boot_Sector_Nand)


SECTIONS
{
#ifdef COMPRESS_PROCESS_ENABLE
    /* The load address of the compressed code should be set */
    /* to an address at which no LOD data will be generated. */
    /* Here it is set to (EXT_RAM_BASE + RAM_SIZE). */
    _compressed_code_load_start = EXT_RAM_BASE + RAM_SIZE;
#endif


#ifndef RAMRUN
    /* ========================================== */
    /* set current location counter to flash base */
    /* ========================================== */

    . = FLASH_BASE;
    _flash_start = .;

#if CHIP_HAS_BTCPU==1
    . = BT_FLASH_END;
#else
	.boot_head . : AT (ADDR(.boot_head))
	{
		*boot_flash.o(.text)
		. = ALIGN(0x10);
	}
		
#ifdef LOAD_BCPU_ELF
    /* ======================================== */
    /* BCPU elf in XCPU elf */
    /* ======================================== */
    #include "link_bcpu_elf.ld"

    . = _bcpu_flash_end;
#endif	
	.bt_code . : AT (ADDR(.bt_code))	
	{
		*lmp_*.o(INPUT_SECTIONS())
		*lslc_*.o(INPUT_SECTIONS())
		*uslc_*.o(INPUT_SECTIONS())
		*lelc_*.o(INPUT_SECTIONS())
		*avdtp_*.o(INPUT_SECTIONS())
		*avrcp_*.o(INPUT_SECTIONS())
		/*host_*.o(EXCLUDE_FILE(hal_*.o) INPUT_SECTIONS()) */
		*hfp_*.o(INPUT_SECTIONS())
		*rfcomm_*.o(INPUT_SECTIONS())
		*l2_*.o(INPUT_SECTIONS())
		*sdp_*.o(INPUT_SECTIONS())
		*rdabt_*.o(INPUT_SECTIONS())

		. = ALIGN(0x100);
		_bt_flash_end = .;
	}

#endif

    _xcpu_flash_start = .;

#endif /* !RAMRUN */

    /* ============================= */
    /* boot sector in flash          */
    /* ============================= */
    .boot_code _xcpu_flash_start : AT (ADDR(.boot_code))
    {
#if 0 /* CHIP_HAS_BTCPU==0*/
    		LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        . = ALIGN(0x10);
#endif /* CHIP_HAS_BTCPU */

#ifndef RAMRUN

        . = ALIGN(0x10);
        _boot_sector = .;
        *(.boot_sector_start)
        *boot_sector.o(INPUT_SECTIONS())
        *boot_sector_chip.o(INPUT_SECTIONS())
        *(.boottext .bootrodata)
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
    LD_ASSERT(ABSOLUTE(CODE_BASE) >= ABSOLUTE(.), "CBE", "CODE_BASE is invalid. \
Note that CODE_BASE is designed to be used only when calib and factory sectors are located at flash forepart, \
and it should never be set in all other cases.")

    . = CODE_BASE;

#endif /* CODE_BASE != FLASH_BASE */
#endif /* RAMRUN */

    .main_entry_section . : AT (ADDR(.main_entry_section))
    {
#ifndef RAMRUN
        _code_base = .;
        *boot_loader.o(.text.boot_loader_start)
        *boot_loader.o(INPUT_SECTIONS())
        . = ALIGN(0x10);
#endif /* RAMRUN */

        _board_config_start = .;
        *(.board_config)
        . = ALIGN(0x10);
        _board_config_end = .;
    }
    _board_config_size = _board_config_end - _board_config_start;

    _int_rom_flash_start_location = .;

#if defined(LOAD_ROM_ELF)
    /* ======================================== */
    /* internal rom in flash elf */
    /* ======================================== */
    #include "link_rom_elf.ld"

    /* #include "link_btcpu_elf.ld" */
   /* #include "link_btcpu_rom_elf.ld"*/

#endif

#if (defined(REWRITE_ROM_AT_BOOT) && defined(FPGA))
    _int_rom_flash_end_location = .;
#else
    _int_rom_flash_end_location = _int_rom_flash_start_location;
#endif


    /* ================================================= */
    /* in BB SRAM                                        */
    /* ================================================= */

    . = BB_SRAM_BASE;

    _bcpu_sys_sram_bss_start = .;


    _bcpu_sys_sram_bss_end = .;
    _bcpu_sram_bss_size = _bcpu_sys_sram_bss_end - _bcpu_sys_sram_bss_start;

    _bcpu_ram_heap_start = _bcpu_sys_sram_bss_end;
    _bcpu_ram_heap_size = BB_SRAM_SIZE - _bcpu_sram_bss_size;

    LD_ASSERT(ABSOLUTE(BB_SRAM_BASE + BB_SRAM_SIZE) >= ABSOLUTE(_bcpu_sys_sram_bss_end), "BBGS", "BBSRAM not romed globals overflow out of BBSRAM.")

    /* ================================================= */
    /* in DUALPORT SRAM                                  */
    /* ================================================= */
#if (NAND_FLASH == 1)
    . = BB_DUALPORT_SRAM_BASE + NAND_FLASH_BOOT_CODE_SIZE;
#else
    . = BB_DUALPORT_SRAM_BASE;
#endif

    _dualport_bss_start = .;
    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .dualport_sram_ubss . (NOLOAD) : AT (ADDR(.dualport_sram_ubss))
    {
#if (CHIP_ASIC_ID == 15) /* CHIP_ASIC_ID_5856 */
        *hal_sharemem.o(.dualucbss)
        *rdabt_sharemem.o(.dualucbss)
        *dsm_dev_driver.o(.dualucbss)
        *hal_bt.o(.dualucbss)
#endif
#if (CHIP_BTOVERLAY_SUPPORTED == 1)
				*overlay.o(.pcm_big_datamem)
		  	*BTSco.o(BSS())
        *plc.o(BSS())
        *vpp_speech_bt.o(BSS())
        *hal_speech_bt.o(BSS())
        *mmc_audiosbc.o(BSS())
        *mmc_audiosco.o(BSS())
        *mmc_audiompeg.o(BSS())
        *mmc_audiocommon.o(BSS())
        *mmc_audiotone.o(BSS())
        *mmc.o(BSS())
		*dsm_dev_driver.o(BSS())
        *vpp_audiojpeg_dec.o(.dualucbss BSS())
        *hal_voc.o(BSS())
        *hal_aif.o(BSS())
        *aud_core*.o(BSS())
        *aud_codec_gallite.o(BSS())
        *aud_codec_common.o(BSS())
        *hal_rda_audio.o(BSS())
        *dm_tone.o(BSS())
        *mci.o(BSS())
        *sysmalloc.o(BSS())
#endif
        . = ALIGN(0x10);
    }
    /* Go Uncached */
    . -= MEM_CACHED_UNCACHED_OFFSET;
    _dualport_bss_end = .;

    /* ===================================================== */
    /* in SYS SRAM                                           */
    /* ===================================================== */

    /*----------------- */
    /* SYS SRAM UNCACHED */
    /*----------------- */

#ifdef RAMRUN
    /* For ramruns, this section loads an execution context
     * content allowing to directly executed the 
     * boot sector (preloaded with PC=boot_Sector and SP=0,
     * cf boot_sector.c) */
    .executor_ctx ROM_MISC_UBASE : AT (ADDR(.executor_ctx) - MEM_CACHED_UNCACHED_OFFSET)
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

    /*LD_ASSERT(ABSOLUTE(__int_rom_reserved_end) <= ROM_MISC_BASE + ROM_MISC_SIZE, "RRRO", "Reserved Ram Reserved for Rom Overflowed")*/

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

    .hal_tflash_upgrade_struct . (NOLOAD) :
        AT (ADDR(.hal_tflash_upgrade_struct))
    {
        KEEP(*(.hal_tflash_upgrade_struct))
    }
	
    .boot_tflash_upgrade_struct_ptr (SYS_SRAM_AVAILABLE_BASE-4-4-4-4) (NOLOAD) :
        AT (ADDR(.boot_tflash_upgrade_struct_ptr))
    {
        KEEP(*(.boot_tflash_upgrade_struct_ptr))
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
    .fixptr (SYS_SRAM_AVAILABLE_BASE-4+MEM_CACHED_UNCACHED_OFFSET) (NOLOAD) : AT (ADDR(.fixptr))
    {
        KEEP(*(.fixptr))
    }

    . = SYS_SRAM_AVAILABLE_BASE;

    /* ------------------------------------------------ */
    /* SYS SRAM IRQ - IRQ HANDLER IN SYS SRAM */
    /* ------------------------------------------------ */
    _irq_sys_sram_flash_start_location = _int_rom_flash_end_location;

    _irq_sys_sram_start = .;

#ifndef PRELOADED_INT_SRAM
    .irqsram . : AT (_irq_sys_sram_flash_start_location)
#else
    .irqsram . : AT (ADDR(.irqsram))
#endif
    {
        *hal_irq_handler.o(.irqtext)
        *hal_irq_handler.o(.text .text.* .sramtext .rdata .rodata .rodata.*)
        . = ALIGN(0x10);
    }

    _irq_sys_sram_end = .;
    _irq_sys_sram_size = _irq_sys_sram_end - _irq_sys_sram_start;
    
#if CHIP_BTOVERLAY_SUPPORTED == 1
    /*LD_ASSERT(ABSOLUTE(_irq_sys_sram_end) <= _sys_rom_btoverly_start, "ORRO", "Overlay Ram in ROM Overflowed")*/
#endif

#ifndef PRELOADED_INT_SRAM
    _irq_sys_sram_flash_end_location = _irq_sys_sram_flash_start_location + _irq_sys_sram_size;
#else
    _irq_sys_sram_flash_end_location = _irq_sys_sram_flash_start_location;
#endif

#if defined(LOAD_BCPU_ELF)
    . = _bcpu_sys_sram_end;
#endif

#if CHIP_BTOVERLAY_SUPPORTED == 1
		/*. = _sys_rom_btoverly_end;*/
		 . += MEM_CACHED_UNCACHED_OFFSET;

		.bss_btoverlay . (NOLOAD) : AT (ADDR(.bss_btoverlay))
		{
       /* *ap_mgr.o(BSS())
        *ap_hfp.o(BSS())
        *ap_a2dp.o(BSS())
        *ap_avrcp.o(BSS())
        *ap_bas.o(BSS()) 
        host_*.o(BSS())
        mgr_*.o(BSS())
        rdabt_*.o(BSS())
        avrcp_*.o(BSS())
        avdtp_*.o(BSS())
        hfp_*.o(BSS())
        rfcomm_*.o(BSS())
        sdp_*.o(BSS())
        l2_*.o(BSS())
        att_*.o(BSS())*/
        lmp_*.o(BSS())
        tra_*.o(BSS())
        bt_*.o(BSS())
        sys_*.o(BSS())
        hc_*.o(BSS())
        uslc_*.o(BSS())
        *dl_dev_impl.o(BSS())
        *hw_radio.o(BSS())
        *lc_interface.o(BSS())
        *ll_link.o(BSS())
        lelc_*.o(BSS())
        lslc_*.o(BSS())
        
        . = ALIGN(0x10);
		}
		 . -= MEM_CACHED_UNCACHED_OFFSET;
			bss_btoverlay_end = .;
		/*bss_btoverlay_size = bss_btoverlay_end - _sys_rom_btoverly_start;

		. = _sys_rom_btoverly_start;*/
		
		bss_music_overlay_start = .;
    	bss_music_overlay . (NOLOAD) : AT (bss_music_overlay_start)
		/*.bss_music_overlay . (NOLOAD) : AT (ADDR(.bss_music_overlay)) */
		{
        *fmd.o(BSS())
        *ap_fm.o(BSS())
        *ap_linein.o(BSS())
        *mcd_sdmmc.o(BSS())
        *drv_mmc_chip.o(BSS())
        *uctls_fnc.o(BSS())
        *hal_usb_mgc.o(BSS())
        *uaud_service.o(BSS())
        *umss_service.o(BSS())
        *uhid_service.o(BSS())
        *hal_usb*.o(BSS())
        *hal_musb_dma.o(BSS())
        *mcd_usb.o(BSS())
        *usb_hcd.o(BSS())
        *scsi2.o(BSS())
        *bot.o(BSS())
        *ap_usbdisk.o(BSS())
        *Mmc_sndRec.o(BSS())
        *ap_music.o(BSS())
        *mmc_audiowav.o(BSS())
        *mmc_audiowma.o(BSS())
        /**mmc_audiompeg.o(BSS())*/
        *mmc_adpcm.o(BSS())
        /**vpp_audiojpeg_dec.o(BSS())*/
        *fs_api.o(BSS())
        *fs_list.o(BSS())
        *fs_base.o(BSS())
        *fs_namei.o(BSS())
        *fat_base.o(BSS())
        *ap_fselector.o(BSS())
        *scsi2.o(BSS())
        *usbmsc.o(BSS())
        *apfs.o(BSS())
        *ars.o(BSS())
        *ap_record.o(BSS())
        . = ALIGN(0x10);
		}

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .sramucbss_overlay . (NOLOAD) : AT (ADDR(.sramucbss_overlay))
    {
        *hal_usb*.o(.ucbss)
        *usb_hcd.o(.ucbss)
        *scsi2.o(.ucbss)
        *bot.o(.ucbss)
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;
	bss_music_overlay_end = .;

	bss_music_overlay_size = bss_music_overlay_end - bss_music_overlay_start;


	_bss_overlay_end = MAX(bss_music_overlay_end, bss_btoverlay_end);

	. = _dualport_bss_start; /* reuse bigbuff memory */
#endif

    /* ------------------------------------------------ */
    /* BOOTING TEXT AND DATA SECTIONS IN SYS SRAM */
    /* ------------------------------------------------ */
    _boot_sys_sram_flash_start_location = _irq_sys_sram_flash_end_location;


    _boot_sys_sram_start = .;

    _boot_sys_sram_txt_start = .;

#ifndef PRELOADED_INT_SRAM
    .bootsramtext . : AT (_boot_sys_sram_flash_start_location)
#else
    .bootsramtext . : AT (ADDR(.bootsramtext))
#endif
    {
        /* The flash driver must always be in RAM
         * because we need to be able to execute its code
         * when writing any sectors (For examplen, the calibration
         * sector which is on the first bank. 
         * And so must be the functions called by the 
         * flash drivers. */
        *drv_flsh.o(.text .text.* .rotext .stub .gnu.linkonce.t.*)
        *hal_ebc.o(.text .text.* .rotext .stub .gnu.linkonce.t.*)
        *hal_flash.o(.text .text.* .rotext .stub .gnu.linkonce.t.*)
        *hal_spi_flash.o(.text .text.* .rotext .stub .gnu.linkonce.t.*)
        *(.bootramtext)
        *drv_flsh.o(.sramtext)
        *hal_ebc.o(.sramtext)
        *hal_flash.o(.sramtext)
        *hal_spi_flash.o(.sramtext)
        *(.bootsramtext)
        . = ALIGN(0x10);
    }

    _boot_sys_sram_txt_end = .;
    _boot_sys_sram_txt_size = _boot_sys_sram_txt_end - _boot_sys_sram_txt_start;

    _boot_sys_sram_data_start = .;
#ifndef PRELOADED_INT_SRAM
    .bootsramdata . : AT (_boot_sys_sram_flash_start_location + _boot_sys_sram_txt_size)
#else
    .bootsramdata . : AT (ADDR(.bootsramdata))
#endif
    {
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
        *hal_spi_flash.o(.rdata .rodata .rodata.* .gnu.linkonce.r.* .data .data.* .gnu.linkonce.d.* )
        *(.bootdata)
        *drv_flsh.o(.sramdata)
        *drv_flsh_map.o(.sramdata)
        *hal_ebc.o(.sramdata)
        *hal_flash.o(.sramdata)
        *hal_spi_flash.o(.sramdata)
        *(.bootsramdata)
        . = ALIGN(0x10);
    }
    _boot_sys_sram_txt_end = .;
    _boot_sys_sram_txt_size = _boot_sys_sram_txt_end - _boot_sys_sram_txt_start;

    _boot_sys_sram_end = .;
    _boot_sys_sram_size = _boot_sys_sram_end - _boot_sys_sram_start;

#ifndef PRELOADED_INT_SRAM
    _boot_sys_sram_flash_end_location = _boot_sys_sram_flash_start_location + _boot_sys_sram_size;
#else
    _boot_sys_sram_flash_end_location = _boot_sys_sram_flash_start_location;
#endif

    /* ------------------------------------------------ */
    /* BOOTING BSS (content reset to 0 ) */
    /* ------------------------------------------------ */
    _boot_ram_bss_start = .;
    .bootbss . (NOLOAD) : AT (ADDR(.bootbss))
    {
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
        *hal_spi_flash.o(.bss .bss.* .gnu.linkonce.b.* COMMON)
        *(.bootbss)
        . = ALIGN(0x10);
    }
    _boot_ram_bss_end = .;
    _boot_ram_bss_size = _boot_ram_bss_end - _boot_ram_bss_start;

    _dualport_bss_size = _dualport_bss_end - _dualport_bss_start;
    _dualport_heap_start = _dualport_bss_end;
		_dualport_heap_size = BB_DUALPORT_SRAM_SIZE - _dualport_bss_size;
		
    LD_ASSERT(ABSOLUTE(BB_DUALPORT_SRAM_BASE + BB_DUALPORT_SRAM_SIZE) >= ABSOLUTE(_dualport_bss_end), "DPOV", "DUALPORT SRAM overflow.")

    /* ------------------------------------------------ */
    /* NON-BOOTING TEXT AND DATA SECTIONS IN SYS SRAM */
    /* ------------------------------------------------ */
    _sys_sram_flash_start_location = _boot_sys_sram_flash_end_location;

#if CHIP_BTOVERLAY_SUPPORTED == 1
	. = _bss_overlay_end;
#endif

    _sys_sram_start = .;
    _sys_sram_cached_start = .;

	_sys_sram_txt_start = .;
	
#ifndef PRELOADED_INT_SRAM
    .sramtext . : AT (_sys_sram_flash_start_location)
#else
    .sramtext . : AT (ADDR(.sramtext))
#endif
    {
		
		
        *(.sramtext)
        *(.ram)
        *(.btsramtext)
        *(.btsramdata)

#ifdef RAMRUN
        _boot_sector = .;
        *(.boot_sector_start)
        *boot_sector.o(INPUT_SECTIONS())
        *boot_sector_chip.o(INPUT_SECTIONS())
        *(.boottext .bootrodata)

        _code_base = .;
        *boot_loader.o(.text.boot_loader_start)
        *boot_loader.o(INPUT_SECTIONS())
#endif /* RAMRUN */

        /*pal_gsmr.o(.text .text.* .gnu.linkonce.t.* .stub)
        *sxr_mem.o(.text .text.* .gnu.linkonce.t.* .stub)
        *sxr_jbsd.o(.text .text.* .gnu.linkonce.t.* .stub)*/

#if (CHIP_ASIC_ID != 8) /* not 8808 */
        *wifi_sha1.o(.text .text.* .rodata .rodata.*) 
#endif
#ifdef CODE_IN_RAM
        *(.text .text.* .stub .gnu.linkonce.t.*)
#endif

        . = ALIGN(0x10);
		
    }

    _sys_sram_txt_end = .;
    _sys_sram_txt_size = _sys_sram_txt_end - _sys_sram_start;


#ifndef PRELOADED_INT_SRAM
    .sramdata _sys_sram_txt_end : AT (_sys_sram_flash_start_location + _sys_sram_txt_size )
#else
    .sramdata _sys_sram_txt_end : AT (ADDR(.sramdata))
#endif
    {
        _sys_sram_data_start = .;
        *(.sramdata)

        *pal_gsmr.o(.data .data.* .gnu.linkonce.d.* .data1)
        *pal_gsmr.o(.sdata .sdata.* .gnu.linkonce.s.*)
        *pal_gsmr.o(.sdata2 .sdata2.* .gnu.linkonce.s2.*)

        *(.srrodata)

#ifdef CONST_IN_RAM
        *(.rdata)
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.rodata1)
        *(.ctors)
        . = ALIGN(0x10);
#endif

        /* NOTE: globalvars.c sections are to be removed.
         * They are kept here for backward compatibility of SVC/VPP codes.
         * These codes are linked from Total_BJ branch and the special global
         * variable sections will be removed in the future. */
        *(*.globalvars.c *.globalvars.c.*)

        *(EXCLUDE_JAVA_OBJ EXCLUDE_FILE(*res.bin.o) .data)
        *(EXCLUDE_JAVA_OBJ EXCLUDE_FILE(*res.bin.o) .data.*)
        *(EXCLUDE_JAVA_OBJ .gnu.linkonce.d.*)
        *(.data1)
        _gp = ALIGN(16) + 0x7ff0;
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
        *(.lit8)
        *(.lit4)
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

#ifndef PRELOADED_INT_SRAM
    .sramucdata . : AT ( _sys_sram_flash_start_location + _sys_sram_cached_size )
#else
    .sramucdata . : AT (ADDR(.sramucdata))
#endif
    {
        _sys_sram_uncached_start = .;
        *(.sramucdata)
        *(.srroucdata)
        *(.ucdata)
        *(*.globalvars.uc)
        . = ALIGN(0x10);
    }
    _sys_sram_uncached_end = .;
    _sys_sram_uncached_size = _sys_sram_uncached_end - _sys_sram_uncached_start;
  
    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    _sys_sram_end =  .;
    _sys_sram_size = _sys_sram_end - _sys_sram_start;

#ifndef PRELOADED_INT_SRAM
    _sys_sram_flash_end_location = _sys_sram_flash_start_location + _sys_sram_size;
#else
    _sys_sram_flash_end_location = _sys_sram_flash_start_location;
#endif


#if (RAM_SIZE==0)
    /*----------------------- */
    /* SYS SRAM BSS           */
    /*----------------------- */
	/*. = 0x81c10000;    5855_TODO: remove after metal change */

    _sys_sram_bss_start = .;

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    .sramucbss . (NOLOAD) : AT (ADDR(.sramucbss))
    {
#if (CHIP_ASIC_ID < 14) /* CHIP_ASIC_ID_5855 */
        *(.bt_ucbss)
#endif
        *(.dualucbss)
        *(.bt_ucbss)
        *(.sramucbss)
        *(.ramucbss)
        *(.ucbss)
#if CHIP_BTOVERLAY_SUPPORTED == 0
        *BTSco.o(COMMON)
        *overlay.o(.pcm_big_datamem)
#endif
        . = ALIGN(0x10);
    }

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;

    .srambss . (NOLOAD) : AT (ADDR(.srambss))
    {
        *(.srambss)
        *(.unoverlaybss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(EXCLUDE_FILE(ime_*.o) .bss.* .bss  .gnu.linkonce.b.*)
        *(EXCLUDE_FILE(ime_*.o) COMMON)
        *(.rambss)
        . = ALIGN(0x10);
    }

    _sys_sram_bss_end = .;
    _sys_sram_bss_size = _sys_sram_bss_end - _sys_sram_bss_start;
#endif /* (RAM_SIZE==0) */

    /*-------------------------------------------------------  */
    /* Uninitialized section in RAM (content kept after reboot)*/
    /*-------------------------------------------------------  */
    .backup . (NOLOAD) : AT (ADDR(.backup))
    {
        *(.backup)
        . = ALIGN(0x10);
    }

    _sys_sram_end = .;
    _sys_sram_top = SYS_SRAM_END; /* End of internal sram */
#if TFLASH_IMAGE_SUPPORT==1
		_sys_sram_available_end = _tflash_sys_sram_start;
#else
		/* for boot load build */
		
		_sys_sram_data_start = .;
		_tflash_sys_sram_end = _sys_sram_data_start;
		_sys_sram_tflash_start_location = _sys_sram_flash_end_location;
		_sys_sram_tflash_end_location = _sys_sram_tflash_start_location;
		
		_tflash_sram_bss_start = .;
		_tflash_sram_bss_end = _tflash_sram_bss_start;
     _sys_sram_available_end = _sys_sram_top - (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (XCPU_IRQ_STACK_SIZE);
#endif
		
    /*----------------------- */
    /* SYS SRAM HEAP          */
    /*----------------------- */    
    /* heap in sys sram */
    _sys_sram_heap_start = .;
    _sys_sram_heap_size = _sys_sram_available_end - _sys_sram_heap_start;

    /* Check that the INT SRAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_sys_sram_available_end) > ABSOLUTE(_sys_sram_end), "INTS", "Internal SRAM overflow !")

    /*----------------------- */
    /* SYS SRAM IRQ STACK WATCH */
    /*----------------------- */
    _sys_sram_irq_stackwatch_size = (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (PS_STACK_SIZE) ;
    _sys_sram_irq_stackwatch_start = _sys_sram_top - (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (XCPU_IRQ_STACK_SIZE);
    _sys_sram_irq_stackwatch_end = _sys_sram_irq_stackwatch_start + _sys_sram_irq_stackwatch_size;

#ifdef PAGE_SPY_LOG
    _sys_sram_sp_stack_start = _sys_sram_irq_stackwatch_end;
    _sys_sram_sp_stack_end = . + PS_STACK_SIZE - 4;
#endif

    /* Caution, _sys_sram_non_romed_top is used by the boot ROM code. */
    .sram_romed_top (SYS_SRAM_END) (NOLOAD) : AT (ADDR(.sram_romed_top))
    {
        _sys_sram_non_romed_top = .;
    }

    /*---------------- */
    /* End of SYS SRAM */
    /*---------------- */

#if (CHIP_HAS_PROXY_MEMORY == 1)
    /* ========================================== */
    /* Proxy Memory                               */
    /* ========================================== */
    /* Standard usage of proxy memory */
    /* Manual placing and copy from flash */
    _proxy_memory_flash_start_location = _sys_sram_flash_end_location;
#ifndef PRELOADED_PROXY_MEMORY
    .proxy_memory SYS_PROXY_MEM_BASE : AT (_proxy_memory_flash_start_location)
#else
    .proxy_memory SYS_PROXY_MEM_BASE : AT (ADDR(.proxy_memory))
#endif
    {
        _proxy_memory_start = .;
        /* Loadable as a ramrun since the proxy memory is enabled
         * by the romed monitor. */ 
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

    /* Overlay in Proxy Memory */
    _fast_cpu_memory_overlay_start = .;
    OVERLAY . : NOCROSSREFS AT (_proxy_memory_flash_end_location)
    {
        #include "fast_cpu_memory_overlay.ld"
    }

    /* ld-2.15 End of overlay bug workaround (Manual calculation)*/
    _fast_cpu_memory_overlay_end           = _fast_cpu_memory_overlay_start + MAX_SIZE_OF_FAST_CPU_MEMORY_OVERLAY;

    . = _fast_cpu_memory_overlay_end;
    
    _fast_cpu_memory_overlay_flash_size    = FLASH_SIZE_OF_FAST_CPU_MEMORY_OVERLAY;
    _fast_cpu_memory_overlay_flash_end_location = _proxy_memory_flash_end_location + _fast_cpu_memory_overlay_flash_size;

    _proxy_memory_top = SYS_PROXY_MEM_BASE + SYS_PROXY_MEM_SIZE - 4;
    _proxy_memory_overlay_top = _proxy_memory_top - XCPU_IRQ_STACK_SIZE;
    LD_ASSERT(ABSOLUTE(_fast_cpu_memory_overlay_end) < ABSOLUTE(_proxy_memory_overlay_top), "SPMO", "System Proxy Memory Overlfow !!!")

    /* ========================================== */
    /* End of Proxy Memory                        */
    /* ========================================== */
#else /* CHIP_HAS_PROXY_MEMORY */
    _fast_cpu_memory_overlay_flash_end_location = _sys_sram_flash_end_location;
#endif /* CHIP_HAS_PROXY_MEMORY */


    /* ========================================== */
    /* in External RAM                            */
    /* ========================================== */

    . = EXT_RAM_BASE;

#ifdef SPI_REG_DEBUG

#if (CHIP_ASIC_ID == 6 || CHIP_ASIC_ID == 8) /* GALLITE or 8808 */
#define ABB_REG_SIZE 0x100
#define PMU_REG_SIZE 0x100
#else /* 8809 or later */
#define ABB_REG_SIZE 0x140
#define PMU_REG_SIZE 0x140
#endif

#if (CHIP_ASIC_ID == 6) /* GALLITE */
#define FM_REG_SIZE  0x100
#else /* 8808 or later */
#define FM_REG_SIZE  0
#endif

#if (CHIP_ASIC_ID == 6) /* GALLITE */
#if defined(GALLITE_IS_8805)
#define XCV_REG_SIZE 0x300
#else /* 8806 */
#define XCV_REG_SIZE 0x400
#endif
#else /* 8808 or later */
#define XCV_REG_SIZE 0x500
#endif

#ifdef LINK_BCPU_IMAGE

    /* Just leave a hole for spi reg debug */
    . += XCV_REG_SIZE + ABB_REG_SIZE + FM_REG_SIZE + PMU_REG_SIZE;

#else

    /* Go Uncached */
    . += MEM_CACHED_UNCACHED_OFFSET;

    _ram_spi_reg_debug_uncached_start = .;

    .spi_reg_debug (_ram_spi_reg_debug_uncached_start) (NOLOAD) :
        AT (ADDR(.spi_reg_debug))
    {
        _spi_reg_debug_start = .;
        *(.xcv_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE;
        *(.abb_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE + ABB_REG_SIZE;
#if (FM_REG_SIZE > 0)
        *(.fm_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE + ABB_REG_SIZE + FM_REG_SIZE;
#endif
        *(.pmu_reg_value)
        . = _spi_reg_debug_start + XCV_REG_SIZE + ABB_REG_SIZE + FM_REG_SIZE + PMU_REG_SIZE;
    }

    _ram_spi_reg_debug_uncached_end = .;
    _ram_spi_reg_debug_uncached_size = _ram_spi_reg_debug_uncached_end - _ram_spi_reg_debug_uncached_start;

    /* Go Cached */
    . -= MEM_CACHED_UNCACHED_OFFSET;
    
#endif

#endif

#if (RAM_SIZE>0)

    /*----------------------- */
    /* SYS SRAM BSS           */
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
        *(.unoverlaybss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(EXCLUDE_FILE(ime_*.o) .bss.* .bss  .gnu.linkonce.b.*)
        *(EXCLUDE_FILE(ime_*.o) COMMON)
        *(.rambss)
        . = ALIGN(0x10);
    }

    _sys_sram_bss_end = .;
    _sys_sram_bss_size = _sys_sram_bss_end - _sys_sram_bss_start;
    
#ifdef BTCORE_IN_RAM

		_bt_sram_flash_start_location = _fast_cpu_memory_overlay_flash_end_location;
		_bt_sram_start = .;
    .btsramtext . : AT (_bt_sram_flash_start_location)
		{
			  #include "btcore_code_content"
        . = ALIGN(0x10);
		}
		_bt_sram_end = .;
		_bt_sram_size = _bt_sram_end - _bt_sram_start;
		
		_bt_sram_flash_end_location = _bt_sram_flash_start_location + _bt_sram_size;
#endif


#endif //(RAM_SIZE>0)


    _ram_available_base = .;

		_ram_bss_start = .;

    .rambss . (NOLOAD) : AT (ADDR(.rambss))
    {
        ime_*.o(.bss*)
        ime_*.o(COMMON)
        . = ALIGN(0x10);
    }

		_ram_bss_end = .;
		
		_heap_start = .;
		
#if (RAM_SIZE>0)
    /* heap in external sram */
    _sram_top = EXT_RAM_BASE + RAM_SIZE; /* End of external sram */
    _heap_size = _sram_top - _heap_start;

    /*----------- */
    /* End of RAM */
    /*----------- */

    /* Check that the external RAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_sram_top) > ABSOLUTE(_ram_bss_end), "EXTS", "External RAM overflow !")
#else
		_heap_size = 0;
#endif

    /*========================================================= */
    /* In FLASH                                                 */
    /*========================================================= */
#if (RAM_SIZE>0)
#ifdef BTCORE_IN_RAM
		_rom_flash_start_location = _bt_sram_flash_end_location;
#else
    _rom_flash_start_location = _fast_cpu_memory_overlay_flash_end_location;
#endif
#else
	_rom_flash_start_location = _fast_cpu_memory_overlay_flash_end_location;
#endif
    /* .crossrefs section can be used to list functions which is share by other application */
    .crossrefs _rom_flash_start_location : AT (ADDR(.crossrefs))
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

    .rotext . : AT (ADDR(.rotext))
    {
#ifndef CODE_IN_RAM
        *(.text)
        *(.text.*)
        *(.stub .gnu.linkonce.t.*)
#endif
        . = ALIGN(0x20);
    }

    .rodata . : AT (ADDR(.rodata))
    {
#ifndef CONST_IN_RAM
        *(.rdata)
        *(.rodata)
        *(.rodata.*)
        *(.rodata.*_png)
        *(.gnu.linkonce.r.*) 
        *(.rodata1)
        *(.roresdata)
        *(.robsdata)
#if (EXTRA_CODE_SIZE == 0)
        *(.extra)
#endif
#endif
        . = ALIGN(0x10);
    }


    /*=================================================== */
    /* Placeholder for misc hacks, not used for real apps */
    /*=================================================== */
    .misc . : AT (ADDR(.misc))
    {
        *(.misc)
        . = ALIGN(0x10);
        *(.build_date)
    }
    
		. = ALIGN(0x1000);

    _flash_end = .;

    _res_data_flash_start_location = .;
    
    .res . : AT (ADDR(.res))
    {
    	KEEP(*res.bin.o(.data .data.*))
    }
    _res_data_end_location = .;

    /*=================================================== */
    /* Codes to fill the hole between user sector and     */
    /* calib sector                                       */
    /*=================================================== */
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

    LD_ASSERT(ABSOLUTE(extra_rores_size) > ABSOLUTE(_extra_section_size), "FLAS", "Extra section overflow: Please check EXTRA_RORES_SIZE in memd.def and the .extra section size.")
    extra_remain_size = extra_rores_size - _extra_section_size;
#endif


    /*================================================= */
    /* Define user data boundaries based on the defines */
    /*================================================= */
    .user_data USER_DATA_BASE : AT (ADDR(.user_data))
    {
        _user_data_start = .;
#if !(defined(COMPRESS_PROCESS_ENABLE) && !defined(LINK_STAGE_2))
        *user.bin.o(.user_data);
        KEEP(*(.user_data))
#endif
    }    

    _user_data_end = .;
    _user_data_size = _user_data_end - _user_data_start;

    /*Check that the data and code in flash do not overwrite
     * the user data space */
    /* ASSERT seems to be boguous as of binutils-2.15 : we are
     * not allowed to used error messages longer than FLAS in that
     * particular case. Don't ask and port a newer version,
     * maybe. FIXME */
    LD_ASSERT(ABSOLUTE(_user_data_start) > ABSOLUTE(_res_data_end_location),
              "FLAS",
              "Flash overflow: there is no more space available in flash memory.")
#ifdef REDUNDANT_DATA_REGION

    /*================================================= */
    /* Define remain data boundaries based on the defines */
    /*================================================= */
    
    .remain_data REMAIN_BASE : AT (ADDR(.remain_data))
    {
        _remain_start = .;      
        KEEP(*(.remain_data))
    }       
    _remain_end = _remain_start + REMAIN_SIZE;

#endif
    /* ============================== */
    /* calib sector in flash          */
    /* ============================== */

    .calib CALIB_BASE : AT (ADDR(.calib))
    {
        _calib_start = .;
        *(.calib);
        . = ALIGN(0x10);
    }
    _calib_end = .;
    _calib_size = _calib_end - _calib_start;
#ifdef REDUNDANT_DATA_REGION

    LD_ASSERT(ABSOLUTE(_calib_start) >= ABSOLUTE (_remain_end),
              "FLAS",
              "Remain data is overflow.")
    
     LD_ASSERT(ABSOLUTE(_remain_end - _remain_start) >= 0x1000,
              "FLAS",
              "Remain sector is null.")

#endif
    /* ============================== */
    /* factory settings in flash      */
    /* ============================== */

    .factory_settings FACT_SETTINGS_BASE : AT (ADDR(.factory_settings))
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
    LD_ASSERT(ABSOLUTE(_calib_start) >= ABSOLUTE(_user_data_end) ||
              ABSOLUTE(_factory_start) > ABSOLUTE(_code_base),
              "CFOV", "Calib data is overlapped with user data or factory data is overlapped with code.")

    /*Check that the calib data in flash do not overwrite
     * the factory setting data space */
    LD_ASSERT(ABSOLUTE(_factory_start) > ABSOLUTE(_calib_start),
              "FACO", "Factory setting data is overlapped with calib data.")

    /*#include "discard.ld"*/
}

#ifdef COMPRESS_DATA_SECTION_NOCROSSREFS
COMPRESS_DATA_SECTION_NOCROSSREFS
#endif

