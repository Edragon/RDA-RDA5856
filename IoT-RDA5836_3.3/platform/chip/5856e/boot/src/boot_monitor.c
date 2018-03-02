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
//                                                                            //
/// @file boot_monitor.c                                                      //
///     RMC monitor
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "boot_sector_chip.h"
#include "boot_sector.h"

#include "global_macros.h"
#include "timer.h"
#include "uart.h"
#include "debug_host.h"
#include "debug_uart.h"
#include "debug_port.h"
#include "cfg_regs.h"
#include "cp0.h"
#include "sys_ctrl.h"
#include "tcu.h"

#include "boot_host.h"
#include "boot.h"
#include "bootp_mode.h"
#include "bootp_host.h"
#include "boot_uart.h"
#include "boot_ispi.h"
#include "boot_usb_monitor.h"
#include "bootp_uart_monitor.h"
#include "bootp_debug.h"
#include "boot_nand.h"

#include "hal_debug.h"
#include "hal_timers.h"
#include "sys_ctrl.h"
#include "rf_if.h"


// Forward declaration.
PROTECTED VOID boot_MonitorDoMonitoring(VOID);
PUBLIC VOID boot_SetIOMux(void);

// =============================================================================
// boot_Monitor
// -----------------------------------------------------------------------------
/// Entry point for the boot monitor. This is the first C function called out
/// of boot. When exiting this function, we jump into flash with an almost not
/// altered stack pointer.
// =============================================================================
PROTECTED VOID boot_Monitor(VOID)
{
    // Enable the host from within. (Forced)
    // Set clock
    // Formula from the uart driver
    // Divider mode is 4
    // There is no PLL during monitoring
    // Rq: the plus 4/2 * rate is a subtle way to get the upper integer
    //INT32 timebegin;
    //INT32 timeend;
    //INT32 clock=0;
    hwp_sysCtrl->per_module_en |= SYS_CTRL_TIMER_EN | SYS_CTRL_DBG_HST_EN;
	
    hwp_rf_if->adpll_setting1 = RF_IF_SYS_CLK_SEL_FAST | RF_IF_SYS_FAST_CLK_SEL_XTAL_26M;
	
    // Force on.
    hwp_debugHost->mode = DEBUG_HOST_FORCE_ON;

#if 0
    UINT32 divisor_mode = (hwp_debug_uart->ctrl & DEBUG_UART_DIVISOR_MODE ) >> 20;


    if(divisor_mode == 0)
    {
        divisor_mode = 4;
    }
    else
    {
       divisor_mode= 16;
    }

    UINT32 divider = BOOT_HST_SYSTEM_CLOCK / (divisor_mode * BOOT_HST_HOST_RATE);

    hwp_sysCtrl->uart_clk_cfg = SYS_CTRL_UART_CLK_NUM(1) | SYS_CTRL_UART_DIVIDER(divider) |
        SYS_CTRL_UART_SEL_PLL_SLOW;

    
    
// Copy from hal_SysSetupPLL(). Enable 26M clock.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_PLL_BYPASS_PASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_PLL_LOCK_RESET_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_POWER_DOWN
                          | SYS_CTRL_BB_PLL_BYPASS_PASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_RESET
                          ;

    hwp_sysCtrl->Pll_Ctrl = SYS_CTRL_PLL_ENABLE_ENABLE
                          | SYS_CTRL_PLL_BYPASS_PASS
                          | SYS_CTRL_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_PLL_LOCK_RESET_NO_RESET
                          | SYS_CTRL_BB_PLL_ENABLE_ENABLE
                          | SYS_CTRL_BB_PLL_BYPASS_PASS
                          | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_DISABLE
                          | SYS_CTRL_BB_PLL_LOCK_RESET_NO_RESET;

   //hwp_tcu->LPS_PU_Ctrl &= ~TCU_LPS_PU_CO0_ON;
    hwp_tcu->LPS_PU_Ctrl |= TCU_LPS_PU_CLK_RF_ON;
    timebegin=hal_TimGetUpTime();

    while(!(hwp_sysCtrl->pll_ctrl & SYS_CTRL_BBPLL_LOCKED))
    {
        timeend=hal_TimGetUpTime();
        clock=(timeend-timebegin)<<6;              //over 100us 26m 16.384khz 1/16.384khz= ms
        if(clock>=100)
        {
           g_bootBootMode |= BOOT_MODE_USB_MONITOR_DISABLE;
           break;
    	}
    }

    if(clock<100)
    {
        hwp_sysCtrl->sys_clk_cfg |= SYS_CTRL_SYS_CLK_SEL_FAST; 
    }
 #endif       
    #if 0
    //while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED));
    while(!(hwp_sysCtrl->Sel_Clock & SYS_CTRL_PLL_LOCKED)) //wait for Sel_Clock(PLL_LOCKED=1)
    {
        timeend=hal_TimGetUpTime();
        clock=(timeend-timebegin)<<6;              //over 100us 26m 16.384khz 1/16.384khz= ms
        if(clock>=100)
        {
           g_bootBootMode |= BOOT_MODE_USB_MONITOR_DISABLE;
           break;
    	}
	
    }
    if(clock<100)
    {
        hwp_sysCtrl->Pll_Ctrl |= SYS_CTRL_PLL_CLK_FAST_ENABLE_ENABLE
                               | SYS_CTRL_BB_PLL_CLK_FAST_ENABLE_ENABLE;

        hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
                | SYS_CTRL_SYS_SEL_FAST_FAST
                | SYS_CTRL_BB_SEL_FAST_FAST
                | SYS_CTRL_MPMC_SYS_SAME_ENABLE
                | SYS_CTRL_SYS_SEL_TCU_26M
                ;  
    }
    //hwp_sysCtrl->REG_DBG    = SYS_CTRL_PROTECT_LOCK;
    //hwp_sysCtrl->Sel_Clock = SYS_CTRL_SLOW_SEL_RF_RF
    //        | SYS_CTRL_SYS_SEL_FAST_FAST;
    
    #endif
    VOID(*flashEntry)(UINT32);
    {
        flashEntry = (VOID(*)(UINT32)) BOOT_SECTOR_ENTRY_POINT;
    }

    // Open the ISPI to access and configure stuff, using the default,romed
    // configuration.
    // FIXME There just to link boot_ispi into the ROM.
    //boot_IspiOpen(NULL);
    // Do monitoring
    boot_MonitorDoMonitoring();
    // Boot into flash.
    //BOOT_PROFILE_PULSE(BOOT_DIRECT_IN_FLASH);
    flashEntry(BOOT_SECTOR_STANDARD_MAIN);
}



// =============================================================================
// boot_MonitorDoMonitoring
// -----------------------------------------------------------------------------
/// This function actually implement the monitoring operations.
// =============================================================================
PROTECTED VOID boot_MonitorDoMonitoring(VOID)
{
    BOOT_HOST_USB_MONITOR_CTX_T hostUsbContext;
    BOOT_MONITOR_OP_STATUS_T    monitorLoopStatus = BOOT_MONITOR_OP_STATUS_NONE;
    BOOL flashProgrammed = FALSE;

    hostUsbContext.MonitorMode = BOOT_HOST_USB_MODE_BOOT;
    // Get current time to calculate time-out exit
    UINT32 enterTime = hwp_timer->hwtimer_curval;

    // Initializes the uart baudrate detection variables.
    //g_bootUartMonitorBrDetected = FALSE;
    //g_bootUartMonitorBaudRate   = 0;

    {
        // Check if SPI flash is programmed
        if (*(BOOT_FLASH_PROGRAMMED_PATTERN_ADDRESS) == BOOT_FLASH_PROGRAMMED_PATTERN)
        {
            flashProgrammed = TRUE;
        }
    }
    // We enter the monitor if access is forced through 
    // boot mode or there is no code on CS0.
    if ((g_bootBootMode & BOOT_MODE_FORCE_MONITOR) || (!flashProgrammed))
    {
        BOOT_PROFILE_PULSE(BOOT_ENTER_MONITOR_LOOP);

        // If we are out of reset, set the status to 'waiting for a command'.
        // Otherwise, keep the status as it was probably set by the boot sector
        // and carries a specific meaning.
        if (hwp_debugHost->p2h_status == BOOT_HST_STATUS_NONE)
        {
            hwp_debugHost->p2h_status = BOOT_HST_STATUS_WAITING_FOR_COMMAND;
        }
#if 0
        // Open the UART monitor, only when the boot mode requires it.
        if (g_bootBootMode & BOOT_MODE_UART_MONITOR_ENABLE)
        {
            boot_UartMonitorOpen();
        }
#endif       

        boot_SetIOMux();
		
        // Open the USB monitor, except when to boot mode prevents it.
        if (!(g_bootBootMode & BOOT_MODE_USB_MONITOR_DISABLE))
        {   
            //mon_Event(0x1015102a);
            //open usb_PHY and usb clk and GPIO8/9
            boot_usbinitclockenable();
            boot_UsbInitVar();
            //boot_HostUsbClockEnable();
            boot_UsbOpen(boot_HostUsbDeviceCallback);
            // We never exit this function, so the hostUsbContext
            // is valid (On the stack).
            boot_HostUsbOpen(&hostUsbContext, 0, 0);
         }
         
        mon_Event(0x56);
        while(1)
        {
            // We are in the monitor. We are indefinitely trying to 
            // execute command from any (available) monitor. This
            // is done through a dedicated function per monitor
            // (boot_HstMonitor for the host monitor,
            // boot_UartMonitor for the uart monitor,
            // boot_HostUsbRecv for the USB monitor.)
            // When any of them receive a "Exit the monitor"
            // BOOT_HST_MONITOR_END_CMD command, it
            // returns BOOT_MONITOR_OP_STATUS_EXIT, and we check that to leave the 
            // monitor loop.
            monitorLoopStatus = BOOT_MONITOR_OP_STATUS_NONE;

            // Host monitor always enabled. 
            monitorLoopStatus |= boot_HstMonitor();
#if 0
            // Process the UART monitor, only when to boot mode requires it.
            if (g_bootBootMode & BOOT_MODE_UART_MONITOR_ENABLE)
            {
                monitorLoopStatus |= boot_UartMonitor();
            }
#endif
#if 1
            // Process the USB monitor, except when to boot mode prevents it.
            if (!(g_bootBootMode & BOOT_MODE_USB_MONITOR_DISABLE))
            {
                // Receive a USB packet, parse it and execute 
                // the command if any.
                // Return BOOT_MONITOR_OP_STATUS_EXIT if the exit monitor command is received.
                monitorLoopStatus |= boot_HostUsbRecv();
                // If a read command was received, send a packet
                // with the read data.
                boot_HostUsbSend();
				
            }
#endif
            // Check the status returned by the monitors.
            if (monitorLoopStatus & BOOT_MONITOR_OP_STATUS_EXIT)
            {
                // Exit the monitoring loop
                break;
            }

            // Check we received a command, and update the timeout
            // FIXME Fixe here to implement the behaviour about the timeout
            // (No command or after the command)
            if (monitorLoopStatus & BOOT_MONITOR_OP_STATUS_CONTINUE)
            {
                // Update the timeout or set a variable bypassing the
                // timeout stuff.
                enterTime = hwp_timer->hwtimer_curval;
            }

            if (monitorLoopStatus == BOOT_MONITOR_OP_STATUS_NONE)
            {
                // No command received.
                // Exit the loop if valid flash and timeout.
                if ((*(BOOT_FLASH_PROGRAMMED_PATTERN_ADDRESS) == BOOT_FLASH_PROGRAMMED_PATTERN)
                 && ((hwp_timer->hwtimer_curval - enterTime) > (BOOT_IDLE_TIMEOUT*16384)))
                {
                    // If no command was sent, we have to tell we're not ready
                    // anymore to handle new commands.
                    hwp_debugHost->p2h_status = BOOT_HST_STATUS_NONE;
                    break;
                }
              }
        }
#if 0
        // Close the UART monitor, only when the boot mode requires it.
        if (g_bootBootMode & BOOT_MODE_UART_MONITOR_ENABLE)
        {
            boot_UartMonitorClose();
        }
#endif

        // Close the USB monitor, except when to boot mode prevents it.
        if (!(g_bootBootMode & BOOT_MODE_USB_MONITOR_DISABLE))
        {
            boot_HostUsbClose();
            boot_UsbClose();
        }

    }
}

