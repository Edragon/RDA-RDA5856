/* 
 * Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

////////////////////////////////////////////////////////////////////////////////
//
/// @file boot_sector.c
///
/// Boot sector.
/// 
/// The code of the Boot Sector must be mapped in the first sector of the
/// Flash and should only use internal RAM (as the external SRAM might not
/// be usable in case of burst mode, after a reset).
//
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "string.h"

#include "chip_id.h"
#include "global_macros.h"
#include "cfg_regs.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "mem_bridge.h"
#include "debug_uart.h"
#include "debug_host.h"
#include "keypad.h"
#include "key_defs.h"
#undef KP_CTRL
#include "timer.h"

#include "bootp_host.h"
#include "bootp_loader.h"
#include "bootp_sector.h"
#include "boot_host.h"
#include "boot_sector.h"
#include "boot.h"

#include "hal_sys.h"
#include "hal_ebc.h"
#include "hal_mem_map.h"

#include "memd_m.h"
#if defined(TGT_WITH_TS_MODEL_rda1207)
#include "rda1207.h"
#endif /* TGT_WITH_TS_MODEL_rda1207 */
#include "hal_ispi.h"
#include "boot_ispi.h"

#include "tgt_m.h"
#include "tgt_memd_cfg.h"
#include "hal_spi_flash.h"
#include "register.h"


// =============================================================================
// _boot_sys_sram
// -----------------------------------------------------------------------------
/// Addresses of the sections in system internal sram for booting process
// =============================================================================
extern UINT32 _boot_sys_sram_flash_start_location;
extern UINT32 _boot_sys_sram_flash_end_location;
extern UINT32 _boot_sys_sram_start;

extern UINT32 _sys_rom_data_base;
extern UINT32 _sys_rom_data_end;
extern UINT32 _sys_rom_data_start;
extern UINT32 _sys_rom_bss_end;
extern UINT32 _sys_rom_bss_start;

// =============================================================================
// _boot_ram
// -----------------------------------------------------------------------------
/// Addresses of the sections in external ram for booting process
// =============================================================================
extern UINT32 _boot_ram_flash_start_location;
extern UINT32 _boot_ram_flash_end_location;
extern UINT32 _boot_ram_start;
extern UINT32 _boot_ram_bss_start;
extern UINT32 _boot_ram_bss_end;


extern VOID hal_BootSectorSetEbcRamId(UINT8 ramId);


// =============================================================================
// MACROS                                                                       
// =============================================================================


/// Linker section where the Boot Sector main function must be mapped:
/// Beginning of the first sector of the flash.
#define BOOT_SECTOR_SECTION_START  __attribute__((section (".boot_sector_start")))


/// Linker sections where the Boot Sector structure pointer must be mapped.
/// At a fixed address in the Internal SRAM, right before the fixed pointer.
#define BOOT_SECTOR_SECTION_STRUCT_PTR __attribute__((section (".boot_sector_struct_ptr")))


/// Linker sections where the Boot Sector reload structure pointer must be mapped.
/// At a fixed address in the Internal SRAM, right before the boot sector struct pointer.
#define BOOT_SECTOR_SECTION_RELOAD_STRUCT_PTR __attribute__((section (".boot_sector_reload_struct_ptr")))

/// Linker sections where the Boot Tfalsh Upgrade structure pointer must be mapped.
/// At a fixed address in the Internal SRAM, right before the boot sector struct pointer.
#define BOOT_TFLASH_UPGRADE_STRUCT_PTR __attribute__((section (".boot_tflash_upgrade_struct_ptr")))

/// The high 16 bits of force-monitor key status events
#define BOOT_FORCE_MONITOR_KEY_EVENT_PREFIX (0xFE900000)


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


/// Boot Sector structure pointer.
/// 
/// This pointer is filled by HAL during the normal execution of the code.
/// It is used by the Boot Sector to find the Boot Sector structure in
/// Internal RAM.
/// 
/// This pointer will be also used by the host tool (E.g., by CoolWatcher to configure
/// EBC during a restart). Its location is specified in the ld file for the software
/// on the target, and in the corresponding ???_soft_root.xmd for the host tool.
/// These location definitions should be always kept consistent.
PUBLIC BOOT_SECTOR_STRUCT_T* 
    BOOT_SECTOR_SECTION_STRUCT_PTR g_bootSectorStructPtr;


/// Boot Sector reload structure.
///
/// This structure contains information written by HAL during the normal
/// execution of the code and used by the Boot Sector to reload the ram image
/// when the phone boots. 
PUBLIC BOOT_SECTOR_RELOAD_STRUCT_T* 
    BOOT_SECTOR_SECTION_RELOAD_STRUCT_PTR g_bootSectorReloadStructPtr;

PUBLIC BOOT_TFLASH_UPGRADE_STRUCT_T* 
    BOOT_TFLASH_UPGRADE_STRUCT_PTR g_tflashUpgradeStructPtr;

// Copied by the lod command of CoolWatcher. Useful for Ramruns.
PROTECTED BOOT_HST_EXECUTOR_T g_bootHstExecutorCtx
    __attribute__((section (".executor_ctx"))) =
    {{ 0, (UINT32)boot_Sector, 0, 0, 0}, {0}};

// =============================================================================
// EXPORTS                                                            
// =============================================================================


// =============================================================================
// FUNCTIONS                                                            
// =============================================================================


// =============================================================================
// boot_ForceMonitorKeyCheck
// -----------------------------------------------------------------------------
/// Enter boot monitor if all of the force-monitor keys are pressed.
// =============================================================================
INLINE VOID boot_ForceMonitorKeyCheck(VOID)
{
#if 0 // ndef RAMRUN tianwq: don't check key here, for some platrom has no key pad
    // If power-on key is pressed ...
    if (hwp_keypad->KP_STATUS & KEYPAD_KP_ON)
    {
        CONST UINT8 *monKeys = NULL;
        UINT32 listLen = 0;

        tgt_GetBootDownloadKeyList(&monKeys, &listLen);

        if (monKeys != NULL && listLen > 0)
        {
            // Assume all the key-in and key-out lines are in use
            hwp_keypad->KP_IRQ_MASK = 0;
            hwp_keypad->KP_IRQ_CLR = KEYPAD_KP_IRQ_CLR;
            hwp_keypad->KP_CTRL  = KEYPAD_KP_DBN_TIME(0)
                | KEYPAD_KP_ITV_TIME(0)
                | KEYPAD_KP_EN | KEYPAD_KP_IN_MASK(0xff)
                | KEYPAD_KP_OUT_MASK(0xff);

            // 0.3125 * keyOutLines * ( debouceTime + 1 ) =
            // 0.3125 * 8 * (0 + 1) ~= 3 ms
            #define DEBOUNCE_TIME ((3 * 2 + 1) * 16384 / 1000)

            UINT32 startTime = hwp_timer->hwtimer_curval;
            while (hwp_timer->hwtimer_curval - startTime < DEBOUNCE_TIME);

            UINT32 index;
            UINT32 key;
            BOOL down;
            BOOL forceMon = TRUE;
            for (UINT32 i=0; i<listLen; i++)
            {
                index = tgt_GetKeyMapIndex(monKeys[i]);
                if (index < LOW_KEY_NB)
                {
                    key = GET_BITFIELD(hwp_keypad->KP_DATA_L, KEYPAD_KP_DATA_L);
                    down = ((key & (1<<index)) != 0);
                }
                else
                {
                    key = GET_BITFIELD(hwp_keypad->KP_DATA_H, KEYPAD_KP_DATA_H);
                    down = ((key & (1<<(index-LOW_KEY_NB))) != 0);
                }
                mon_Event(BOOT_FORCE_MONITOR_KEY_EVENT_PREFIX|(index<<8)|down);
                if (!down)
                {
                    forceMon = FALSE;
                }
            }
            if (forceMon)
            {
                mon_Event(BOOT_SECTOR_EVENT_ENTER_MONITOR);
                boot_SectorEnterBootMonitor();
            }
        }
    }
#endif // !RAMRUN
}


// =============================================================================
// boot_BootSectorGetStructValidity
// -----------------------------------------------------------------------------
/// Check the validity of the Boot Sector structure by reading a bit in
/// the Boot Mode register and then by checking that the address contained in
/// the Boot Sector structure pointer is a valid address (in KSEG0 or in
/// KSEG(1) and not impair).
///
/// @return TRUE if valid, FALSE else.
// =============================================================================
INLINE BOOL boot_BootSectorGetStructValidity(VOID)
{
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    if (FALSE == BOOT_SECTOR_GET_STRUCT_VALIDITY())
    {
        return FALSE;
    }
#endif
    // Is the address word-aligned?
    if (((UINT32)g_bootSectorStructPtr & 0x3) != 0)
    {
        return FALSE;
    }

    // Is the address in INT-SRAM space?
    if ( ! (
             ( (UINT32)g_bootSectorStructPtr >= KSEG0(REG_INT_SRAM_BASE) &&
                  (UINT32)g_bootSectorStructPtr < KSEG0(REG_INT_SRAM_BASE)+sizeof(HWP_INT_SRAM_T) ) ||
             ( (UINT32)g_bootSectorStructPtr >= KSEG1(REG_INT_SRAM_BASE) &&
                  (UINT32)g_bootSectorStructPtr < KSEG1(REG_INT_SRAM_BASE)+sizeof(HWP_INT_SRAM_T) )
           ) )
    {
        return FALSE;
    }

    return TRUE;
}


#ifndef GALLITE_IS_8805
// =============================================================================
// boot_SpiFlashControllerInit
// -----------------------------------------------------------------------------
/// Initialize SPI flash controller if any.
// =============================================================================
INLINE VOID boot_SpiFlashControllerInit(VOID)
{
    //set the second 32/64 Mbit flash chip  enable 
    //if no the second 32/64 Mbit flash chip, it will be cleared in memd_FlashOpen function.
#ifdef __PRJ_WITH_2_32MBIT_SPIFLSH__
    *(volatile unsigned int *)0xa1a2501c = 0x01;
#endif
#ifdef __PRJ_WITH_2_64MBIT_SPIFLSH__
    *(volatile unsigned int *)0xa1a2501c = 0x03;
#endif
#ifdef __PRJ_WITH_2_128MBIT_SPIFLSH__
    *(volatile unsigned int *)0xa1a2501c = 0x08;
#endif

    //change spi flash controller frequency divider. 
#ifdef __PRJ_WITH_SPIFLSH__
    *(volatile unsigned int *)0xa1a25014 = 0x210;
#endif
}


#define ERAM_CHECK_PATTERN0  0xa569f0f0
#define ERAM_CHECK_PATTERN1  0x5a960f0f
#define ERAM_ADDR_NUM 6
#define ERAM_CHECK_ERROR_CODE 0xdead0000
#define CRAM_DIDR 0x40000

const UINT32 eram_end_addr[ERAM_ADDR_NUM] = {0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000};

// =============================================================================
// boot_ExtRamSizeCheck
// -----------------------------------------------------------------------------
/// Check whether the ram size configuration is correct, and raise
/// an exception if not.
/// The ram size is checked by ram ID or actual size.
// =============================================================================
INLINE VOID boot_ExtRamSizeCheck(VOID)
{
#if 0 // todo:
    // Using the CR controlled method 
    // Configure DIDR
    // CRAM_DIDR[15] : ROW length
    // CRAM_DIDR[14:11] : Device Version
    // CRAM_DIDR[10:8] : Device density
    // CRAM_DIDR[7:5] : CRAM Generation
    // CRAM_DIDR[4:0] : Vendor ID
    UINT16 index;
    REG16* csBase = (UINT16*)hwp_cs1;
    REG32* pCRE = &(hwp_memFifo->EBC_CRE);

    mon_Event(RAM_SIZE >> 20);

    *pCRE = 1; // MEM_FIFO_CRE    
    index = *(csBase + CRAM_DIDR); 
    UINT32 flush __attribute__((unused)) = *pCRE;    
    *pCRE = 0;

    if ((index & 0x1f) == 0xd) //aptic
    {
        index = (index >> 8) & 7;
        if (RAM_SIZE > eram_end_addr[index])
        {
            mon_Event(ERAM_CHECK_ERROR_CODE | (eram_end_addr[index] >> 20)); //EXT ramsize error
            asm("break 2");
        }
    }
    else
    {
        VOLATILE UINT32 *ERam_Base = (UINT32 *)csBase;

        UINT32 contentBackup;
        // Save the original RAM data (possibly some boot codes)
        contentBackup = *ERam_Base;

        *ERam_Base = ERAM_CHECK_PATTERN0;

        for (index = 0; index < ERAM_ADDR_NUM; index++)
        {
            if (*(VOLATILE UINT32 *)((UINT32)ERam_Base + eram_end_addr[index]) == ERAM_CHECK_PATTERN0)
            {
                *ERam_Base = ERAM_CHECK_PATTERN1;
                if (*(VOLATILE UINT32 *)((UINT32)ERam_Base + eram_end_addr[index]) == ERAM_CHECK_PATTERN1)
                {
                    if (RAM_SIZE > eram_end_addr[index]) 
                    {
                        mon_Event(ERAM_CHECK_ERROR_CODE | (eram_end_addr[index] >> 20)); //EXT ramsize error
                        asm("break 2");
                    }
                    else
                        break;
                }
            }
        }

        // Restore the original RAM data (possibly some boot codes)
        *ERam_Base = contentBackup;
    }
#endif
}


#ifdef RDA_PSRAM_ADAPTIVE

// =============================================================================
//  boot_EbcRamIdValid
// -----------------------------------------------------------------------------
/// Check whether an external RAM ID is valid.
///
/// @param ramId the RAM ID to be checked.
// =============================================================================
INLINE BOOL boot_EbcRamIdValid(UINT32 ramId)
{
    return (ramId < RDA_EXTSAM_QTY);
}


// =============================================================================
//  boot_EbcReadRamId
// -----------------------------------------------------------------------------
/// Read the RAM ID from the external RAM chipset.
// =============================================================================
INLINE UINT32 boot_EbcReadRamId(VOID)
{
    // Using the CR controlled method 
    // Configure DIDR
    // CRAM_DIDR[15] : ROW length
    // CRAM_DIDR[14:11] : Device Version
    // CRAM_DIDR[10:8] : Device density
    // CRAM_DIDR[7:5] : CRAM Generation
    // CRAM_DIDR[4:0] : Vendor ID
    UINT16 index;
    UINT32 ramId = RDA_APTIC;
    REG16* csBase = (UINT16*)hwp_cs1;
    REG32* pCRE = &(hwp_memFifo->EBC_CRE);

#ifdef RDA_PSRAM_ADAPTIVE_NOT
    return;
#endif

    mon_Event(0x66666666);

    *pCRE = 1; // MEM_FIFO_CRE    
    index = *(csBase + CRAM_DIDR); 
    UINT32 flush __attribute__((unused)) = *pCRE;    
    *pCRE = 0;

    mon_Event(index);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)

    if (hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID) == HAL_SYS_CHIP_PROD_ID_8808)
    {
        ramId = RDA_APTIC; 
    }
    else  if (hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID) == HAL_SYS_CHIP_PROD_ID_8808S)
    {
        if((index & 0x1f) == 0xd)//aptic
        {
            ramId = RDA8808S_APTIC;  
        }
        else if ((index & 0x1f) == 0x6)//winbond
        {
            ramId = RDA8808S_WINBOND;  
        }
        else if ((index & 0x1f) == 0xa)//emc
        {
            ramId = RDA8808S_EMC;  
        }
        else if ((index & 0x1f) == 0x9)//etron
        {
            ramId = RDA8808S_ETRON;  
        }
        else
        {
            ramId = RDA8808S_APTIC;
        }
    }
    else
    {
        ramId = RDA_APTIC;  
    }

#else // 8809 or later

        if((index & 0x1f) == 0xd)//aptic
        {
            ramId = RDA8809_APTIC;  
        }
        else if ((index & 0x1f) == 0x9)//etron
        {
            ramId = RDA8809_ETRON;              
        }
        else if ((index & 0x1f) == 0x6)//winbond
        {
            ramId = RDA8809_WINBOND;
        }
        else if ((index & 0x1f) == 0xa)//emc
        {
            ramId = RDA8809_EMC;              
            mon_Event(0x66667777);
        }
        else
        {
            ramId =  RDA8809_APTIC;
        }

#endif

    return ramId;
}
#endif // RDA_PSRAM_ADAPTIVE
#endif // !GALLITE_IS_8805

PUBLIC VOID hal_TimDelay(UINT32 delay);

PUBLIC VOID boot_InitFlashDriver(VOID)
{


#ifndef _FLASH_PROGRAMMER
    UINT32* src;
    UINT32* dst;
    // Copy booting code & data from ROM space to internal SRAM.
    for (src = (UINT32*) &_boot_sys_sram_flash_start_location,
             dst = (UINT32*) &_boot_sys_sram_start;
         src < (UINT32*) &_boot_sys_sram_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }
    // Clear booting BSS in SYS RAM
    for (dst = (UINT32*) &_boot_ram_bss_start;
         dst < (UINT32*) &_boot_ram_bss_end;
         dst++)
    {
        *dst = 0;
    }
#endif
    memd_FlashRst();//TODO: reset spi flash
    memd_FlashOpen(tgt_GetMemdFlashConfig());
}

// =============================================================================
// boot_MemInit
// -----------------------------------------------------------------------------
/// Initialize flash memory and RAM memory.
///
/// @param bootSectorStructValid whether boot sector structure is valid.
// =============================================================================
INLINE VOID boot_MemInit(BOOL bootSectorStructValid)
{
    UINT32* src;
    UINT32* dst;
    
#ifdef RDA_PSRAM_ADAPTIVE
    BOOL ramIdValid = FALSE;
    UINT8 ramId = 0;
#endif

#ifndef _FLASH_PROGRAMMER 
    // Copy booting code & data from ROM space to internal SRAM.
    for (src = (UINT32*) &_boot_sys_sram_flash_start_location,
             dst = (UINT32*) &_boot_sys_sram_start;
         src < (UINT32*) &_boot_sys_sram_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }
    // Clear booting BSS in SYS RAM
    for (dst = (UINT32*) &_boot_ram_bss_start;
         dst < (UINT32*) &_boot_ram_bss_end;
         dst++)
    {
        *dst = 0;
    }
#endif


#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5855)
//#ifndef _FLASH_PROGRAMMER 
    // Copy booting code & data from ROM space to internal SRAM.
    for (src = (UINT32*) &_sys_rom_data_base,
             dst = (UINT32*) &_sys_rom_data_start;
         src < (UINT32*) &_sys_rom_data_end;
         src++, dst++)
    {
        *dst = *src;
    }

    // Clear ROM BSS in Internal SRAM.
    for (dst = (UINT32*) &_sys_rom_bss_start;
         dst < (UINT32*) &_sys_rom_bss_end;
         dst++)
    {
        *dst = 0;
    }
//#endif
#endif

#if 1//ndef _FLASH_PROGRAMMER
HAL_SYS_MEM_FREQ_T memClk = HAL_SYS_MEM_FREQ_78M;
    memClk = memClk; 
    // Setup system PLL first
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL  
#ifdef 	__FORCE_WORK_IN_26MHZ_LOCAL_UPTO_52MHZ__
	memClk = HAL_SYS_MEM_FREQ_52M;
#else
	memClk = HAL_SYS_MEM_FREQ_26M;
#endif
#endif

#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
	hal_SysSetupNonePLL();
#else

    if (bootSectorStructValid)
    {
        if (g_bootSectorStructPtr->ebcConfigValidTag == BOOT_SECTOR_EBC_VALID_TAG &&
            g_bootSectorStructPtr->ebcExtConfigValidTag == BOOT_SECTOR_EBC_EXT_VALID_TAG)
        {
            switch (g_bootSectorStructPtr->ebcFreq)
            {
            case HAL_SYS_MEM_FREQ_52M:
            case HAL_SYS_MEM_FREQ_78M:
            case HAL_SYS_MEM_FREQ_104M:
            case HAL_SYS_MEM_FREQ_125M:
            case HAL_SYS_MEM_FREQ_139M:
                memClk = g_bootSectorStructPtr->ebcFreq;
#ifdef RDA_PSRAM_ADAPTIVE
                ramIdValid = boot_EbcRamIdValid(g_bootSectorStructPtr->ramId);
#endif
                break;
            default:
                memClk = HAL_SYS_MEM_FREQ_78M;
                break;
            }
        }
    }

 #ifndef __UPDATE_PROGRAM__
     hal_SysSetupPLL();

 #endif //(__UPDATE_PROGRAM__)
#endif 
#if (RAM_SIZE!=0)
    // Setup EBC clock according to boot sector structure
    //hal_EbcCsSetup(memClk);
#endif

#ifdef MEMD_EXTA_OPEN_EARLY
	boot_IspiOpen(NULL);
	hal_TimDelay(2000);
	boot_IspiSendData(HAL_ISPI_CS_PMU, (RDA_ADDR_PMU_RESET<<16)| RDA_PMU_SOFT_RESETN,FALSE);
	hal_TimDelay(2000);
	boot_IspiSendData(HAL_ISPI_CS_PMU, (RDA_ADDR_PMU_RESET<<16)| RDA_PMU_REGISTER_RESETN|RDA_PMU_SOFT_RESETN,FALSE);
	hal_TimDelay(2000);
    boot_IspiSendData(HAL_ISPI_CS_PMU,0x0dbfd3,FALSE);
	hal_TimDelay(2000);
    boot_IspiSendData(HAL_ISPI_CS_PMU,0x08f953,FALSE);
    //boot_IspiSendData(HAL_ISPI_CS_PMU,0x0dbfd3,FALSE);
    //boot_IspiSendData(HAL_ISPI_CS_PMU,0x0dbfd3,FALSE);
    //pmd_OpalSpiWrite(RDA_ADDR_LDO_POWER_ON_SETTING1, 0xBFD3); // RDA_ADDR_LDO_POWER_ON_SETTING1, set pu_vio1p8_pon
#endif


#ifdef RDA_PSRAM_ADAPTIVE
    if (ramIdValid)
    {
        ramId = g_bootSectorStructPtr->ramId;
    }
    else
    {
        ramId = boot_EbcReadRamId();
    }
    // Save the RAM ID for future reference (and for s/w upgrade
    // to a version with a different boot sector struct location)
    hal_BootSectorSetEbcRamId(ramId);
#endif

#ifdef RAM_CLK_IS_52M
    memClk = HAL_SYS_MEM_FREQ_52M;
#else
#ifdef RAM_CLK_IS_78M
    memClk = HAL_SYS_MEM_FREQ_78M;
#else
#ifdef RAM_CLK_IS_104M
    memClk = HAL_SYS_MEM_FREQ_104M;
#else
#ifdef RAM_CLK_IS_125M
    memClk = HAL_SYS_MEM_FREQ_125M;
#else
#ifdef RAM_CLK_IS_139M
    memClk = HAL_SYS_MEM_FREQ_139M;
#else
    memClk = HAL_SYS_MEM_FREQ_52M; // EBC use 52M clk in default.
#endif//RAM_CLK_IS_139M
#endif//RAM_CLK_IS_125M
#endif//RAM_CLK_IS_104M
#endif//RAM_CLK_IS_78M
#endif//RAM_CLK_IS_52M
#if (RAM_SIZE!=0)
    // Setup EBC clock again according to target configuration
    //hal_EbcCsSetup(memClk);

    // Load target RAM parameter to speed up the booting
    // NOTE: System should NOT access RAM during RAM opening process,
    //       otherwise exception will occur.
    memd_RamOpen(tgt_GetMemdRamConfig());
#endif

#if CHIP_HAS_FLSH_CACHE
	hal_FlashCacheOff();
#endif /* CHIP_HAS_FLSH_CACHE */

    memd_FlashRst();
#ifndef GALLITE_IS_8805
 #if 0//ndef __UPDATE_PROGRAM__ //Init SpiFlashController in memd_FlashOpen, huangyx
    boot_SpiFlashControllerInit();
 #endif   //(__UPDATE_PROGRAM__)

#if !defined(_FLASH_PROGRAMMER) && !defined(_T_UPGRADE_PROGRAMMER)
    boot_ExtRamSizeCheck();
#endif
#endif

    // Load target FLASH parameter to speed up the booting
 //   extern void get_flash_ID0();
//	get_flash_ID0();
 #ifndef __UPDATE_PROGRAM__
     memd_FlashOpen(tgt_GetMemdFlashConfig());

 #endif //(__UPDATE_PROGRAM__)
 //   memd_FlashOpen(tgt_GetMemdFlashConfig());
#if CHIP_HAS_FLSH_CACHE
#ifndef _FLASH_PROGRAMMER
#if 1
	hal_FlashCacheDebugOn();
#endif
	hal_FlashCacheOn(FLASH_CACHE_MAP_00_08MB_TO_16KB);
#endif
#endif /* CHIP_HAS_FLSH_CACHE */
#endif
}


// =============================================================================
// boot_BootSectorGetReloadValid
// -----------------------------------------------------------------------------
/// Check the validity of the Boot Sector reload structure by reading the flag and checksum in
/// the Boot reload structure.
///
/// @return TRUE if valid, FALSE else.
// =============================================================================
INLINE BOOL boot_BootSectorGetReloadValid(VOID)
{
    // Is the address word-aligned?
    if (((UINT32)g_bootSectorReloadStructPtr & 0x3) != 0)
    {
        return FALSE;
    }

    // Is the address in INT-SRAM space?
    if ( ! (
             ( (UINT32)g_bootSectorReloadStructPtr >= KSEG0(REG_INT_SRAM_BASE) &&
                  (UINT32)g_bootSectorReloadStructPtr < KSEG0(REG_INT_SRAM_BASE)+sizeof(HWP_INT_SRAM_T) ) ||
             ( (UINT32)g_bootSectorReloadStructPtr >= KSEG1(REG_INT_SRAM_BASE) &&
                  (UINT32)g_bootSectorReloadStructPtr < KSEG1(REG_INT_SRAM_BASE)+sizeof(HWP_INT_SRAM_T) )
           ) )
    {
        return FALSE;
    }

    if( (g_bootSectorReloadStructPtr->validTag != BOOT_SECTOR_RELOAD_VALID_TAG) ||
        (g_bootSectorReloadStructPtr->checkSum != ~BOOT_SECTOR_RELOAD_VALID_TAG) )
    {
        return FALSE;
    }

    g_bootSectorReloadStructPtr->validTag = 0;
    g_bootSectorReloadStructPtr->checkSum = 0;

    return TRUE;
}


// =============================================================================
// boot_SectorReload
// -----------------------------------------------------------------------------
/// Relocate the ram image to the execution section and jump to the entry point.
///
/// @return TRUE if succeeded, FALSE else.
// =============================================================================
INLINE BOOL boot_SectorReload(VOID)
{
    UINT32 i, j;
    for (j=0; j<BOOT_SECTOR_RELOAD_MAX_NUM; j++)
    {
        UINT32 *mapAddr = (UINT32 *)KSEG0(g_bootSectorReloadStructPtr->ctx[j].mapAddress);
        UINT32 *stroreAddr = (UINT32 *)KSEG0(g_bootSectorReloadStructPtr->ctx[j].storeAddress);
        UINT32 size = g_bootSectorReloadStructPtr->ctx[j].size;

        for (i=0; i<size; i++)
        {
            *mapAddr++ = *stroreAddr++;
        }
    }
    // Execute at the PC in the execution context with the provided SP
    boot_HstMonitorX();
    return TRUE;
}


// =============================================================================
// boot_Sector
// -----------------------------------------------------------------------------
/// Entry of the Boot Sector in the first flash sector.
/// 
/// This code is executed right after the Boot ROM and right before the
/// Boot Loader, in normal mode execution. It does the following:
/// 
/// * Writes the EBC RAM chip select configuration if a valid configuration
/// has been found in the Boot Sector structure.
/// 
/// * If the command in the Boot Sector structure tells us to enter
/// boot monitor, we do so after sending an event.
/// 
/// * Else, we enter the Boot Loader, providing it with the #param.
/// 
/// @param param Generic parameter only used to invoke boot_LoaderEnter(param).
// =============================================================================
PROTECTED VOID BOOT_SECTOR_SECTION_START boot_Sector(UINT32 param)
{
    BOOL bootSectorStructValid = boot_BootSectorGetStructValidity();
    BOOL bootSectorReloadValid = boot_BootSectorGetReloadValid(); 

     #if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
     hwp_abb->abb_rsvd_status =0;
     hwp_abb->denoise_pwr_cfg =0;
     #endif
 
    // Check whether to reload host monitor codes (from RAM,
    // for T-card flash programmer).
    if(bootSectorReloadValid)
    {
       boot_SectorReload();
    }  
    // Writes the EBC RAM chip select configuration if a valid configuration
    // has been found in the Boot Sector structure.
    // This is the first try to recover RAM access capability.
#if (RAM_SIZE!=0)
    if (bootSectorStructValid)
    {
        if (g_bootSectorStructPtr->ebcConfigValidTag == BOOT_SECTOR_EBC_VALID_TAG)
        {
            mon_Event(BOOT_SECTOR_EVENT_CONFIG_RAM);
            hwp_memBridge->CS_Time_Write = g_bootSectorStructPtr->ebcConfigRamTimings & 0xfffff1ff;
            hwp_memBridge->CS_Config[1].CS_Mode = g_bootSectorStructPtr->ebcConfigRamMode & 0xffcfffff;
        }
    }
#endif
    // Use to fix some startup missing feature (like usb clock on greenstone)
    boot_SectorFixRom();
     #if (CHIP_ASIC_ID <= CHIP_ASIC_ID_5856E)
     g_halSysCsAddress = (UINT32*)&hwp_sysIrq->sc;
     #endif
    // Check whether the tool requests the system to enter boot monitor.
    if (bootSectorStructValid)
    {
        BOOT_SECTOR_CMD_T cmd = g_bootSectorStructPtr->command;
        // Execute the command of the Boot Sector structure only once.
        g_bootSectorStructPtr->command = 0;
        // If the command in the Boot Sector structure tells us to enter
        // boot monitor, we do so after sending an event.
        if (cmd == BOOT_SECTOR_CMD_ENTER_BOOT_MONITOR)
        {
            // Write in the exchange status we are in the 
            // monitor, ready for operation.
            hwp_debugHost->p2h_status = DEBUG_HOST_P2H_STATUS(BOOT_HST_STATUS_WAITING_FOR_COMMAND);
            mon_Event(BOOT_SECTOR_EVENT_ENTER_MONITOR);
            boot_SectorEnterBootMonitor();
        }
    }
    // Clear any remaining exchange status.
    hwp_debugHost->p2h_status = DEBUG_HOST_P2H_STATUS(BOOT_HST_STATUS_NONE);

    // Check whether boot download keys are pressed and the system
    // should enter boot monitor.
    // This ensures that the system can be recovered even when there
    // is an error during flash/RAM initialization or during boot time
    // with USB debug connection.
    boot_ForceMonitorKeyCheck();

    // Initialize external RAM and flash.
    // This is the second try to recover RAM access capability.
    boot_MemInit(bootSectorStructValid);
    // Boot loader is now always linked with boot sector.
    boot_LoaderEnter(param);
}


