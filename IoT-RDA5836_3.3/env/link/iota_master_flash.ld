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


/*Mandatory to have the BB side irq handler linked */
ENTRY (_flash_btcpu_start)
EXTERN (__bb_int_handler)
EXTERN (_sxr_Irq_handler)
EXTERN (boot_LoaderEnter)
EXTERN (boot_Sector)
EXTERN (boot_Sector_Nand)


SECTIONS
{
    . = FLASH_BASE;

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
    #include "link_btcpu_elf.ld"

    /* ======================================== */
    /* XCPU flash elf */
    /* ======================================== */
    #include "link_xcpu_elf.ld"

    /* ======================================== */
    /* XCPU ROM elf */
    /* ======================================== */
    #include "link_rom_elf.ld"

    /* ======================================== */
    /* BTCPU ROM elf  */
    /* ======================================== */
    #include "link_btcpu_rom_elf.ld"
  
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
    LD_ASSERT(ABSOLUTE(_user_data_start) > ABSOLUTE(_res_data_end),
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
    LD_ASSERT(ABSOLUTE(_factory_start) >= ABSOLUTE(_calib_start),
              "FACO", "Factory setting data is overlapped with calib data.")

    #include "discard.ld"
}

#ifdef COMPRESS_DATA_SECTION_NOCROSSREFS
COMPRESS_DATA_SECTION_NOCROSSREFS
#endif

