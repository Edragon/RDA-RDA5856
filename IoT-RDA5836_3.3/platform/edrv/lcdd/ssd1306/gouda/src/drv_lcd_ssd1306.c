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
#include "drv_lcd_ssd1306.h"

#if (CHIP_HAS_GOUDA==1)
#define SSD1306_USE_GOUDA
#endif
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
//PUBLIC LCDD_ERR_T lcdd_Init(VOID);


#ifdef SSD1306_USE_GOUDA
// LCDD screen config.
static const LCDD_CONFIG_T g_tgtLcddCfg = TGT_LCDD_CONFIG;

PRIVATE LCDD_ERR_T lcdSsd1306pOpen(VOID)
{
    //lcddp_CheckProductId();

#ifdef USE_SPI_LCD
    hal_GoudaSerialOpen(LCDD_SPI_LINE_TYPE, LCDD_SPI_FREQ, &g_tgtLcddCfg.config, 0);
#else
    hal_GoudaOpen(&g_tgtLcddCfg.config, g_tgtLcddCfg.timings, 0);
#endif

    // Init code 
    COS_Sleep(50 );                         // Delay 50 ms
   // lcddp_Init(); 

    g_lcddInSleep = FALSE;
    
    if(0)
        {
        dsp_single_colour(0xf8,0x00);//红色
        COS_Sleep(500 );	
        	
        dsp_single_colour(0x07,0xe0);//绿色
        COS_Sleep(500 );	
        	
        dsp_single_colour(0x00,0x1f);//蓝色
        COS_Sleep(500 );	
     }

	return LCDD_ERR_NO;
}


PRIVATE VOID lcdSsd1306pWriteCmd(UINT8 cmd)
{
	while(hal_GoudaWriteCmd(cmd)       != HAL_ERR_NO);
}

PRIVATE VOID lcdSsd1306pWriteData(UINT8 *pData, UINT32 length)
{
	UINT32 i;

	for(i = 0; i < length; i++)
	{
		while(hal_GoudaWriteData(pData[i])  != HAL_ERR_NO);
	}
}
#else

LCD_GPIO_CONFIG_STRUCT_T g_lcd_gpio_config;

PRIVATE VOID LCD_SSD1306_RESET_ENABLE(VOID)
{
	hal_GpioClr(g_lcd_gpio_config.gpio_res);
}

PRIVATE VOID LCD_SSD1306_RESET_DISABLE(VOID)
{
	hal_GpioSet(g_lcd_gpio_config.gpio_res);
}

PRIVATE VOID LCD_SSD1306_DC_DATA_ENABLE(VOID)
{
	hal_GpioSet(g_lcd_gpio_config.gpio_dc);
}

PRIVATE VOID LCD_SSD1306_DC_COMMAND_ENABLE(VOID)
{
	hal_GpioClr(g_lcd_gpio_config.gpio_dc);
}



PRIVATE VOID lcdSsd1306pWriteCmd(UINT8 cmd)
{
#ifdef SPI_4_WIRE_MODE
	LCD_SSD1306_DC_COMMAND_ENABLE();
	hal_SpiSendData(HAL_SPI_2,HAL_SPI_CS0,&cmd,1);
	while(!hal_SpiTxDmaDone(HAL_SPI_2,HAL_SPI_CS0));
	hal_SpiClearTxDmaDone(HAL_SPI_2);
#endif
#ifdef SPI_3_WIRE_MODE
	UINT8 data[4];
	data[0] = 0x57;
	data[1] = 0x01;
	data[2] = 0x00;
	data[3] = 0x00;
	//hal_HstSendEvent(BOOT_EVENT, 0x88880d00);
	hal_SpiSendData_32Bit(HAL_SPI_2,HAL_SPI_CS0,(UINT32)(&data),1);
	//hal_HstSendEvent(BOOT_EVENT, 0x88880d01);
	while(!hal_SpiTxFinished(HAL_SPI_2,HAL_SPI_CS0));
	//hal_HstSendEvent(BOOT_EVENT, 0x88880d02);
#endif
}

PRIVATE VOID lcdSsd1306pWriteData(UINT8 *pData, UINT32 length)
{
#ifdef SPI_4_WIRE_MODE
	LCD_SSD1306_DC_COMMAND_ENABLE();
	LCD_SSD1306_DC_DATA_ENABLE();
	hal_SpiSendData(HAL_SPI_2, HAL_SPI_CS0, pData, length);
	while(!hal_SpiTxDmaDone(HAL_SPI_2,HAL_SPI_CS0));
	hal_SpiClearTxDmaDone(HAL_SPI_2);
#endif
#ifdef SPI_3_WIRE_MODE
	UINT8 *p=pData;
	UINT32 i;
	//UINT32 data;
	UINT8 data[4];
	//hal_HstSendEvent(BOOT_EVENT, 0x88880d03);
	for(i=0; i<length; i++)
	{
		//data = ((UINT32)(*p)) & 0xff;
		//data |= 0x100;
		//data[0] = *p;
		data[0] = 0x57;
		data[1] = 0x01;
		data[2] = 0x00;
		data[3] = 0x00;
		p++;
		hal_SpiSendData_32Bit(HAL_SPI_2,HAL_SPI_CS0,(UINT32)(&data),1);
	//hal_HstSendEvent(BOOT_EVENT, 0x88880d04);
		while(!hal_SpiTxFinished(HAL_SPI_2,HAL_SPI_CS0));
	//hal_HstSendEvent(BOOT_EVENT, 0x88880d05);
	}
	//hal_HstSendEvent(BOOT_EVENT, 0x88880d06);
#endif

	
}


PRIVATE VOID lcdSsd1306pOpenSpi(VOID)
{
	 HAL_SPI_CFG_T lcdSsd1306SpiCfg =
	{
	       .enabledCS    = HAL_SPI_CS0,
	       .csActiveLow    = TRUE,
	       .inputEn        = FALSE,
	       .clkFallEdge    = TRUE,
	       .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_0,
	       .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
	       .diDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
	       .csDelay        = HAL_SPI_HALF_CLK_PERIOD_1,
	       .csPulse        = HAL_SPI_HALF_CLK_PERIOD_0,//HAL_SPI_HALF_CLK_PERIOD_1
#ifdef SPI_4_WIRE_MODE
	       .frameSize      = 8,
#endif
#ifdef SPI_3_WIRE_MODE
		.frameSize      = 9,
#endif
	       .oeRatio        = 0,
	       .spiFreq        = 1000000,//5M 5000000
	       .rxTrigger      = HAL_SPI_RX_TRIGGER_1_BYTE,
	       .txTrigger      = HAL_SPI_TX_TRIGGER_1_EMPTY,
	       .rxMode         = HAL_SPI_DIRECT_POLLING,
#ifdef SPI_4_WIRE_MODE
	       .txMode         = HAL_SPI_DMA_POLLING,
#endif
#ifdef SPI_3_WIRE_MODE
		.txMode         = HAL_SPI_DIRECT_POLLING,
#endif
	       .mask           = {0,0,0,0,0},
	       .handler        = NULL
	};
	hal_SpiOpen(HAL_SPI_2,HAL_SPI_CS0, &lcdSsd1306SpiCfg);
    	hal_SpiActivateCs(HAL_SPI_2,HAL_SPI_CS0);
}

PRIVATE LCDD_ERR_T lcdSsd1306pGpioInit(VOID)
{
	g_lcd_gpio_config.gpio_sclk.gpioId = LCD_SSD1306_GPIOPIN_SCLK;
	g_lcd_gpio_config.gpio_sdo.gpioId = LCD_SSD1306_GPIOPIN_SDO;
	g_lcd_gpio_config.gpio_res.gpioId = LCD_SSD1306_GPIOPIN_RES;
	g_lcd_gpio_config.gpio_dc.gpioId = LCD_SSD1306_GPIOPIN_DC;
	g_lcd_gpio_config.gpio_cs.gpioId = LCD_SSD1306_GPIOPIN_CS;


	LCD_SSD1306_RESET_DISABLE();
	LCD_SSD1306_DC_COMMAND_ENABLE();


	return LCDD_ERR_NO;
}

PRIVATE LCDD_ERR_T lcdSsd1306pReset(VOID)
{
	LCD_SSD1306_RESET_DISABLE();
	hal_TimDelay(1 MS_WAITING);
	LCD_SSD1306_RESET_ENABLE();
	hal_TimDelay(1 MS_WAITING);
	LCD_SSD1306_RESET_DISABLE();
	hal_TimDelay(1 MS_WAITING);
}


PRIVATE LCDD_ERR_T lcdSsd1306pOpen(VOID)
{
	lcdSsd1306pOpenSpi();
	lcdSsd1306pGpioInit();
	lcdSsd1306pReset();

	return LCDD_ERR_NO;
}
#endif /* SSD1306_USE_GOUDA */

PRIVATE LCDD_ERR_T  lcdSsd1306pInit(VOID)
{
	lcdSsd1306pWriteCmd(0xA8);
	lcdSsd1306pWriteCmd(0x3F);

	lcdSsd1306pWriteCmd(0xD3);
	lcdSsd1306pWriteCmd(0x00);
	
	lcdSsd1306pWriteCmd(0x40);
	
	lcdSsd1306pWriteCmd(0xA0);
	
	lcdSsd1306pWriteCmd(0xC0);
	
	lcdSsd1306pWriteCmd(0xDA);
	lcdSsd1306pWriteCmd(0x12);
	
	lcdSsd1306pWriteCmd(0x81);
	lcdSsd1306pWriteCmd(0x7F);

	lcdSsd1306pWriteCmd(0xA4);

	
	lcdSsd1306pWriteCmd(0xA6);
	
	lcdSsd1306pWriteCmd(0xD5);
	lcdSsd1306pWriteCmd(0x80);
	
	lcdSsd1306pWriteCmd(0x8D);
	lcdSsd1306pWriteCmd(0x14);
	
	lcdSsd1306pWriteCmd(0xAF);
	hal_HstSendEvent(BOOT_EVENT, 0x88880e00);
	return LCDD_ERR_NO;
	
}

PRIVATE VOID lcdSsd1306ClrScreen(VOID)
{
	UINT8 data = 0x0;

	
	lcdSsd1306pWriteCmd(0x20);
	lcdSsd1306pWriteCmd(0x00);

	lcdSsd1306pWriteCmd(0x21);
	lcdSsd1306pWriteCmd(0x00);
	lcdSsd1306pWriteCmd(0x7f);

	lcdSsd1306pWriteCmd(0x22);
	lcdSsd1306pWriteCmd(0x00);
	lcdSsd1306pWriteCmd(0x07);

	for(UINT16 i=0; i<1024; i++)
	{
		lcdSsd1306pWriteData(&data, 1);
	}
}

PRIVATE VOID lcdSsd1306SetScreen(UINT8 *data)
{
	lcdSsd1306pWriteCmd(0x20);
	lcdSsd1306pWriteCmd(0x00);

	lcdSsd1306pWriteCmd(0x21);
	lcdSsd1306pWriteCmd(0x00);
	lcdSsd1306pWriteCmd(0x7f);

	lcdSsd1306pWriteCmd(0x22);
	lcdSsd1306pWriteCmd(0x00);
	lcdSsd1306pWriteCmd(0x07);
	
	lcdSsd1306pWriteData(data, 1024);
}

PUBLIC LCDD_ERR_T lcdd_update_screen(UINT8* Data, LCDD_ROI_T *rect)
{

	
	/*UINT8 buffer[1024];
	UINT16 i;

hal_HstSendEvent(BOOT_EVENT, 0x88880706);
	for(i=0; i<1024; i++)
	{
		Data[i] = 0x57;
	}
	hal_HstSendEvent(BOOT_EVENT, 0x88880707);
	lcdSsd1306SetScreen(Data);

	*/
	lcdSsd1306SetScreen(Data);
	hal_HstSendEvent(BOOT_EVENT, 0x88880708);

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
	//lcdd_SetBrightness(7);
	hal_HstSendEvent(BOOT_EVENT, 0x88880800);

#ifdef SSD1306_USE_GOUDA
    pmd_SetLevel(1,6);
#endif /* SSD1306_USE_GOUDA */

	lcdSsd1306pOpen();

	COS_Sleep(20 );
	lcdd_Init();
	g_lcddInSleep = FALSE;

	lcdSsd1306ClrScreen();
	return LCDD_ERR_NO;
}

PUBLIC LCDD_ERR_T lcdd_Init(VOID)
{
    return lcdSsd1306pInit();
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
#ifdef SSD1306_USE_GOUDA
	hal_GoudaClose();
#endif /* SSD1306_USE_GOUDA */
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


