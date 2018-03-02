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
/// @file drv_lcd.c                                                           //
/// That file provides an implementation for the AU LCD screen.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "lcdd_config.h"
#include "lcdd_types.h"
#include "hal_timers.h"
#include "hal_gouda.h"
#include "hal_gpio.h"

#include "pmd_m.h"
#include "sxr_tls.h"

// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

// =============================================================================
//  MACROS
// =============================================================================

#define LCM_WR_REG(Addr, Data)  { while(hal_GoudaWriteReg(Addr, Data)!= HAL_ERR_NO);}
#define LCM_WR_DAT(Data)        { while(hal_GoudaWriteData(Data)     != HAL_ERR_NO);}
#define LCM_WR_CMD(Cmd)         { while(hal_GoudaWriteCmd(Cmd)       != HAL_ERR_NO);}

#define LCDD_BUILD_CMD_WR_CMD(c,i,r) do{c[i].isData=FALSE; c[i].value=r;}while(0)
#define LCDD_BUILD_CMD_WR_DAT(c,i,d) do{c[i].isData=TRUE; c[i].value=d;}while(0)
#define LCDD_BUILD_CMD_WR_REG(c,i,r,d) do{LCDD_BUILD_CMD_WR_CMD(c,i,r); LCDD_BUILD_CMD_WR_DAT(c,i+1,d);}while(0)

#define LCDD_TIME_MUTEX_RETRY 50

// =============================================================================
// Screen properties
// =============================================================================

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// Sleep status of the LCD
PRIVATE BOOL g_lcddInSleep = FALSE;

// =============================================================================
//  FUNCTIONS
// =============================================================================

#define RDA_LCD_BASE		0xa1a20000
#define RDA_LCD_CON			(RDA_LCD_BASE + 0)
#define RDA_LCD_TXDATA		(RDA_LCD_BASE + 0x4)
#define RDA_LCD_RXDATA		(RDA_LCD_BASE + 0x8)
#define RDA_LCD_SPISTATUE	(RDA_LCD_BASE + 0xc)
#define RDA_LCD_INT			(RDA_LCD_BASE + 0x10)

//LCD开关命令
#define		M_LCD_ON		0xaf
#define		M_LCD_OFF		0xae
//设置上电控制模式
#define		M_LCD_POWER_BC	0x2c
#define		M_LCD_POWER_VR	0x2a
#define		M_LCD_POWER_VC	0x29
#define		M_LCD_POWER_ALL	0x2f
//V5内部电压调节电阻设置……
#define		M_LCD_SETR_0	0x20
#define		M_LCD_SETR_1	0x21
#define		M_LCD_SETR_2	0x22
#define		M_LCD_SETR_3	0x23
#define		M_LCD_SETR_4	0x24
#define		M_LCD_SETR_5	0x25
#define		M_LCD_SETR_6	0x26
#define		M_LCD_SETR_7	0x27
//偏压设置
#define		M_LCD_BIAS_9	0xa2		//V5时选此选项设置
#define		M_LCD_BIAS_7	0xa1		//V3时选此选项设置
#define		M_LCD_ELE_VOL	0x81		//电量设置模式（显示亮度）
#define		M_LCD_VDD_SET	M_LCD_SETR_4
#define		M_LCD_VDD		M_LCD_BIAS_9
#define		M_LCD_COM_NOR	0xc0		//正常方式
#define		M_LCD_COM_REV	0xc8		//反相
//Segment方向选择
#define		M_LCD_SEG_NOR	0xa0		//正常
#define		M_LCD_SEG_REV	0xa1		//反向
//全屏点亮/变暗指令
#define		M_LCD_ALL_LIGNT	0xa5		//LCD ALL paint ON
#define		M_LCD_ALL_LOW	0xa4		//Normal Display mode
//正相反相显示控制指令，RAM中数据不变
#define		M_LCD_ALL_NOR	0xa6		//正相
#define		M_LCD_ALL_REV	0xa7		//反相
//静态指示器控制指令
#define		M_LCD_STATIC_ON	0xad		//ON
#define		M_LCD_STATIC_OFF 0xac		//OFF
//设置显示起始行对应RAM行号
#define		M_LCD_BEGIN_LINE 0x40		//基数，后面可加的尾数可为0~63
//设置当前页基数
#define		M_LCD_COL_PAGE	0xb0		//基数指令，后可加尾数0~8
//设置当前列基数
#define		M_LCD_COL_LINE_LOW	0x04		//基数指令，低四位有效
#define		M_LCD_COL_LINE_HIG	0x10		//基数指令，低四位有效
#define LCD_INITIAL_COLOR	0x00			//定义LCD屏初始化时的背景色
#define LCD_X_MAX			128-1			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			64-1			//屏幕的Y轴的物理宽度


#define LCD_DataWrite(Data)        { while(hal_GoudaWriteData(Data)     != HAL_ERR_NO);}
#define LCD_RegWrite(Cmd)         { while(hal_GoudaWriteCmd(Cmd)       != HAL_ERR_NO);}
PUBLIC LCDD_ERR_T lcdd_Init(VOID);

PUBLIC LCDD_ERR_T lcdd_update_screen(UINT8* Data, LCDD_ROI_T *rect)
{
    LCDD_CONFIG_T lcddReadConfig=LCDD_READ_CONFIG;
	unsigned char i,j;
	unsigned char uiTemp;
	uiTemp = LCD_Y_MAX;
	uiTemp = uiTemp>>3;
	//lcdd_Init();
    //hal_HstSendEvent(SYS_EVENT, 0x11030700);
    //hal_HstSendEvent(SYS_EVENT, g_lcddInSleep);
    if(g_lcddInSleep)
        hal_GoudaWakeup();
    //hal_HstSendEvent(SYS_EVENT,0x852963);
    //hal_HstSendEvent(SYS_EVENT,Data);
	for(i=0;i<=uiTemp;i++)								//往LCD中填充初始化的显示数据
	{
		LCD_RegWrite(0xb0+i);
		LCD_RegWrite(0x01);
		LCD_RegWrite(0x10);
		for(j=0;j<=LCD_X_MAX;j++)
		{
			/*hwp_sysIfc->std_ch[7].start_addr  =  (UINT32) LCD_DATA[];
    		hwp_sysIfc->std_ch[7].tc          =  sizeof(LCD_DATA);
			hwp_sysIfc->std_ch[7].control     = (0x400521);*/
			//LCD_DataWrite(LCD_DATA[i*(LCD_X_MAX+1)+j]);
			LCD_DataWrite(Data[i*(LCD_X_MAX+1)+j]);
		}
	}
    if(g_lcddInSleep)
        hal_GoudaSleep();
    return LCDD_ERR_NO;
}

// ============================================================================
// lcdd_Open
// ----------------------------------------------------------------------------
/// Open the LCDD driver.
/// It must be called before any call to any other function of this driver.
/// This function is to be called only once.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Open(VOID)
{
	UINT32 i=0;

    LCDD_CONFIG_T lcddReadConfig=LCDD_READ_CONFIG;

lcdd_SetBrightness(7);

#ifdef USE_SPI_LCD
    hal_GoudaSerialOpen(LCDD_SPI_LINE_TYPE, LCDD_SPI_FREQ, &lcddReadConfig.config, 0);
#else
    hal_GoudaOpen(&lcddReadConfig.config, lcddReadConfig.timings, 0);
#endif    

    COS_Sleep(20 );

    lcdd_Init();

    g_lcddInSleep = FALSE;

    return LCDD_ERR_NO;
}

PUBLIC LCDD_ERR_T lcdd_Init(VOID)
{
	LCD_RegWrite(M_LCD_ON);							//LCD On
	LCD_RegWrite(M_LCD_POWER_ALL);					//设置上电控制模式
	
	LCD_RegWrite(M_LCD_ELE_VOL);					//电量设置模式（显示亮度）
	LCD_RegWrite(0x0f);								//指令数据0x0000~0x003f
	
	LCD_RegWrite(M_LCD_VDD_SET);					//V5内部电压调节电阻设置
	LCD_RegWrite(M_LCD_VDD);						//LCD偏压设置，V3时选
	
	LCD_RegWrite(M_LCD_COM_REV);					//Com 扫描方式设置
	LCD_RegWrite(M_LCD_SEG_NOR);					//Segment方向选择
	LCD_RegWrite(M_LCD_ALL_LOW);					//全屏点亮/变暗指令
	LCD_RegWrite(M_LCD_ALL_NOR);					//正向反向显示控制指令
	
	LCD_RegWrite(M_LCD_STATIC_OFF);					//关闭静态指示器
	LCD_RegWrite(0x00);								//指令数据
	
	LCD_RegWrite(M_LCD_BEGIN_LINE+32);		
    return LCDD_ERR_NO;
}

// ============================================================================
// lcdd_Close
// ----------------------------------------------------------------------------
/// Close the LCDD driver
/// No other functions of this driver should be called after a call to 
/// #lcdd_Close.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Close(VOID)
{
    return LCDD_ERR_NO;
}

 
// ============================================================================
// lcdd_SetContrast
// ----------------------------------------------------------------------------
/// Set the contrast of the 'main'LCD screen.
/// @param contrast Value to set the contrast to.
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SetContrast(UINT32 contrast)
{
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_SetStandbyMode
// ----------------------------------------------------------------------------
/// Set the main LCD in standby mode or exit from it
/// @param standbyMode If \c TRUE, go in standby mode. 
///                    If \c FALSE, cancel standby mode. 
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SetStandbyMode(BOOL standbyMode)
{
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_PartialOn
// ----------------------------------------------------------------------------
/// Set the Partial mode of the LCD
/// @param vsa : Vertical Start Active
/// @param vea : Vertical End Active
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_PartialOn(UINT16 vsa, UINT16 vea)
{
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_PartialOff
// ----------------------------------------------------------------------------
/// return to Normal Mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_PartialOff(VOID)
{
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_WakeUp
// ----------------------------------------------------------------------------
/// Wake the main LCD screen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_WakeUp(VOID)
{
    LCDD_CONFIG_T lcddReadConfig=LCDD_READ_CONFIG;
    hal_GoudaWakeup();
    //lcdd_Open();
    g_lcddInSleep = FALSE;
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_Sleep
// ----------------------------------------------------------------------------
/// Set the main LCD screen in sleep mode.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Sleep(VOID)
{
    hal_GoudaSleep();
    g_lcddInSleep = TRUE;
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_GetScreenInfo
// ----------------------------------------------------------------------------
/// Get information about the main LCD device.
/// @param screenInfo Pointer to the structure where the information
/// obtained will be stored
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_GetScreenInfo(LCDD_SCREEN_INFO_T* screenInfo)
{
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_Busy
// ----------------------------------------------------------------------------
/// This function is not implemented now for the driver
// ============================================================================
PUBLIC BOOL lcdd_Busy(VOID)
{
    return FALSE;
}

PUBLIC char* lcdd_GetStringId(VOID)
{
   static char mzl02_id_str[] = "mzl02\n"; 
   return mzl02_id_str; 
}


