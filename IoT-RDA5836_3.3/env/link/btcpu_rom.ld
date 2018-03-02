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


#define BSS()  .bss .bss.* COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.* .rambss .gnu.linkonce.b.*

#define DATA()  .data .data.*

#ifdef CHIP_HAS_NOCACHE
#define MEM_CACHED_UNCACHED_OFFSET 0x00000000
#else
#define MEM_CACHED_UNCACHED_OFFSET 0x20000000
#endif


#define RODATA() .rdata .rodata .rodata.str1.4 .rodata.*
#define ROTEXT() .text .text.*
#define INPUT_SECTIONS() ROTEXT() RODATA()



#ifdef CHIP_HAS_BTCPU
EXTERN (btcpu_error_code btcpu_error_status btcpu_sp_context
        btcpu_stack_base btcpu_main_entry btcpu_int_entry)
EXTERN (__btcpu_start __btcpu_int_handler)
/*EXTERN(SYSirq_Handle) */
EXTERN(LSLCirq_BT_AUX_TIM_Handler LSLCirq_ARM_COMMOM_INT_Handler \
				LSLCirq_BT_PKD_RX_HDR_Handler LSLCirq_BT_PKD_Handler LSLCirq_BT_TIM1_Handler LSLCirq_BT_NO_PKT_RCVD_Handler LSLCirq_BT_PKA_Handler LSLCirq_BT_TIM3_Handler LSLCirq_BT_SYNC_DET_Handler \
				LSLCirq_BT_TIM2_Handler LSLCirq_BT_TIM0_Handler LM_SecEng_Generate_Kc_Prime LM_SecEng_Generate_Number  )

/*EXTERN(  LM_SecEng_Authenticate_E1 LM_SecEng_Generate_Key_E21 LM_SecEng_Generate_Key_E22 LM_SecEng_Generate_Key_E3\
				 LM_SecEng_Generate_DHKey LM_SecEng_Generate_Commitment \
				 LM_SecEng_Generate_Link_Key LM_SecEng_Generate_Check)*/

EXTERN(BTq_BB_Dequeue_Next BTq_BB_Dequeue_Next_Synchronous )

#endif

/* bt host sdp and rfcomm */
/*EXTERN (RDABT_Execute rdabt_sdp_cli_msg_dispatch rdabt_sdp_srv_msg_dispatch rdabt_rfcomm_msg_dispatch)*/


SECTIONS
{
    /* ========================================================= */
    /* INT ROM                                                   */
    /* ========================================================= */

    . = BT_ROM_BASE;
    .btcpu_rom . : AT(ADDR(.btcpu_rom))
    {
        /* Specifies a section for the functions to put
           in ROM. */
 				*btcpu_rom.o(INPUT_SECTIONS())
				*lslc_irq.o(INPUT_SECTIONS())
			  *lslc_pkt.o(INPUT_SECTIONS())
				*lslc_slot.o(INPUT_SECTIONS())
        *(INPUT_SECTIONS())                                       	
        *(.boottext)
        *(.sramtext)
        *(share)
				KEEP(*(share))
        . = ALIGN(0x10);
    }
    
   _btrom_data_base = .;


    /* ================================================= */
    /* in DUALPORT SRAM                                  */
    /* ================================================= */
#if (NAND_FLASH == 1)
    . = DUALPORT_SRAM_BASE + NAND_FLASH_BOOT_CODE_SIZE;
#else
    . = DUALPORT_SRAM_BASE;
#endif

		
    .dualport_sram_bss . (NOLOAD) : AT (ADDR(.dualport_sram_bss))
    {
    		*(.dualucbss)
    		*tra_queue.o(.bss .bss.*)
    
        . = ALIGN(0x10);
    }
    _rom_dualport_bss_end = .;

	 . = BT_SRAM_BASE;
	 
	 _btrom_irq_start = .;
	 
	 . += 0x180; /* reserved for irq handler */

    _btrom_data_start = .;
    .bt_rom_inter_data : AT (_btrom_data_base)
    {
			  *(.bt_inter_ram_data)
        *(.bootsramdata)
        *(.data)
        *(DATA())
        *(trap)
        KEEP(*(trap))
        . = ALIGN(0x10);
    }
  	_btrom_data_ram_end = .;
  	_btrom_data_ram_size = (_btrom_data_ram_end - _btrom_data_start);

		_btrom_data_end = _btrom_data_base + _btrom_data_ram_size;

   _btrom_bss_start = .; 

    . += MEM_CACHED_UNCACHED_OFFSET;

    .bt_rom_uncached . (NOLOAD) : AT(ADDR(.bt_rom_uncached))
    {
				*(.bt_ucbss)
				*(.ucbss)
				*sys_config.o(BSS())
        . = ALIGN(0x4);
    }

   . -= MEM_CACHED_UNCACHED_OFFSET;	
	
    .bt_rom_inter_ram (NOLOAD) : AT (ADDR(.bt_rom_inter_ram))
    {

        *(BSS())
        *(.bt_inter_ram)
        . = ALIGN(0x10);
    }
  	_btrom_bss_end = .;
   
		_btrom_sram_end = _btrom_bss_end;

    /* BTSRAM overflow */
    LD_ASSERT(ABSOLUTE(_btrom_data_ram_end) <= ABSOLUTE(BT_SRAM_BASE + BT_SRAM_SIZE), "BTSO", "BTSRAM Overflozed !!") 


    /* ========================================================= */
    /* BT INT SRAM                                                  */
    /* ========================================================= */
    .bt_sram_romed_top (BT_SRAM_BASE + BT_SRAM_SIZE - 0x10) : AT(ADDR(.bt_sram_romed_top))
    {
        _bt_sram_non_romed_top = .;
    }
    
    /* ========================================================= */
    /* Back in the INT ROM                                       */
    /* ========================================================= */
		_bt_rom_end = BT_ROM_BASE + BT_ROM_SIZE - 4;
    .bt_boot_rom_version_number (_bt_rom_end - 4) : AT(ADDR(.bt_boot_rom_version_number))
    {
        _bt_int_rom_version_number = .;
        *(.boot_rom_version_number)
    }

    ._bt_internal_rom.crc _bt_rom_end : AT(ADDR(.bt_internal_rom.crc))
    {
        _bt_int_rom_crc = . ;
        LONG(0xc001c001)
    }

		_btflash_start = FLASH_BASE + 0x40;

    #include "discard.ld"
}

