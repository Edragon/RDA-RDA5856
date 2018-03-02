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



#include "cs_types.h"

#include "global_macros.h"
#include "sys_irq.h"

#include "boot_usb.h"
#include "bootp_debug.h"
#include "register.h"


// =============================================================================
// boot_IrqDispatch
// -----------------------------------------------------------------------------
/// BOOT romed IRQ dispatch.
/// Exceptions are handled in the rom (boot_rom.S) assemlby code.
/// Here, we check the cause is really USB before calling the USB handler,
/// otherwise sending an error if the interrupt cause is not supported.
// =============================================================================
PROTECTED VOID boot_IrqDispatch(VOID)
{
    if (hwp_sysIrq->cause & SYS_IRQ_SYS_IRQ_USBC)
    {
        // Call the USB handler if, and only if,
        // the interruption is really for the USB.
        #if (CHIP_HAS_USB == 1)
        boot_UsbIrqHandler(SYS_IRQ_USBC);
	 #endif
    }
    else
    {
        // We received an unsupported (here) interruption
        BOOT_PROFILE_PULSE(BOOT_ROMED_XCPU_ERROR);

        // Stay here.
        while (1);
    }
}

