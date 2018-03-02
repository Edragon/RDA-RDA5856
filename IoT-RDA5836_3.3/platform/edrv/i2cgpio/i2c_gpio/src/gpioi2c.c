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

/*************************************************************************
 *
 * MODULE NAME:    GPIO_based_I2C.c
 * PROJECT CODE:   rda
 * DESCRIPTION:    rda iic drv.
 * MAINTAINER:     cj
 * CREATION DATE:  16/04/10
 *
 * SOURCE CONTROL: $Id: gpioi2c.c 7713 2011-05-20 09:43:10Z jiancui $
 *
 *
 * NOTES TO USERS:
 *   None.
 *
 *************************************************************************/

#include "cs_types.h"
#include "i2cgpio_config.h"
#include "tgt_gpioi2c_cfg.h"
#include "i2cgpio_debug.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_timers.h"
#ifndef MCU_WIFI_PLAT
#include "sxr_tim.h"
#include "sxr_tls.h"
#endif
//#include "cmn_defs.h"
#include "gpio_i2c.h"

#define GPIO_based_I2C      1
#define GPIO_I2C_SIM_160K 0

PRIVATE UINT8 g_i2c_multiple = 1;
PRIVATE UINT8 g_i2c_open = FALSE;


#ifndef MCU_WIFI_PLAT
#define DURATION_INIT_1 	20//600ns 
#define DURATION_INIT_2	    20//600ns 
#define DURATION_INIT_3 	20//600ns 

#define DURATION_START_1	20//600ns 
#define DURATION_START_2	24//600ns 
#define DURATION_START_3	24//800ns 

#define DURATION_STOP_1	24//800ns 
#define DURATION_STOP_2	20//600ns 
#define DURATION_STOP_3	50//1300ns

#define DURATION_HIGH		40//900ns 
#define DURATION_LOW		56//1600ns

#define DELAY(DURATION)		{volatile UINT32 j; for(j = DURATION * g_i2c_multiple ; j >0; j--){}}

#else

#define DURATION_INIT_1 	48//600ns
#define DURATION_INIT_2	    48//600ns
#define DURATION_INIT_3 	48//600ns

#define DURATION_START_1	48//600ns
#define DURATION_START_2	48//600ns
#define DURATION_START_3	64//800ns

#define DURATION_STOP_1	64//800ns
#define DURATION_STOP_2	48//600ns
#define DURATION_STOP_3	104//1300ns

#define DURATION_HIGH		100//900ns
#define DURATION_LOW		100//1600ns

#define DELAY(DURATION)		{volatile UINT32 j; for(j = DURATION ; j >0; j--){}}
#endif


VOID DRV_GpioSetDirection_iic( UINT8 nGpio, UINT8 nDirection)
{
#ifndef MCU_WIFI_PLAT  //liyanshui removed, why ?
	if ((nGpio&0x1) == 0x1)  //gpo
	{
		;
	}
	else
#endif
	{
		HAL_GPIO_GPIO_ID_T gpio_0;
		#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
			gpio_0 = HAL_GPIO_0;
		#else
			gpio_0 = HAL_GPIO_0_0;
		#endif

		if (nDirection == 1)
		{
			hal_GpioSetOut(nGpio);
		}
		else
		{
			hal_GpioSetIn(nGpio);
		}
	}
}

VOID DRV_GpioSetLevel_iic(UINT8 nGpio,UINT8 nLevel)
{
#ifndef MCU_WIFI_PLAT
  if ((nGpio&0x1) == 0x1)  //gpo
    {
        HAL_APO_ID_T gpio;
        nGpio = nGpio >> 1;

#if 0
        gpio.val = {
                                .type = HAL_GPIO_TYPE_O,
                                .id = nGpio
                            };
#endif
        gpio.val.type = HAL_GPIO_TYPE_O;
        gpio.val.id = nGpio;
        if (nLevel == 1)
        {
            hal_GpioSet(gpio);
        }
        else
        {
            hal_GpioClr(gpio);
        }
    }
    else
#endif
    {

    	HAL_APO_ID_T gpio;
        //nGpio = nGpio >> 1;  // why ??
#ifndef MCU_WIFI_PLAT
        HAL_APO_ID_T gpio = {{ 
                                .type = HAL_GPIO_TYPE_IO,
                                .id = nGpio
                            }};
#else
        gpio.val.type =  HAL_GPIO_TYPE_IO;
        gpio.val.id = nGpio;
#endif
        if (nLevel == 1)
        {
            hal_GpioSet(gpio);
        }
        else
        {
            hal_GpioClr(gpio);
        }
    }
}
VOID DRV_GpioGetLevel_iic(UINT8 nGpio,UINT8* nLevel)
{
#ifndef MCU_WIFI_PLAT
	if ((nGpio&0x1) == 0x1)  //gpo
	{
		;
	}
	else
	{

		HAL_GPIO_GPIO_ID_T gpio_0;
		#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
		gpio_0 = HAL_GPIO_0;
		#else
		gpio_0 = HAL_GPIO_0_0;
		#endif

		// nGpio = nGpio >> 1;
		*nLevel = hal_GpioGet(nGpio);
	}
#else
	// nGpio = nGpio >> 1;
	*nLevel = hal_GpioGet(nGpio);
#endif
}

// Global variant

PRIVATE UINT32 SCL_PIN,SDA_PIN;
PRIVATE UINT8 level = 0;


PRIVATE VOID GPIO_InitIO(UINT8 direction, UINT8 port)
{
    DRV_GpioSetDirection_iic(port,direction);
}

PRIVATE VOID GPIO_WriteIO(UINT8 data, UINT8 port)
{
    DRV_GpioSetLevel_iic(port,data);
}
PRIVATE UINT8 GPIO_ReadIO(UINT8 port)
{
    DRV_GpioGetLevel_iic(port,&level);
    return level;
}


//
//-------------------------------------------------------------------
// Function:  gpio_i2c_init
// Purpose:  This function is used to init I2C port of the  device   
// In: 
// Return:      bool
//-------------------------------------------------------------------

CONST TGT_GPIOI2C_CONFIG_T*  g_gi2cgpioConfig;

BOOL gpio_i2c_initialize(VOID)
{
	//SXS_TRACE(TSTDOUT,"init I2C ports... \r\n");
	//u32 iGpioSetting = hal_board_config.Used_Gpio;

	byte i = 0;

	g_gi2cgpioConfig = tgt_GetGpioi2cConfig();
	if (g_gi2cgpioConfig -> i2c_gpio_Bps == GPIO_I2C_BPS_160K)
	{
	//g_i2c_multiple = 1;
	}
	if (g_gi2cgpioConfig->scl_i2c_gpio == HAL_GPIO_NONE)
	{
		SCL_PIN = (g_gi2cgpioConfig->scl_i2c_gpo)&0xffff;
		SDA_PIN = (g_gi2cgpioConfig->sda_i2c)&0xffff;
		// SCL_PIN = (SCL_PIN << 1) + 1;
		// SDA_PIN = (SDA_PIN << 1) + 0;
	}
	else
	{
		SCL_PIN = (g_gi2cgpioConfig->scl_i2c_gpio)&0xffff;
		SDA_PIN = (g_gi2cgpioConfig->sda_i2c)&0xffff;
		//SCL_PIN = (SCL_PIN << 1) + 0;
		//SDA_PIN = (SDA_PIN << 1) + 0;
	}
	// GI2C_TRACE(GI2C_INFO_TRC, 0, "gss:  gpio_i2c_init_gsensor scl = %d ,  sda = %d   ",SCL_PIN,SDA_PIN);
	//printf("gss:  gpio_i2c_init_gsensor scl = %d ,  sda = %d   ",SCL_PIN,SDA_PIN);
	//iTemp = SCL_PIN | SDA_PIN ;
	// Set the GPIO pin to output status
	DRV_GpioSetDirection_iic(SCL_PIN,1);
	DRV_GpioSetDirection_iic(SDA_PIN,1);
	DELAY(DURATION_INIT_1);

	// Make the I2C bus in idle status
	DRV_GpioSetLevel_iic(SCL_PIN,1);
	DELAY(DURATION_INIT_1);
	DRV_GpioSetLevel_iic(SDA_PIN,1);
	DELAY(DURATION_INIT_1);

#if 1
	for( i=0;i<30;i++)
	{
		GPIO_WriteIO(0, SCL_PIN); /* low */
		DELAY(DURATION_LOW);   
		GPIO_WriteIO(1, SCL_PIN);
		DELAY(DURATION_HIGH);
	}
#endif

	return (TRUE);
}

PRIVATE VOID gpio_i2c_Start(VOID) /* start or re-start */
{
#ifndef MCU_WIFI_PLAT
   GPIO_InitIO(1,SDA_PIN);

   DELAY(DURATION_START_1);
   //COS_Sleep(1000);

   GPIO_WriteIO(1,SDA_PIN);
   DELAY(DURATION_START_1);
   GPIO_WriteIO(1,SCL_PIN);
  

   //COS_Sleep(1000);

   DELAY(DURATION_START_1);
   GPIO_WriteIO(0,SDA_PIN);   
   DELAY(DURATION_START_2);
   GPIO_WriteIO(0,SCL_PIN);

   DELAY(DURATION_START_3);/* start condition */
#else
   GPIO_InitIO(1,SDA_PIN);
   GPIO_InitIO(1,SCL_PIN);

   GPIO_WriteIO(1,SDA_PIN);
   DELAY(DURATION_LOW);
   GPIO_WriteIO(1,SCL_PIN);
  
   DELAY(DURATION_LOW);
   GPIO_WriteIO(0,SDA_PIN);   
   DELAY(DURATION_LOW);
   GPIO_WriteIO(0,SCL_PIN);

   DELAY(DURATION_LOW);/* start condition */
#endif
}

PRIVATE VOID gpio_i2c_Stop(VOID)
{
   GPIO_WriteIO(0,SCL_PIN);
   DELAY(DURATION_LOW);
   GPIO_InitIO(1,SDA_PIN);
   GPIO_WriteIO(0,SDA_PIN);
   DELAY(DURATION_STOP_1);
   GPIO_WriteIO(1,SCL_PIN);
   DELAY(DURATION_STOP_2);
   GPIO_WriteIO(1,SDA_PIN); /* stop condition */
   DELAY(DURATION_STOP_3);
}

PRIVATE UINT8 gpio_i2c_TxByte(UINT8 data) /* return 0 --> ack */
{
#ifndef MCU_WIFI_PLAT
	int32 i;
	uint8 temp_value = 0;
	for(i=7; (i>=0)&&(i<=7); i--)
	{
		GPIO_WriteIO( 0, SCL_PIN); /* low */
		DELAY(DURATION_LOW);
		if(i==7)GPIO_InitIO(1,SDA_PIN);
		DELAY(DURATION_LOW);

		GPIO_WriteIO(((data>>i)&0x01), SDA_PIN);
		DELAY(DURATION_LOW/2);
		GPIO_WriteIO( 1, SCL_PIN); /* high */
		DELAY(DURATION_HIGH);
	}
	GPIO_WriteIO(0, SCL_PIN); /* low */
	DELAY(DURATION_LOW);
	GPIO_InitIO(0,SDA_PIN);/* input  */
	DELAY(DURATION_LOW/2);
	GPIO_WriteIO(1, SCL_PIN); /* high */
	DELAY(DURATION_HIGH);
	temp_value = GPIO_ReadIO(SDA_PIN);
	GPIO_WriteIO(0, SCL_PIN); /* low */
	DELAY(DURATION_LOW);
	return temp_value;
#else
	int32 i;
	uint8 temp_value = 0;
    GPIO_InitIO(1,SDA_PIN);
    GPIO_InitIO(1,SCL_PIN);
	for(i=7; (i>=0)&&(i<=7); i--)
	{
		GPIO_WriteIO( 0, SCL_PIN); /* low */
		DELAY(DURATION_LOW);

		GPIO_WriteIO(((data>>i)&0x01), SDA_PIN);
		DELAY(DURATION_LOW);
		GPIO_WriteIO( 1, SCL_PIN); /* high */
		DELAY(DURATION_HIGH);
	}
	GPIO_WriteIO(0, SCL_PIN); /* low */
	GPIO_InitIO(0,SDA_PIN);/* input  */
	DELAY(DURATION_LOW*4);
	GPIO_WriteIO(1, SCL_PIN); /* high */
	//DELAY(DURATION_HIGH);
	temp_value = GPIO_ReadIO(SDA_PIN);
    DELAY(DURATION_LOW);
	GPIO_WriteIO(0, SCL_PIN); /* low */
	return temp_value;
#endif
}

PRIVATE VOID gpio_i2c_RxByte(uint8 *data, uint8 ack)
{
	int32 i;
	uint32 dataCache;

	dataCache = 0;
	for(i=7; (i>=0)&&(i<=7); i--)
	{
		GPIO_WriteIO(0, SCL_PIN);
		DELAY(DURATION_LOW);
		if(i==7)GPIO_InitIO(0,SDA_PIN);
		DELAY(DURATION_LOW);
		GPIO_WriteIO(1, SCL_PIN);
		DELAY(DURATION_HIGH);
		dataCache |= (GPIO_ReadIO(SDA_PIN)<<i);
		DELAY(DURATION_LOW/2);
	}

	GPIO_WriteIO(0, SCL_PIN);
	DELAY(DURATION_LOW);
	GPIO_InitIO(1,SDA_PIN);
	GPIO_WriteIO(ack, SDA_PIN);
	DELAY(DURATION_LOW/2);
	GPIO_WriteIO(1, SCL_PIN);
	DELAY(DURATION_HIGH);
	GPIO_WriteIO(0, SCL_PIN); /* low */
	DELAY(DURATION_LOW);
	*data = (uint8)dataCache;
}


VOID i2c_test(void)
{
	//gpio_i2c_Start();///start
	//gpio_i2c_TxByte(0x23);//chip adress
	//DELAY(DURATION_LOW*100);
    UINT8 temp_value;
	GPIO_InitIO(0,SDA_PIN);
    temp_value = GPIO_ReadIO(SDA_PIN);
     printf("i2c_test  temp_value=%d\n",temp_value);

 //   GPIO_InitIO(1,SCL_PIN);
}

VOID gpio_i2c_write_data(UINT32 addr,UINT8 * regaddr,UINT8 addrlen,UINT8 *data, UINT8 datalen)
{
	UINT8 i;
	if (SCL_PIN == 0)
	{
    GI2C_ASSERT(0, "initial the gpio_i2c firstly.", "xx");
    printf("inital gpio_i2c firstly.\n");
	}
	gpio_i2c_Start();///start
	gpio_i2c_TxByte(addr<<1);//chip adress


	for(i=addrlen;i>0;i--,regaddr++)//addr
	{
		gpio_i2c_TxByte(*regaddr);
	}


	for( i=datalen;i>0;i--,data++)//data
	{
		gpio_i2c_TxByte(*data);
	}
	gpio_i2c_Stop();
}


UINT8 gpio_i2c_read_data(UINT32 addr,UINT8 * regaddr, UINT8 addrlen,UINT8 *datas, UINT8 datalen)
{
	UINT8 tempdata;
	UINT8 i;
	UINT8 *data = datas;
	
	if(addrlen)
	{
		gpio_i2c_Start();///start
		gpio_i2c_TxByte(addr<<1);//chip adress

		for(i=addrlen;i>0;i--,regaddr++)//addr
		{
			gpio_i2c_TxByte(*regaddr);
		}
	}
	
	gpio_i2c_Start();//start
	gpio_i2c_TxByte((addr<<1)+1);//chip adress
	
	for( i=datalen-1;i>0;i--,data++)//data
	{
		gpio_i2c_RxByte(&tempdata, 0);
		*data = tempdata;	
		GI2C_TRACE(GI2C_INFO_TRC, 0, "gss: gpio read 1*datas 0x%x",tempdata);
	}
	
	gpio_i2c_RxByte(&tempdata, 1);
	*data = tempdata;	
	
	gpio_i2c_Stop();
	GI2C_TRACE(GI2C_INFO_TRC, 0, "gss: gpio read end *datas 0x%x",*data);
	return *datas;
}

void gpio_i2c_rdabt_rf_write_data(UINT32 addr,unsigned char regaddr, unsigned short *data, unsigned char datalen)
{
	unsigned char i=0;
	
	gpio_i2c_Start();///start
	gpio_i2c_TxByte(addr);//chip adress
//	hal_HstSendEvent(gpio_i2c_TxByte(addr)|0xdd110000);//chip adress
	gpio_i2c_TxByte(regaddr);

	for(i=0;i<datalen;i++,data++)//data
	{
		gpio_i2c_TxByte(*data>>8);
		gpio_i2c_TxByte(*data);
	}
	gpio_i2c_Stop();
}

void gpio_i2c_rdabt_rf_read_data(UINT32 addr,unsigned char regaddr, unsigned short *data, unsigned char datalen)
{
	unsigned char tempdata;
	unsigned char i=0;
	
	gpio_i2c_Start();///start
	gpio_i2c_TxByte(addr);//chip adress
//	hal_HstSendEvent(gpio_i2c_TxByte(addr)|0xdd220000);//chip adress
	gpio_i2c_TxByte(regaddr);
	
	gpio_i2c_Start();//start
	gpio_i2c_TxByte(addr+1);//chip adress
//	hal_HstSendEvent(gpio_i2c_TxByte(addr+1)|0xdd330000);//chip adress
	for( i=0;i<datalen-1;i++,data++)//data
	{
		gpio_i2c_RxByte(&tempdata, 0);
		*data = (tempdata<<8);	
		gpio_i2c_RxByte(&tempdata, 0);			
		*data |= tempdata;		
	}
	
	gpio_i2c_RxByte(&tempdata, 0);
	*data = (tempdata<<8);	
	gpio_i2c_RxByte(&tempdata, 1);			
	*data |= tempdata;	
	
	gpio_i2c_Stop();
}

void gpio_i2c_rdabt_core_write_data(UINT32 addr,unsigned int regaddr, unsigned int *data, unsigned char datalen)
{
	unsigned char i=0;    
	gpio_i2c_Start();///start
	gpio_i2c_TxByte(addr);//chip adress
//	hal_HstSendEvent(gpio_i2c_TxByte(addr)|0xd440000);//chip adress
	
	gpio_i2c_TxByte(regaddr>>24);/////reg adress
	gpio_i2c_TxByte(regaddr>>16);
	gpio_i2c_TxByte(regaddr>>8);	
	gpio_i2c_TxByte(regaddr);
	gpio_i2c_Start();///start
	gpio_i2c_TxByte(addr);//chip adress
	for(i=0;i<datalen;i++,data++)//data
	{
		gpio_i2c_TxByte(*data>>24);
		gpio_i2c_TxByte(*data>>16);
		gpio_i2c_TxByte(*data>>8);
		gpio_i2c_TxByte(*data);
	}
	gpio_i2c_Stop();
}

void gpio_i2c_rdabt_core_read_data(UINT32 addr,unsigned int regaddr, unsigned int *data, unsigned char datalen)
{
	unsigned char tempdata;
	unsigned char i=0;
	
	gpio_i2c_Start();///start
	gpio_i2c_TxByte(addr);//chip adress
//	hal_HstSendEvent(gpio_i2c_TxByte(addr)|0xdd550000);//chip adress
	
	gpio_i2c_TxByte(regaddr>>24);
	gpio_i2c_TxByte(regaddr>>16);
	gpio_i2c_TxByte(regaddr>>8);
	gpio_i2c_TxByte(regaddr);
	
	gpio_i2c_Start();//start
	gpio_i2c_TxByte(addr+1);//chip adress
//	hal_HstSendEvent(gpio_i2c_TxByte(addr+1)|0xdd660000);//chip adress
	for( i=0;i<datalen-1;i++,data++)//data
	{
		gpio_i2c_RxByte(&tempdata, 0);
		*data  = (tempdata<<24);	
		gpio_i2c_RxByte(&tempdata, 0);			
		*data |= (tempdata<<16);				
		gpio_i2c_RxByte(&tempdata, 0);
		*data |= (tempdata<<8);	
		gpio_i2c_RxByte(&tempdata, 0);			
		*data |= tempdata;		
	}
	gpio_i2c_RxByte(&tempdata, 0);
	*data = (tempdata<<24);	
	gpio_i2c_RxByte(&tempdata, 0);			
	*data |= (tempdata<<16);		
	gpio_i2c_RxByte(&tempdata, 0);
	*data |= (tempdata<<8);	
	gpio_i2c_RxByte(&tempdata, 1);			
	*data |= tempdata;	
	
	gpio_i2c_Stop();
}


BOOL gpio_i2c_open(VOID)
{
    static BOOL i2cInitDone = FALSE;
    
    UINT32 scStatus;
    if (g_i2c_open == TRUE)
    {
        return FALSE;
    }
    scStatus = hal_SysEnterCriticalSection();
    g_i2c_open = TRUE;
    hal_SysExitCriticalSection(scStatus);

    if (!i2cInitDone)
    {
        gpio_i2c_initialize();
        i2cInitDone = TRUE;
    }
    
    return TRUE;
}

BOOL gpio_i2c_close(VOID)
{
    UINT32 scStatus;
    if (g_i2c_open == FALSE)
    {
        return FALSE;
    }
    scStatus = hal_SysEnterCriticalSection();
    g_i2c_open = FALSE;
    hal_SysExitCriticalSection(scStatus);
    return TRUE;
}



