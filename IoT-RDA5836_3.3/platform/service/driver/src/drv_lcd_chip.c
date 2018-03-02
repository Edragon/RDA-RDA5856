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


#if (CSW_EXTENDED_API_LCD == 1)

#include "mci_lcd.h"

// This is bad - this is coming from the MMI
// and the platform should be independant of the MMI
EXPORT UINT32 get_lcd_frame_buffer_address(VOID);

// ============================================================================
// Init_DI
// ----------------------------------------------------------------------------
// ============================================================================
VOID Init_DI(UINT32 background, VOID** unused)
{
    mci_LcdInit(background);
}

// ============================================================================
// mci_LcdBlockWrite
// ----------------------------------------------------------------------------
VOID BlockWrite_DI(UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
#ifdef __PRJ_WITH_SPILCD__
    mci_LcdBlockWrite((UINT16*)(get_lcd_frame_buffer_address() | 0xa0000000),startx,starty,endx,endy);
#else
    mci_LcdBlockWrite((UINT16*)get_lcd_frame_buffer_address(),startx,starty,endx,endy);
#endif
}

UINT8 partial_display_align_DI(VOID)
{
    return 0;
}

VOID lcd_update_image_DI(UINT16 startx, UINT16 starty,UINT16 Width,UINT16 Height)
{
}

VOID lcd_chipinit_DI(VOID)
{
}

#endif // CSW_EXTENDED_API_LCD


