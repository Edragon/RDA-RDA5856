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
/*  - PRELOADED_RAM : The external RAm is already preloaded, useful for ram_run or simu */

/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif



#define BSS()  .bss .bss.* COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.* .rambss .gnu.linkonce.b.* .sdata.*

#define DATA()  .data .data.*

#ifdef CHIP_HAS_NOCACHE
#define MEM_CACHED_UNCACHED_OFFSET 0x00000000
#else
#define MEM_CACHED_UNCACHED_OFFSET 0x20000000
#endif

#define RODATA() .rdata .rodata .rodata.str1.4 .rodata.* .srodata.*
#define ROTEXT() .text .text.*
#define INPUT_SECTIONS() ROTEXT() RODATA()

EXTERN (__xcpu_start)

EXTERN (boot_romCsString boot_Monitor boot_IspiOpen g_bootRomVersionNumber)

EXTERN (_boot_IrqHandler)

EXTERN (boot_UsbOpen
        boot_UsbClose
        boot_UsbReset
        boot_UsbRecv
        boot_UsbSend
        boot_UsbEpStall
        boot_UsbCompletedCommand
        boot_UsbEpEnableInterrupt
        boot_UsbEpRxTransferedSize
        boot_UsbEpTransfertDone
        boot_UsbIrqHandler
        )
/*musb_start*/
/*EXTERN (boot_HostUsbOpen
        boot_HostUsbClose
        boot_HostUsbSend
        boot_HostUsbRecv)*/
        

EXTERN (btll_rom_main)

/*EXTERN(BTq_BB_Dequeue_Next BTq_BB_Dequeue_Next_Synchronous )*/


/* sxr task and event functions */
/*EXTERN (sxr_Wait 
		sxr_Send 
		sxr_RunNextTask 
		sxr_TaskSwap
		sxr_Tampax 
		sxr_SetTimer
		sxr_SuspendTask
		sxs_StartTimer
		sxs_StopTimer
		sxr_EnterScSchedule
		sxr_StartTask
		sxr_Sleep)*/

/*EXTERN (FS_Read FS_Write) */

/*EXTERN (memd_FlashErase memd_FlashWrite hal_FlashCacheOn hal_FlashCacheOff)*/

/*EXTERN ( __fixsfsi
         __mulsf3
      __divsf3
        __divdf3 
		__divdi3 
		__udivdi3 
		__muldf3 
		__fixunsdfsi 
		__subsf3 
		__addsf3 
		__floatsisf 
		 )*/
/* EXTERN (__divsf3 __divdf3 __divdi3 __udivdi3 __mulsf3 __muldf3)  __muldf3  __fixunsdfsi __subsf3 __addsf3 __mulsf3  __floatsisf __fixsfsi */

/* EXTERN(hal_TimDelay   hal_TimGetUpTime sprintf)*/


SECTIONS
{
    /* ========================================================= */
    /* INT ROM                                                   */
    /* ========================================================= */
    . = INT_ROM_BASE;

    __int_rom_main = .;
    _int_rom_start = .;

    .internal_rom . : AT(ADDR(.internal_rom))
    {
        /* Specifies a section for the functions to put
           in ROM. */
 				*boot_rom.o(INPUT_SECTIONS())
				*boot*(INPUT_SECTIONS())
				*(.boottext)
				. += 0x10; 
				. = ALIGN(0x10);

				*(.sramtext)
			
				*sxr_tls.o(INPUT_SECTIONS())
				*sxr_sbx.o(INPUT_SECTIONS())
				/*sxs_io.o(INPUT_SECTIONS())*/
				*sxr_tim.o(INPUT_SECTIONS())
				*sxr_tksd.o(INPUT_SECTIONS())
				*sxr*(INPUT_SECTIONS())  
				*sxs*(INPUT_SECTIONS()) 
				*drv_flsh.o(INPUT_SECTIONS())
				*hal_spi_flash.o(INPUT_SECTIONS())
				*hal_timer.o(INPUT_SECTIONS())
				*hal_utils_mips.o(INPUT_SECTIONS())
				*hal_timer.o(INPUT_SECTIONS())
				*hal_sxr_mips.o(INPUT_SECTIONS())
				*hal_host.o(INPUT_SECTIONS())
				*target_config.o(INPUT_SECTIONS())
				*memcpy.o(INPUT_SECTIONS())
				*memset.o(INPUT_SECTIONS())
				*memcmp.o(INPUT_SECTIONS())
				*memmove.o(INPUT_SECTIONS())
				*strlen.o(INPUT_SECTIONS())				
				*lslc_irq.o(INPUT_SECTIONS())
			  *lslc_pkt.o(INPUT_SECTIONS())
				*lslc_slot.o(INPUT_SECTIONS())
        *(INPUT_SECTIONS())  
		*(share)
          KEEP(*(.FAST_ACCESS_SECTION*))
          KEEP(*(.PH_TABLE*))
          KEEP(*(share))

        . = ALIGN(0x10);
    }


   .sys_rom . : AT(ADDR(.sys_rom))
    {	
        *(INPUT_SECTIONS())                                       	
        *(.boottext)
        *(.sramtext)
    }
 
   _sys_rom_data_base = .;

		. = SYS_SRAM_BASE;
		.rom_usb_bss . (NOLOAD) : AT(ADDR(.rom_usb_bss))
    {
				*boot_usb.o(.bss.*)
    }

    . = ROM_MISC_BASE + MEM_CACHED_UNCACHED_OFFSET;

    .rom_entries_uncached . (NOLOAD) : AT(ADDR(.rom_entries_uncached))
    {
        #include "bthost_rom_reserved_uncached_ram"
        . = ALIGN(0x4);
    }

    . -= MEM_CACHED_UNCACHED_OFFSET;

    .rom_entries_cached . (NOLOAD) : AT(ADDR(.rom_entries_cached))
    {
        #include "5855_internal_rom_reserved_ram"
    }

    .rom_inter_ram (NOLOAD) : AT (ADDR(.rom_inter_ram))
    {
        _sys_rom_bss_start = .;
        *hal_*.o(BSS())
        *boot_*.o(BSS())
        . = ALIGN(0x20);
    }
  	_sys_rom_bss_end = .;

    _sys_rom_data_start = .;
    .rom_inter_data : AT (_sys_rom_data_base)
    {
        *(.data)
        *(.bootsramdata)
        *(DATA())
        . = ALIGN(0x10);
    }
  	_sys_rom_data_ram_end = .;
  	_sys_rom_data_ram_size = (_sys_rom_data_ram_end - _sys_rom_data_start);

		_sys_rom_data_end = _sys_rom_data_base + _sys_rom_data_ram_size;
		
    __int_rom_reserved_end = . ;

    /* Reserved Ram for Rom overflow */
    LD_ASSERT(ABSOLUTE(__int_rom_reserved_end) <= ROM_MISC_BASE + ROM_MISC_SIZE, "RRRO", "Reserved Ram Reserved for Rom Overflowed")


		. = SYS_SRAM_BASE + IRQ_RAM_SIZE;
    _sys_rom_btoverly_start = .;
    .rom_btoverly (NOLOAD) : AT (ADDR(.rom_btoverly))
    {
        *(BSS())
        . = ALIGN(0x10);
    }
    
    _sys_rom_btoverly_end = .;

    /* ========================================================= */
    /* INT SRAM                                                  */
    /* ========================================================= */
    .sram_romed_top (SYS_SRAM_BASE + SYS_SRAM_SIZE - 4) : AT(ADDR(.sram_romed_top))
    {
        _sys_sram_non_romed_top = .;
    }
    
    /* Reserved Ram for Rom overflow */
    LD_ASSERT(ABSOLUTE(_sys_rom_bss_end) <= _sys_sram_non_romed_top, "RRRO", "Reserved Ram Reserved for Rom Overflowed")
    
    /* ========================================================= */
    /* Back in the INT ROM                                       */
    /* ========================================================= */
		_int_rom_end = INT_ROM_BASE + INT_ROM_SIZE - 4;
    .boot_rom_version_number (_int_rom_end - 4) : AT(ADDR(.boot_rom_version_number))
    {
        _int_rom_version_number = .;
        *(.boot_rom_version_number)
    }

    .internal_rom.crc _int_rom_end : AT(ADDR(.internal_rom.crc))
    {
        __int_rom_crc = . ;
        LONG(0xc001c001)
    }

    #include "riscv_discard.ld"
}
