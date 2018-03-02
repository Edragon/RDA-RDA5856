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

//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL:                                                                 //
//  $Author: admin $                                                       //
//  $Date:                                                                    //
//  $Revision:                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///                                                                           //
///     @file tsd.c                                                           //
///                                                                           //
///     Touch panel driver                                                    //
///     opal                                                                  //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 


#include "sxr_ops.h"
#include "sxs_io.h"
#include "cs_types.h"
#include "sxr_tls.h"
#include "tsdp_debug.h"
#include "tsd_config.h"
#include "tsd_m.h"
//#include "tgt_tsd_cfg.h"
#include "tsdp_calib.h"
#include "opal.h"
#include "dsm_dev_driver.h"

#include "hal_host.h"
#include "hal_ana_gpadc.h"
#include "assert.h"
#include "cos.h"
#include "event.h"
#include "pmd_m.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#include "register.h"
#endif
// ============================================================================
// LOCAL DEFINITIONS
// ============================================================================
//#define TSD_DEBUG

// Number of samples we ask the touch screen controller for each coordinate.
// We need more than one because of the touch screen settling time.
// Indeed, the voltage across the touch panel "ring" and then settle down due
// to mechanical bouncing and switch driver parasiting.
// From 1 to 4 : 4 is obviously the more precise.
// well 4 is too long, settling time of switch is not relevant with opal, as opal
// switch between each measure, we keep 3 here to have some filtering, but 1 would
// probably be nice enough for Opal tsd, as it seams fairly stable !
// Security would choose 3
// According to the test, we choose 3 now. You may easily find unstable pen tracking during
// the pen keeping pressing on the same position, especially while handwriting.
#define NUMBER_SAMPLES_PER_POINT 3


// With 12-bit Conversion, keeping the tenth most significant bits.
#define MAX_ADC_VAL    0x3ff

// Flag to indicate if the tsd_GetRawPoints succeeded or not.
#define SUCCESSFUL   1
#define NOT_SUCCESSFUL   0
#define SAMPLE_VALID 3


#define READ_X_AXIS  1
#define READ_Y_AXIS  2


// Parameter of sxr_StartFunctionTimer
#ifdef filter_error_key
#define TSD_DEBOUNCE_TIME (HAL_TICK1S/1000)
#define TSD_REPITITION_TIME (1*(TSD_DEBOUNCE_TIME))
#else
#define TSD_DEBOUNCE_TIME (HAL_TICK1S/1000)
#define TSD_REPITITION_TIME (5*(TSD_DEBOUNCE_TIME))
#endif

// TSD key stuff
#define TSD_KEY_NUMBER 20
#define TSD_KEY_FILTER_COUNT 3
extern UINT32 key_enterInterrupt;

PUBLIC VOID tsd_Debounce(VOID);
extern HAL_ANA_GPADC_MV_T hal_AnaGpadcGpadc2Volt(UINT16 gpadcVal);


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// TSD config
PRIVATE CONST TSD_CONFIG_T*  g_tsdConfig;

// HAL GPIO CFG
//PRIVATE HAL_GPIO_CFG_T       g_gpioCfg;

// Tab of NUMBER_SAMPLES_PER_POINT TSC_POINT samples.
//PRIVATE TSD_POINT_T          g_tabSamples[NUMBER_SAMPLES_PER_POINT];

// This variable is a counter used in #debounce() function to count how many 
// period of g_tsdConfig->debounceTime msec the pen state hold the same value.
PRIVATE UINT8                g_count;

// This variable stores the previous pen state as it has been detected by the 
// #debounce() function and is used to detect a change in the pen state.
// When 0, the pen is considered not touching the screen. 
// When 1, the pen is considered touching the screen.
PRIVATE UINT32 g_previousPenIrqPinState = TSD_EOM_UP;

PRIVATE TSD_CALLBACK_T          g_userCallback = NULL;

PRIVATE TSD_REPORTING_MODE_T    g_userReportingMode = { FALSE, FALSE, FALSE, 0};

PRIVATE TSD_PEN_STATE_T         g_penState = TSD_EOM_UP;

PRIVATE TSD_PEN_STATE_T         g_lastSendPenState = TSD_EOM_UP;


#ifdef _USED_TSC_KEY_

PRIVATE TSD_KEY_CALLBACK_T g_userKeyCallback = NULL;

PRIVATE TSD_USER_DATA_T *g_pTSUserData;

CONST PRIVATE HAL_ANA_GPADC_MV_T gc_tsdKeyVolt[TSD_KEY_NUMBER] = 
{
    107, 194, 278, 363, 446, 535, 615, 701,
    790, 877, 960, 1044, 1128, 1213, 1296, 1385,
    1473, 1554, 1640, 1725, 
};

PRIVATE UINT8 g_tsdKeyFiltIndex = 0;
//PRIVATE UINT8 g_tsdKeyFiltKeys[TSD_KEY_FILTER_COUNT];

#else

PRIVATE TSD_POINT_T          g_tabCalibratedExpectedPoint [3];

PRIVATE TSD_POINT_T          g_tabCalibratedMeasuredPoint [3];

PRIVATE TSD_MATRIX_T         g_calibrationMatrix;

PRIVATE BOOL                 g_isCalibSuccess = TRUE;

PRIVATE UINT32               g_isCalibrationDone = 0;

#endif

PUBLIC BOOL pmd_RegRead(RDA_REG_MAP_T regIdx, UINT32 *pData);

// =============================================================================
//  FUNCTIONS
// =============================================================================

#ifdef _USED_TSC_KEY_

U32  global_last_key_vol=0;
BOOL g_tsk_calib_flag;
INT32 tsd_GetKeyIndex(HAL_ANA_GPADC_MV_T volt)
{
    UINT16 index;
	#if 0
    float step = (g_pTSUserData->maxVol-g_pTSUserData->minVol)/(g_tsdConfig->keyCount-1);
    float value = ((float)volt-g_pTSUserData->minVol)/step;
	#endif 
    float step = (g_tsdConfig->maxVolt-g_tsdConfig->minVolt)/(g_tsdConfig->keyCount-1);
    float value = ((float)volt-g_tsdConfig->minVolt)/step;   
	
    global_last_key_vol = volt;
    if(g_tsk_calib_flag)
        return 0;
    
    index = (UINT16)(value+0.5);

    EDRV_TRACE(TSTDOUT, 0, "tsd_GetKeyIndex volt=%d,index=%d,value=%d", volt, index,(int)(value*100.0));

    if(index>=g_tsdConfig->keyCount)// || (value-(float)index)>=0.3 || ((float)index-value)>=0.3)
        return g_tsdConfig->keyCount -1;
    else
        return index;
}

#if 0
PRIVATE UINT32 tsd_FilterKey(UINT32 key)
{
    UINT32 ret = NOT_SUCCESSFUL;
    UINT32 i;

    if (g_tsdKeyFiltIndex < TSD_KEY_FILTER_COUNT)
    {
        if (g_tsdKeyFiltIndex == TSD_KEY_FILTER_COUNT-1)
        {
            g_tsdKeyFiltIndex = 0;
            for (i=0; i<TSD_KEY_FILTER_COUNT-1; i++)
            {
                if (g_tsdKeyFiltKeys[i] != (UINT8)key)
                {
                    break;
                }
            }
            if (i == TSD_KEY_FILTER_COUNT-1)
            {
                ret = SUCCESSFUL;
            }
            else
            {
                EDRV_TRACE(TSTDOUT, 0, "tsd_FilterKey: Key index filter failed");
            }
        }
        else
        {
            g_tsdKeyFiltKeys[g_tsdKeyFiltIndex++] = (UINT8)key;
        }
    }

    return ret;
}
#endif

PRIVATE VOID tsd_KeyCallback(TSD_POINT_T *pointTouched,TSD_PEN_STATE_T status)
{
    if (g_userKeyCallback == NULL)
    {
        return;
    }

	(*g_userKeyCallback)(pointTouched->x, status);
}

#endif // _USED_TSC_KEY_


// ============================================================================
//  tsd_DisableHostPenIrq
// ----------------------------------------------------------------------------
/// This function disables host pen irq. 
// ============================================================================
VOID tsd_DisableHostPenIrq(VOID)
{
    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_DisableHostPenIrq");
    // actually stop irq
//    g_gpioCfg.irqMask.rising= FALSE;
//    hal_GpioIrqSetMask(g_tsdConfig->penGpio, &g_gpioCfg.irqMask);
}



// ============================================================================
//  tsd_ClearTouchIrq
// ----------------------------------------------------------------------------
/// This function enables host pen irq. 
// ============================================================================
VOID tsd_ClearTouchIrq(VOID)
{
    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_ClearTouchIrq");
    pmd_TsdClearTouchIrq();
}

// ============================================================================
//  tsd_EnableHostPenIrq
// ----------------------------------------------------------------------------
/// This function enables host pen irq. 
// ============================================================================
VOID tsd_EnableHostPenIrq(VOID)
{

    if (FALSE == pmd_ResetTouch())
    {
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_EnableHostPenIrq,(VOID*)NULL,0x03);
        return;

    }

    // filter out previous in queue Irq
    hal_GpioResetIrq(g_tsdConfig->penGpio);
	
//    g_gpioCfg.irqMask.rising = TRUE;
 //   g_gpioCfg.irqMask.level= TRUE;//for 5855
//    hal_GpioIrqSetMask(g_tsdConfig->penGpio, &g_gpioCfg.irqMask);
}


// ============================================================================
//  tsd_CoordFromMode
// ----------------------------------------------------------------------------
/// This function returns the &coordinate corresponding to the mode_xy
// ============================================================================
PRIVATE inline UINT32* tsd_CoordFromMode(TSD_POINT_T* sample,UINT8 mode_xy)
{
    switch(mode_xy)
    {
        case READ_X_AXIS :
            return &sample->x;
            break;
        case READ_Y_AXIS:
            return &sample->y;
            break;
        default:
            break;
    }
    return((UINT32*)1);
}




// ============================================================================
//  tsd_GetSamples
// ----------------------------------------------------------------------------
// ============================================================================
#if 0
PRIVATE TSD_ERR_T tsd_GetSamples(TSD_POINT_T* TabSamples)
{   
    //EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd:  tsd_GetSamples ");

    UINT16 i,k;
    UINT16 xy[2];
    BOOL result;
    TSD_PROFILE_FUNCTION_ENTRY(tsd_GetSamples);

#if (!CHIP_HAS_ASYNC_TCU)
    if(FALSE == pmd_TSDGetSPI())
    {
        TSD_PROFILE_FUNCTION_EXIT(tsd_GetSamples);
        return TSD_ERR_RESOURCE_BUSY;
    }
#endif

    for(i = 0;i < NUMBER_SAMPLES_PER_POINT;i++)
    {
        k=0;
        do{
            result = pmd_TsdReadCoordinatesInternal(&xy[0],&xy[1]);
            if(result)
            {
                break;
            }
            hal_SysWaitMicrosecond(30);
        } while (k++ < 10);

        if(result == FALSE)
        {
            break;
        }
        EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_GetSamples:[%d] x=0x%04x,y=0x%04X",i,xy[0],xy[1]);
        pmd_TsdClearEomIrqInternal();
        *(tsd_CoordFromMode(&TabSamples[i], READ_X_AXIS)) = xy[0];
        *(tsd_CoordFromMode(&TabSamples[i], READ_Y_AXIS)) = xy[1];
    }

    hal_TimDelay(1);
    pmd_TsdClearTouchIrq();

#if (!CHIP_HAS_ASYNC_TCU)
    pmd_TSDReleaseSPI();
#endif

    TSD_PROFILE_FUNCTION_EXIT(tsd_GetSamples);

    if(result == FALSE)
        return TSD_ERR_RESOURCE_BUSY;

    return TSD_ERR_NO;
}

// ============================================================================
//  tsd_EvaluateSamples
// ----------------------------------------------------------------------------
/// Each time,
/// on a pen down, NUMBER_SAMPLES_PER_POINT pairs XY from the ADC are collected.
/// NUMBER_SAMPLES_PER_POINT=4 :
/// The first sample is discarding (less time of acquisition) and the 3 others 
/// will be used here. This function implements the algorithm to the best sample 
/// from 3 pairs of samples by discarding one that is too way out and take a mean
/// of the rest two.
/// NUMBER_SAMPLES_PER_POINT=3 :
/// The same thing but we take in consideration the first sample.
/// NUMBER_SAMPLES_PER_POINT=2 :
/// Average of the two samples.
/// NUMBER_SAMPLES_PER_POINT=1 :
/// No evaluation here, we take the keep the only sample we have.
///
/// @param TabSamples the sampled data
/// @param sample where to fill an evaluation of the sample(s).
/// @param mode_xy selects the coordinate on wich we apply the evaluation
/// @return \c TRUE if the evaluation is valid

/// Evaluate both samples, 4 X's and 4 Y's.
/// Use global var g_samplesX, g_samplesY instead mode_xy
// ============================================================================
PRIVATE BOOL tsd_EvaluateSamples (TSD_POINT_T* TabSamples,UINT32 *sample,UINT8 mode_xy)
{
    UINT16 val0, val1, val2, diff0, diff1, diff2;
    BOOL retval = FALSE;

    TSD_PROFILE_FUNCTION_ENTRY(tsd_EvaluateSamples);

    switch(NUMBER_SAMPLES_PER_POINT)
    {
        case 4:

            // We discard the first sample : The one with the less time of acquisition. 
            val0=*tsd_CoordFromMode(&TabSamples[1],mode_xy);
            val1=*tsd_CoordFromMode(&TabSamples[2],mode_xy);
            val2=*tsd_CoordFromMode(&TabSamples[3],mode_xy);

            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val1 <= MAX_ADC_VAL) && (val2 <= MAX_ADC_VAL) 
                    && (val0 > 0) && (val1 > 0) && (val2 > 0))
            {
                // Calculate the absolute value of the differences of the samples.
                diff0 = (val0> val1)  ? (val0 -val1) : (val1 - val0);
                diff1 = (val1> val2)  ? (val1 -val2) : (val2 - val1);
                diff2 = (val2> val0)  ? (val2 -val0) : (val0 - val2);

                // We estimate the average valid if the difference between two samples are less than g_tsdConfig->maxError.
                if ((diff0 < g_tsdConfig->maxError) && (diff1 < g_tsdConfig->maxError) && (diff2 < g_tsdConfig->maxError))
                {
                    retval = TRUE;
                    // Eliminate the one away from other two and add the two others.
                    if (diff0 < diff1)
                    {
                        *sample=(UINT16)(val0 + ((diff2 < diff0) ? val2 : val1));
                    }
                    else
                    {
                        *sample=(UINT16)(val2 + ((diff2 < diff1) ? val0 : val1));
                    }
                    // Get the average of the two good samples.
                    *sample>>=1;
                } 
                else
                {   
                    EDRV_TRACE(TSTDOUT, 0, "Samples not valid (>= maxError: 0x%X)", g_tsdConfig->maxError);
                }

            }    
            break;
            
        case 3:
            
            // We take in consideration all samples. 
            val0=*tsd_CoordFromMode(&TabSamples[0],mode_xy);
            val1=*tsd_CoordFromMode(&TabSamples[1],mode_xy);
            val2=*tsd_CoordFromMode(&TabSamples[2],mode_xy);

            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val1 <= MAX_ADC_VAL) && (val2 <= MAX_ADC_VAL)
                    && (val0 > 0) && (val1 > 0) && (val2 > 0))
            {
                // Calculate the absolute value of the differences of the samples.
                diff0 = (val0> val1)  ? (val0 -val1) : (val1 - val0);
                diff1 = (val1> val2)  ? (val1 -val2) : (val2 - val1);
                diff2 = (val2> val0)  ? (val2 -val0) : (val0 - val2);

                // We estimate the average valid if the difference between two samples are less than g_tsdConfig->maxError.
                if ((diff0 < g_tsdConfig->maxError) && (diff1 < g_tsdConfig->maxError) && (diff2 < g_tsdConfig->maxError))
                {
                    retval = TRUE;
                    // Eliminate the one away from other two and add the two others.
                    if (diff0 < diff1) 
                    {    
                        *sample=(UINT16)(val0 + ((diff2 < diff0) ? val2 : val1));
                    }
                    else
                    {
                        *sample=(UINT16)(val2 + ((diff2 < diff1) ? val0 : val1));
                    }
                    // Get the average of the two good samples.
                    *sample>>=1;
                }
                else
                {   
                    EDRV_TRACE(TSTDOUT, 0, "Samples not valid with g_tsdConfig->maxError");
                }

            }    
            break;

        case 2:
            
            // We take in consideration all samples. 
            val0=*tsd_CoordFromMode(&TabSamples[0],mode_xy);
            val1=*tsd_CoordFromMode(&TabSamples[1],mode_xy);

            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val1 <= MAX_ADC_VAL)
                    && (val0 > 0) && (val1 > 0))
            {
                // Calculate the absolute value of the differences of the samples.
                diff0 = (val0> val1)  ? (val0 -val1) : (val1 - val0);
                
                // We estimate the average valid if the difference between two samples are less than g_tsdConfig->maxError.
                if (diff0 < g_tsdConfig->maxError)
                {
                    retval = TRUE;
                    *sample=(UINT16)(val0 + val1);
                    // Get the average of the two good samples.
                    *sample>>=1;
                }
                else
                {   
                    EDRV_TRACE(TSTDOUT, 0, "Samples not valid with g_tsdConfig->maxError");
                }

            }    
            break;

        case 1:

            val0=*tsd_CoordFromMode(&TabSamples[0],mode_xy);
            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val0 > 0))
            {
                retval = TRUE;
                *sample=(UINT16)(val0);
            }                
                
            break;

        default:
            break;         
    }
    TSD_PROFILE_FUNCTION_EXIT(tsd_EvaluateSamples);
    return(retval);
}

#endif

// ============================================================================
//  tsd_GetScreenPoints
// ----------------------------------------------------------------------------
/// Get X and Y samples from the Touch Screen, evaluate them, then store
/// the average of valid raw samples: the raw point corresponding to the point
/// touched on the touch screen.
/// This raw point presents distortion errors, so has to be calibrated.
///
/// @screenPoint will contain raw coordinates if calibration parameters aren't as
/// accurate as we need (calibration process will be re-launched), else it will
/// contain a calibrated coordinates ready for display.
///
/// @return 1 if the function succeeded, 0 otherwise.
// ============================================================================
 U8 global_temp_count=0;	
 U32  global_result_all=0;
PROTECTED UINT16 tsd_GetButtonIndex(UINT16 gpadcVal)
{
	UINT32 adc_key1,adc_key4,adc_key5;
	UINT32 r_key5,r_key4;
	UINT32 efuse4=0,efuse5=0,efuse6=0,efuse7=0;
	static UINT8 g_calculated = 0;
	UINT32 k,a,b,r0;
	static UINT16 *dev = NULL;
	static UINT16 *key_adc = NULL;
	//static UINT32 dev[30] = {0};
	//static UINT32 key_adc[30] ={0};
	UINT32 offset =0;
	UINT32 devCount = 0;
	UINT32 i = 0;
	UINT8 ret = 0;
	r_key4 = 10000;
	r_key5 = 68000;	
#define RDA_ADDR_EFUSE_OPT_SETTING1 0x21
#define RDA_ADDR_EFUSE_OUT 0x3f
	if(!g_calculated)
	{
		if(dev == NULL)
			dev = COS_Malloc(g_tsdConfig->keyCount);
		if(key_adc == NULL)
			key_adc = COS_Malloc(g_tsdConfig->keyCount);
		efuse4 = pmd_keyCalibValue(0);

		if((efuse4&0xff) == 0)
		{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)		
			a = 1020;
			b = 12;
			r0 = 9300;
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)	
			a = 985;
			b = 32;
			r0 = 9538;
#endif
		}
		else
		{
			efuse5 = pmd_keyCalibValue(1);
			efuse6 = pmd_keyCalibValue(2);
			efuse7 = pmd_keyCalibValue(3);

			adc_key1 = efuse4&0xff;
			adc_key4 = ((efuse5&0xff)<<2)|(efuse6&0x3);
			adc_key5 = (efuse7&0xff)|(0x3<<8);
#ifdef TSD_DEBUG
			hal_HstSendEvent(SYS_EVENT,0xbbdbbd);
			hal_HstSendEvent(SYS_EVENT,adc_key1);
			hal_HstSendEvent(SYS_EVENT,adc_key4);
			hal_HstSendEvent(SYS_EVENT,adc_key5);
#endif
			b = adc_key1;
			k = (((adc_key4-b)*(r_key5/1000))*100)/((adc_key5-b)*(r_key4/1000));
			r0 = (r_key5*100-r_key4*k)/(k-100);
			a = (adc_key4-b)*(r0+r_key4)/r_key4;
		}
#ifdef TSD_DEBUG
		hal_HstSendEvent(SYS_EVENT,0xbadbad);
		hal_HstSendEvent(SYS_EVENT,(UINT16)a);
		hal_HstSendEvent(SYS_EVENT,(UINT16)b);
		hal_HstSendEvent(SYS_EVENT,(UINT16)r0);
#endif		
		for(i = 0;i < g_tsdConfig->keyCount;i++)
		{
			key_adc[i] =(UINT32)(a*g_tsdConfig->resistance[i]/(r0+g_tsdConfig->resistance[i])+b);	
#ifdef TSD_DEBUG
			hal_HstSendEvent(SYS_EVENT,key_adc[i]);
#endif
		}
		for(i = 0;i < g_tsdConfig->keyCount-1;i++)
		{
			dev[i] = key_adc[i+1]-key_adc[i];	
		}
		g_calculated = 1;
	}
	offset = key_adc[0]>gpadcVal?key_adc[0]-gpadcVal:gpadcVal-key_adc[0];
	for(i = 0;i < g_tsdConfig->keyCount-1;i++)
	{
		devCount +=dev[i];
		if(offset<devCount)
		{
			break;
		}
	}

	if(i == g_tsdConfig->keyCount-1)
	{
		ret = i;
	}
	else
	{
		if(offset < (devCount - dev[i]*6/10))
		{
			ret = i;
		}
		else if(offset >(devCount - dev[i]*4/10))
		{
			ret = i+1;
		}
		else
		{
			ret = 30;
		}
	}
	return ret;
}
UINT32 tsd_GetKeyRegValue(VOID)
{
UINT32 key_Data;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
pmd_RegRead(RDA_ADDR_TOUCH_SCREEN_RESULTS1,&key_Data);
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
	UINT32 i =0;
	key_Data = hwp_gpadc->key_data;
	while(!(key_Data&GPADC_KEY_VALUE_VALID))
	{
		if(i++>200)
		{
			return NOT_SUCCESSFUL;
		}
		key_Data = hwp_gpadc->key_data;
	}
#endif
	return key_Data;
}
PRIVATE UINT32 tsd_GetScreenPoints(TSD_POINT_T* screenPoint)
{
	UINT32 key_Data;
	//UINT8 button;
	key_Data= tsd_GetKeyRegValue();
	screenPoint->x = tsd_GetButtonIndex(key_Data&0x3ff);
	//hal_HstSendEvent(SYS_EVENT, 0xaaaaaaaa);
	//hal_HstSendEvent(SYS_EVENT, key_Data);
	//hal_HstSendEvent(SYS_EVENT, screenPoint->x);
	return screenPoint->x != 30?SUCCESSFUL:NOT_SUCCESSFUL;
#if 0
    UINT32 retval = NOT_SUCCESSFUL;
    // lcdScreenTouchPoint : Estimated lcd coordinates ( average of samples )
   // but without calibration fixing.
   // lcdScreenTouchPoint.x = 0;
   // lcdScreenTouchPoint.y = 0;
    TSD_POINT_T lcdScreenTouchPoint = { 0, 0, };
    TSD_PROFILE_FUNCTION_ENTRY(tsd_GetScreenPoints);
    // Get 4 X samples and 4 Y samples and store them to g_tabSamples
    if (tsd_GetSamples(g_tabSamples) == TSD_ERR_RESOURCE_BUSY)
    {
        return NOT_SUCCESSFUL;
    }
/*
    hal_HstSendEvent(SYS_EVENT, 0x14062501);
    for(UINT8 i=0;i<3;i++)
    {
        hal_HstSendEvent(SYS_EVENT, g_tabSamples[i].x);
    }
*/
    BOOL checkX = TRUE;
    BOOL checkY = TRUE;

#ifdef _USED_TSC_KEY_
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || \
    (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
#ifdef TSC_KEY_USE_X_COORDINATE
    checkY = FALSE;
#else
    checkX = FALSE;
#endif
#else // 8809 or later
    checkY = FALSE;
#endif
#endif // _USED_TSC_KEY_

    // Then, If the samples are valid,evaluate them and use matrix calibration to fix them.
    // checkX is always TRUE here on 5855, so only READ_X_AXIS will run.
    if ( (!checkX ||
          tsd_EvaluateSamples(g_tabSamples,&(lcdScreenTouchPoint.x),READ_X_AXIS)) &&
         (!checkY ||
          tsd_EvaluateSamples(g_tabSamples,&(lcdScreenTouchPoint.y),READ_Y_AXIS))
       )
    {
#ifdef _USED_TSC_KEY_
        HAL_ANA_GPADC_MV_T volt;
        if (checkX)
        {
            volt = hal_AnaGpadcGpadc2Volt(lcdScreenTouchPoint.x);
        }
        if (checkY)
        {
            volt = hal_AnaGpadcGpadc2Volt(lcdScreenTouchPoint.y);
        }
	//chenhanwen();	
	//hal_HstSendEvent(USB_EVENT, volt);
	if(global_temp_count<10)
	{
	    if(global_temp_count==0)
            global_result_all = 0;
      	global_result_all +=volt;
    	screenPoint->x = tsd_GetKeyIndex(volt);
    	EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_GetScreenPoints:mean x=0x%04x,y=0x%04x,volt=%d,KeyIndex=%d",
                lcdScreenTouchPoint.x, lcdScreenTouchPoint.y, volt, screenPoint->x);
        // Filter key index
        retval = tsd_FilterKey(screenPoint->x);
//        hal_HstSendEvent(SYS_EVENT, 0x14122401);
//        hal_HstSendEvent(SYS_EVENT, screenPoint->x);
//        hal_HstSendEvent(SYS_EVENT, retval);
	}
	else
	{
    	if(global_temp_count==10)
    	{
    	    global_result_all=global_result_all/10;
    	}
    	screenPoint->x = tsd_GetKeyIndex(global_result_all);
    	EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_GetScreenPoints:mean x=0x%04x,y=0x%04x,volt=%d,KeyIndex=%d",
                lcdScreenTouchPoint.x, lcdScreenTouchPoint.y, volt, screenPoint->x);
        // Filter key index
        retval = tsd_FilterKey(screenPoint->x);
	}
	  global_temp_count++;
#else
        EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_GetScreenPoints:mean x=0x%04x,y=0x%04x",
        lcdScreenTouchPoint.x, lcdScreenTouchPoint.y);

        retval = SAMPLE_VALID;

        if( g_isCalibrationDone == 1)
        {
            retval = SUCCESSFUL;
            // Provide calibrated coordinates ready for display
            GetDisplayPoint(screenPoint,&lcdScreenTouchPoint,&g_calibrationMatrix);
            
            //EDRV_TRACE(TSTDOUT,0,"calib: 2 x=%d y=%d",lcdScreenTouchPoint.x,lcdScreenTouchPoint.y);
            //EDRV_TRACE(TSTDOUT,0,"calib: 3 x=%d y=%d %d ",screenPoint->x,screenPoint->y,retval2);
            //EDRV_TRACE(TSTDOUT,0,"calib: 1=%d 2=%d",g_calibrationMatrix.An,g_calibrationMatrix.Bn);           
            //EDRV_TRACE(TSTDOUT,0,"calib: 3=%d 4=%d ",g_calibrationMatrix.Cn,g_calibrationMatrix.Dn);         
            //EDRV_TRACE(TSTDOUT,0,"calib: 5=%d 6=%d 7=%d",g_calibrationMatrix.En,g_calibrationMatrix.Fn,g_calibrationMatrix.Divider);
              
            if(!g_isCalibSuccess)
            {
                // If calibration matrix not accurate, raw point will be
                // delivered for a new calibration process
                screenPoint->x = lcdScreenTouchPoint.x;
                screenPoint->y = lcdScreenTouchPoint.y;
            }
        }
        else
        {
            // If the calibration has not been done yet, we return raw coordinate with scaling,rotating errors.
            screenPoint->x = lcdScreenTouchPoint.x;
            screenPoint->y = lcdScreenTouchPoint.y;
        }
#endif
    }

    TSD_PROFILE_FUNCTION_EXIT(tsd_GetScreenPoints);
    return(retval);
#endif
}


// ============================================================================
//  tsd_SetCalibStatus
// ----------------------------------------------------------------------------
/// Set the calibration status
/// @param isCalibrated \c TRUE when calibrated, \c FALSE to restart calibration
/// Once the status is set to \c TRUE, the callback #TSD_CALLBACK_T will be
/// called with corrected values.
/// note that tsd_SetCalibPoints() must be called before setting the calibration
/// to \c TRUE
// ============================================================================
PUBLIC VOID tsd_SetCalibStatus(BOOL isCalibStatus)
{
#ifndef _USED_TSC_KEY_
    g_isCalibSuccess = isCalibStatus;
#endif
}

// ============================================================================
//  tsd_Debounce
// ----------------------------------------------------------------------------
/// This function debounces signal coming from pen irq gpio pin.
/// It return a raw point coordinates to the g_userCallback function which will
/// do calibration process.
// ============================================================================
#if 0
INT32 tsd_GpioGet(VOID)
{//all key status
    UINT32 regVal = 0;
    UINT32 counter = 0;
    while(!pmd_RegRead(0x14, &regVal))
    {
        if(counter == 5)
        {
            break;
        }
        counter++;
    }

    if(counter < 5)
    {
        if((regVal>>10)&1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
/*
    if(pmd_RegRead(0x14, &regVal))
    {
        if((regVal>>10)&1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
*/
/*
    if((pmd_RDARead(0x14)>>10)&1)//up
        return 0;
    else//press down
     return 1;
*/
}
#endif

#define SAMPLE_X_MIN 0x90
#define SAMPLE_X_MAX 0x3e0
#define SAMPLE_STEP ((SAMPLE_X_MAX - SAMPLE_X_MIN)/8)
#ifdef filter_error_key  
void sort_data(UINT16* key_data, UINT16 key_numble)
{
	UINT16 i,j,b;
	key_numble=key_numble-1;
	for(i = 0; i < key_numble; i++)
	{
		for(j = 0; j < (key_numble -i) ; j++)
		{
			if(key_data[j] > key_data[j + 1])
			{ 
				b = key_data[j];
				key_data[j] = key_data[j + 1];
				key_data[j + 1] = b;
			}		
		}
	}
	for(i = 0; i <=key_numble; i++)
	{ 
		EDRV_TRACE(TSTDOUT, 0, "tsd_dcg2: key_data[%d]=%d",i,key_data[i]);
	}

}
#endif
UINT16 get_keymostFrequent(UINT8 *button,UINT8 num)
{
	UINT8 i = 0;
	UINT8 key[30] = {0};
	UINT8 ret = 0;
	UINT8 count = 0;
	for(i = 0;i < num;i++)
	{
		if(button[i] != 30)
			key[button[i]]++;
	}
	for(i = 0;i < 30;i++)
	{
		if(key[i] > count)
		{
			count = key[i];
			ret = i;
		}
	}
	return ret;
}
BOOL tsd_GetKeyIrqState(UINT32 *irqState)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
	return pmd_RegRead(RDA_ADDR_IRQ_SETTINGS, irqState);
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    *irqState = hwp_pmu_intf->pmu_intf_irsr;
      hwp_pmu_intf->pmu_intf_icr |= PMU_INTF_KEY_MEASURE_INT_CLEAR;
      return 1;
#endif
}
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#define RDA_KEY_IRQ_DOWN    RDA_PMU_EOMIRQ
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#define RDA_KEY_IRQ_DOWN    PMU_INTF_KEY_MEASURE_INT_RAW_STATUS
#endif
PUBLIC VOID tsd_Debounce(VOID)
{
    UINT32 irqState;
    static TSD_POINT_T          screenPoint;
    TSD_PROFILE_FUNCTION_ENTRY(tsd_Debounce);
    UINT32 resultOfsample = 0;
    UINT32 tsdClearState = 0;
    INT32 currentPenIrqPinState = TSD_EOM_UP;
	static UINT8 button[10] = {0};
	static UINT8 down_key = 0;
	static UINT8 send_key = 0;
#ifdef filter_error_key  	
	static UINT16 key_from_AD[30]={0};
	UINT8 i_count;	
	UINT32 key_Data0;
#endif

	if(g_tsdConfig == NULL)
	{
		g_tsdConfig = tgt_GetTsdConfig();
	}

    if (!tsd_GetKeyIrqState(&irqState))
    {
        EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: ISPI busy");
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
        return;
    }

    //irqState = pmd_OpalSpiRead(RDA_ADDR_IRQ_SETTINGS);
    //pmd_RegRead(RDA_ADDR_IRQ_SETTINGS, &irqState);
    if(irqState&RDA_KEY_IRQ_DOWN)
    {
    	currentPenIrqPinState = TSD_EOM_DOWN;
    }

    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: curState=%d, prevState=%d, IRQ=0x%04x",
        currentPenIrqPinState, g_previousPenIrqPinState, irqState);

    //hal_HstSendEvent(SYS_EVENT, (0x7774<<16) | (g_count<<8) | (g_penState<<4) | currentPenIrqPinState);
    //hal_HstSendEvent(SYS_EVENT, irqState);

    if(currentPenIrqPinState != g_previousPenIrqPinState)
    {
        // The pen state has just changed : We reset the counter.
        g_count = 0;
#ifdef _USED_TSC_KEY_
        g_tsdKeyFiltIndex = 0;
#endif
        sxr_StartFunctionTimer((TSD_DEBOUNCE_TIME), tsd_Debounce,(VOID*)NULL,0x03);
    }
    else
    {
        // The pen state does not change.
#ifdef filter_error_key
		//resultOfsample = tsd_GetScreenPoints(&screenPoint);
		i_count=g_count;

		if(i_count>g_tsdConfig->downPeriod)
			i_count=g_tsdConfig->downPeriod;
		
		resultOfsample=1;
		pmd_RegRead(RDA_ADDR_TOUCH_SCREEN_RESULTS1,&key_Data0);		
		key_from_AD[i_count]=hal_AnaGpadcGpadc2Volt(key_Data0&0x3ff);
		
		EDRV_TRACE(TSTDOUT, 0, "tsd_dcg3: key_from_AD[%d]=%d",i_count,key_from_AD[i_count]);
		
#endif
        g_count++;
        switch (g_penState)
        {
            case TSD_EOM_DOWN :
            {
              EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: TSD_PEN_DOWN g_count=%d onUp=%d g_lastSendPenState=%d",g_count,g_userReportingMode.onUp,g_lastSendPenState);

                // If we reach upPeriod and if PenIrqPinState is low
                if( (g_count >= g_tsdConfig->upPeriod) && (currentPenIrqPinState == TSD_EOM_UP) )
                {
                    // We consider the pen down.
                    g_penState = TSD_EOM_UP;
                    // We reset the counter.
                    g_count = 0;

                    // We calculate the coordinates of the touch screen point.

                    // If the user wants a call to g_userCallback function.
                    if (g_userReportingMode.onUp)
                    {
                        if(g_lastSendPenState == TSD_EOM_DOWN || g_lastSendPenState == TSD_EOM_PRESSED)
                        {
                            if (g_userCallback)
                            {
                                (*g_userCallback)(&screenPoint, g_penState);
                            }
                            g_lastSendPenState = TSD_EOM_UP;
                        }
                    }
                    // We enable the host pen irq and do not reload the timer.
                    tsdClearState = 1;
                }
                else 
                {
                	resultOfsample = tsd_GetScreenPoints(&screenPoint);
					button[g_count-1] = screenPoint.x;
                    // If we reach repetitionPeriod and if PenIrqPinState is high.
                    if( (g_count >= g_userReportingMode.repetitionPeriod) && (currentPenIrqPinState == TSD_EOM_DOWN) )
                    {
                    	screenPoint.x = get_keymostFrequent(button,g_count);
                        // We consider the pen down.
                        g_penState = TSD_EOM_PRESSED;
                        // We reset the counter.
                        g_count = 0;

                        // We calculate the coordinates of the touch screen point.

                        // If the user wants a call to g_userCallback function.
#ifdef filter_error_key  
						sort_data(key_from_AD,i_count+1);  //ÅÅÐòºóÈ¡×îÐ¡Öµ

						screenPoint.x = tsd_GetKeyIndex(key_from_AD[0]);
					
						EDRV_TRACE(TSTDOUT, 0, "tsd_dcg4: i_count=%d =screenPoint.x=%d",i_count,screenPoint.x);				
#else
                    //    resultOfsample = tsd_GetScreenPoints(&screenPoint);
#endif
                        if (g_userReportingMode.onPressed)
                        {
                            if(resultOfsample != NOT_SUCCESSFUL)
                            {
                                if (g_userCallback)
                                {
                                    (*g_userCallback)(&screenPoint, g_penState);
                                }
                                g_lastSendPenState = TSD_EOM_PRESSED;
                            }
                        }
                    }

                    // We reload the timer.
                    sxr_StartFunctionTimer((TSD_REPITITION_TIME),tsd_Debounce,(VOID*)NULL,0x03); 
                }

                break;
            }

            case TSD_EOM_UP :
            {
                EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: TSD_PEN_UP g_count=%d onDown=%d ",g_count,g_userReportingMode.onDown);

                if(currentPenIrqPinState == TSD_EOM_DOWN)
                {
                	resultOfsample = tsd_GetScreenPoints(&screenPoint);
					button[g_count-1] = screenPoint.x;
                    if (g_count >= g_tsdConfig->downPeriod)
                    {
                    	screenPoint.x = get_keymostFrequent(button,g_count);
                        // We consider the pen down.
                        g_penState = TSD_EOM_DOWN;
                        // We reset the counter.
                        g_count = 0;
                        down_key = screenPoint.x;
                        // We calculate the coordinates of the touch screen point.

                        // If the user wants a call to g_userCallback function.
#ifdef filter_error_key     
					sort_data(key_from_AD,i_count+1);

					screenPoint.x = tsd_GetKeyIndex(key_from_AD[0]);

					EDRV_TRACE(TSTDOUT, 0, "tsd_dcg5: i_count=%d =screenPoint.x=%d",i_count,screenPoint.x); 			

#else

                 //       resultOfsample = tsd_GetScreenPoints(&screenPoint);
#endif
                        if (g_userReportingMode.onDown)
                        {
                            if(resultOfsample != NOT_SUCCESSFUL)
                            {
                                if (g_userCallback)
                                {
                                    (*g_userCallback)(&screenPoint, g_penState);
                                }
                                g_lastSendPenState = TSD_EOM_DOWN;
                            }
                            else
                            {
                                g_penState = TSD_EOM_UP;                        
                            }
                        }
                    }

                    sxr_StartFunctionTimer((TSD_DEBOUNCE_TIME),tsd_Debounce,(VOID*)NULL,0x03);
                }
                else
                {
                    if (g_count <= g_tsdConfig->upPeriod)
                    {
                        sxr_StartFunctionTimer((TSD_DEBOUNCE_TIME),tsd_Debounce,(VOID*)NULL,0x03);
                    }
                    else
                    {
                        g_count = 0;
                        tsdClearState = 1;
                    }
                }
                break;
            }

            case TSD_EOM_PRESSED :
            {   
                EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: TSD_PEN_PRESSED g_count=%d onUp=%d g_lastSendPenState=%d",g_count,g_userReportingMode.onUp,g_lastSendPenState);

                // If we reach upPeriod and if PenIrqPinState is low
                if( (g_count >= g_tsdConfig->upPeriod) && (currentPenIrqPinState == TSD_EOM_UP) )
                {
                    // We consider the pen up.
                    g_penState = TSD_EOM_UP;
                    // We reset the counter.
                    g_count = 0;

                    // We calculate the coordinates of the touch screen point.

                    // If the user wants a call to g_userCallback function.
                    if (g_userReportingMode.onUp)
                    {
                        if(g_lastSendPenState == TSD_EOM_DOWN || g_lastSendPenState == TSD_EOM_PRESSED)
                        {
                            if (g_userCallback)
                            {
                                (*g_userCallback)(&screenPoint, g_penState);
                            }
                            g_lastSendPenState = TSD_EOM_UP;
                        }
                    }
                    // We enable the host pen irq and do not reload the timer.
                    tsdClearState = 1;
                }
                else
                {
                	resultOfsample = tsd_GetScreenPoints(&screenPoint);
					button[g_count-1] = screenPoint.x;
                    if( (g_count >= g_userReportingMode.repetitionPeriod) && (currentPenIrqPinState==TSD_EOM_DOWN) )
                    {
                    	screenPoint.x = get_keymostFrequent(button,g_count);
                        // We consider the pen still pressed and we reset the counter.
                        g_count=0;

                        // We calculate the coordinates of the touch screen point.
                        if(!send_key)
                        {
                        	if(down_key != screenPoint.x)
                        	{
                        		 (*g_userCallback)(&screenPoint, TSD_EOM_DOWN);
                        		 send_key = 1;
                        	}
                        }

                        // If the user wants a call to g_userCallback function.
                
#ifdef filter_error_key     
						sort_data(key_from_AD,i_count+1);

						screenPoint.x = tsd_GetKeyIndex(key_from_AD[0]);
					
						EDRV_TRACE(TSTDOUT, 0, "tsd_dcg6: i_count=%d =screenPoint.x=%d",i_count,screenPoint.x);				
#else

               //         resultOfsample = tsd_GetScreenPoints(&screenPoint);
#endif
                        if (g_userReportingMode.onPressed)
                        {
                            if(resultOfsample != NOT_SUCCESSFUL)
                            {
                                if (g_userCallback)
                                {
                                    (*g_userCallback)(&screenPoint, g_penState);
                                }
                                g_lastSendPenState = TSD_EOM_PRESSED;
                            }
                        }
                    }

                    // We reload the timer.
                    sxr_StartFunctionTimer((TSD_REPITITION_TIME),tsd_Debounce,(VOID*)NULL,0x03);
                }

                break;

            }

            default:
            {
            }
        }
    }

    g_previousPenIrqPinState = currentPenIrqPinState;

    tsd_ClearTouchIrq();
    if(tsdClearState == 1)    
    {	
    	down_key = 0;
    	send_key = 0;
    	sxr_StopFunctionTimer(tsd_Debounce);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
	hwp_pmu_intf->pmu_intf_icr |= PMU_INTF_KEY_INT_CLEAR;
	hwp_pmu_intf->pmu_intf_imr &= ~PMU_INTF_KEY_MEASURE_INT_MASK;
	hwp_sys_ctrl->per_module_en &= ~SYS_CTRL_GPADC_EN_ENABLE;
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    	pmd_RegIrqSettingClr(RDA_PMU_EOMIRQ_MASK);
#endif
#ifdef KEY_LED_MUX_FUNCTION
        key_enterInterrupt = 0;
        //enable_LedKeyTimer();
#endif
    }

    TSD_PROFILE_FUNCTION_EXIT(tsd_Debounce);
}

#ifdef ROTARY_SWITCH_USED
#define MIN_VOLT   (g_tsdConfig->minVolt)
#define MAX_VOLT   (g_tsdConfig->maxVolt)
static UINT32 g_lastVolt;
static UINT32 g_voltStep;
static UINT8  g_lastVolume;

PRIVATE VOID tsd_RotarySwitchProbe(VOID)
{
	UINT32 key_Data0;
    UINT32 volt = 0;
    UINT16 tmp = 1;
    UINT32 irqState;
    
    pmd_RegRead(RDA_ADDR_TOUCH_SCREEN_RESULTS1,&key_Data0);
    volt = hal_AnaGpadcGpadc2Volt(key_Data0&0x3ff);

    if(g_lastVolt!=volt)
    {
        COS_EVENT ev;

        if(volt >= MAX_VOLT)
        {
            tmp = AUD_MAX_LEVEL;
        }
        else if(volt > MIN_VOLT)
        {
            volt -= MIN_VOLT;
            while(volt > g_voltStep)
            {
                tmp++;
                volt -= g_voltStep;
            }
            if(tmp > AUD_MAX_LEVEL)
            {
                tmp = AUD_MAX_LEVEL;
            }
        }

        if(tmp != g_lastVolume)
        {
            hal_HstSendEvent(SYS_EVENT, 0x17051201);
            hal_HstSendEvent(SYS_EVENT, tmp);
            
            g_lastVolume = tmp;
            ev.nEventId = (UINT16)AT_COMMON_VOLUME;
            ev.nParam1 = tmp;
            COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        }
        
        g_lastVolt = volt;
    }
}

PRIVATE VOID tsd_RotarySwitchDetectionTrigger(VOID)
{
    pmd_RegIrqSettingSet(RDA_PMU_KEYIRQ_MASK|RDA_PMU_EOMIRQ_MASK|RDA_PMU_EOMIRQ_CLEAR|RDA_PMU_KEYIRQ_CLEAR);
    sxr_StartFunctionTimer(g_tsdConfig->debounceTime, tsd_RotarySwitchProbe,(VOID*)NULL,0x03);
}

PUBLIC VOID tsd_RotarySwitchOpen(VOID)
{
	if(g_tsdConfig == NULL)
	{
		g_tsdConfig = tgt_GetTsdConfig();
	}

    g_voltStep = (MAX_VOLT - MIN_VOLT)/AUD_MAX_LEVEL;
    pmd_RegIrqSettingSet(RDA_PMU_KEYIRQ_MASK|RDA_PMU_EOMIRQ_MASK);
    COS_SetTimer(200, tsd_RotarySwitchDetectionTrigger, NULL, COS_TIMER_MODE_PERIODIC);
}
#endif

#if 0
// ============================================================================
//  tsd_GpioIrqHandler
// ----------------------------------------------------------------------------
// This function handles GPIO hard interruptions.
// ============================================================================
PRIVATE VOID tsd_GpioIrqHandler(VOID)
{
    TSD_PROFILE_FUNCTION_ENTRY(tsd_GpioIrqHandler);
    // We check the value of the penIrqPin to avoid false triggering when we reset
    // the touch screen controler.


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)    
    UINT32 regVal = 0;
    if(pmd_RegRead(0x46, &regVal))
    {
        pmd_RDAWrite(0x46,(regVal|(1<<8)|(1<<9)));
    }
    else
    {
        hal_HstSendEvent(SYS_EVENT, 0x14121944);
        return ;
    }
#endif

    hal_HstSendEvent(SYS_EVENT, 0xdeadeeee);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809P)    
    hal_HstSendEvent(SYS_EVENT, hal_GpioGet(g_tsdConfig->penGpio));
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) 
    hal_HstSendEvent(SYS_EVENT, tsd_GpioGet());
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809P)    
    if (hal_GpioGet(g_tsdConfig->penGpio) == 1)
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) 
//    if(tsd_GpioGet()==1)
#endif
    {
    
        // We disable host Pen IRQ while the screen is pressed.
        tsd_DisableHostPenIrq();
        pmd_TsdClearTouchIrq();
//        hal_HstSendEvent(SYS_EVENT, 0xdeadcccc);
//        hal_HstSendEvent(SYS_EVENT, *((UINT32*)(0xA1A0301c)));
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
    }
#if 0
    else
    {
        hal_HstSendEvent(SYS_EVENT, 0x14071701);
        regVal = 0;
        if(pmd_RegRead(1, &regVal))
        {
            hal_HstSendEvent(SYS_EVENT, regVal);
        }        
        
        pmd_TsdClearTouchIrq();
        
        hal_HstSendEvent(SYS_EVENT, 0x14071702);
        regVal = 0;
        if(pmd_RegRead(1, &regVal))
        {
            hal_HstSendEvent(SYS_EVENT, regVal);
        } 
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
    }
#endif
    
    TSD_PROFILE_FUNCTION_EXIT(tsd_GpioIrqHandler);
}
#endif

#ifdef _USED_TSC_KEY_
// ============================================================================
//  tsd_KeyOpen
// ----------------------------------------------------------------------------
/// This function initializes the touch screen key driver. The configuration of the
/// touch screen pins used is board dependent and stored in a TSD_CONFIG_T struct.
// ============================================================================
PUBLIC VOID tsd_KeyOpen(TSD_KEY_CALLBACK_T callback)
{
    g_userKeyCallback = callback;
    g_userCallback = &tsd_KeyCallback;

    g_userReportingMode.onDown  = TRUE;
    g_userReportingMode.onUp = TRUE;
   g_userReportingMode.onPressed = TRUE;
   g_userReportingMode.repetitionPeriod = 3;
    tsd_Open();
}
#endif


// ============================================================================
//  tsd_Open
// ----------------------------------------------------------------------------
/// This function initializes the touch screen driver. The configuration of the
/// touch screen pins used is board dependent and stored in a TSD_CONFIG_T struct.
// ============================================================================
PUBLIC VOID tsd_Open()
{
#ifdef _USED_TSC_KEY_
    static BOOL opened = FALSE;

    if (opened)
    {
        return;
    }
    opened = TRUE;
#endif

    g_tsdConfig = tgt_GetTsdConfig();
    g_pTSUserData = DSM_GetUserData(DSM_TS_DATA, sizeof(TSD_USER_DATA_T));
//    g_gpioCfg.direction = HAL_GPIO_DIRECTION_INPUT;

    if(g_pTSUserData->magic != TSD_USER_DATA_MAGIC)
    {
        g_pTSUserData->magic = TSD_USER_DATA_MAGIC;
        g_pTSUserData->minVol = g_tsdConfig->minVolt;
        g_pTSUserData->maxVol = g_tsdConfig->maxVolt;
        //DSM_WriteUserData();
    }

#ifdef ROTARY_SWITCH_USED
    tsd_RotarySwitchOpen();
    return ;
#endif

    while(!pmd_TsdEnableIrq())
    {
        sxr_Sleep(10);
    }

#if (0)
    // GPIO irq mask cfg
    g_gpioCfg.irqMask.falling = FALSE;
    g_gpioCfg.irqMask.debounce = TRUE;
    g_gpioCfg.irqHandler = tsd_GpioIrqHandler;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) 
    g_gpioCfg.irqMask.level = FALSE;
#else
    g_gpioCfg.irqMask.level = TRUE;
#endif
    // Rising level interrupt will be actually enabled in tsd_EnableHostPenIrq
    g_gpioCfg.irqMask.rising = FALSE;
    hal_GpioOpen(g_tsdConfig->penGpio, &g_gpioCfg); 
#endif /* 0 */ 
    tsd_EnableHostPenIrq();
#ifndef CHIP_5856E
    pmd_RegIrqSettingClr(RDA_PMU_PENIRQ_MASK);
    pmd_RegIrqSettingClr(RDA_PMU_EOMIRQ_MASK);
#endif    
#if  (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    hwp_sysIrq->mask_set |= SYS_IRQ_MASK_SET(0x80000000);	
    hwp_pmu_intf->keysense_ctrl1 |=PMU_INTF_KEY_ENABLE;
    hwp_pmu_intf->pmu_intf_imr   &= ~PMU_INTF_KEY_MEASURE_INT_MASK;
	hwp_pmu_intf->keysense_ctrl1 |= PMU_INTF_KEY_CLK_DIV_EN;
	hwp_pmuc->ldo_ctrl |= PMUC_LDO_ULP_VPAD_VBIT(7);
	hwp_pmu_intf->gpadc_ctrl1  |=PMU_INTF_DELAY_BEFORE_SAMP_GPADC(3)|PMU_INTF_TIME_SAMP_POS_GPADC;
#endif
 //   gpio_KeyLedInit();
}


// ============================================================================
//  tsd_Close
// ----------------------------------------------------------------------------
/// This function closes the touch screen driver. Deactivate and close the SPI
// ============================================================================
PUBLIC VOID tsd_Close(VOID)
{
    while(!pmd_TsdDisableIrq())
    {
        sxr_Sleep(10);
    }
    
    hal_GpioClose(g_tsdConfig->penGpio);
}

// ============================================================================
//  tsd_Key_Calib
// ----------------------------------------------------------------------------
/// This function calib the key
/// @param start: 0, start calib; 1, first key press; 2, last key press
// ============================================================================
PUBLIC TSD_ERR_T tsd_Key_Calib(UINT start)
{
    EDRV_TRACE(TSTDOUT, 0, "tsd_Calib start=%d,volt=%d", start, global_last_key_vol);
    if(start==0)
    {
        g_tsk_calib_flag = 1;
    	global_result_all = 0;
    	global_temp_count = 0;
    }
    else if(start==1)
    {
        g_pTSUserData->minVol = global_last_key_vol;
    }
    else if(start==2)
    {
        g_tsk_calib_flag = 0;
        if(g_pTSUserData->minVol >= global_last_key_vol)
            return TSD_ERR_INVALID_CALIBRATION;
        g_pTSUserData->maxVol = global_last_key_vol;
        DSM_WriteUserData();
    }
    return TSD_ERR_NO;
}

// ============================================================================
//  tsd_SetCallback
// ----------------------------------------------------------------------------
/// This function configures which user function will be called by the
/// touch screen driver when the callback is invoked. See the function
/// #tsd_SetReportingMode() for details about when the callback is invoked.
/// 
/// @param callback Pointer to a user function called when an interruption
/// is generated by the touch screen driver.
// ============================================================================
PUBLIC VOID tsd_SetCallback(TSD_CALLBACK_T callback)
{
#ifndef _USED_TSC_KEY_
    g_userCallback = callback;
#endif
}

#if 0

// ============================================================================
//  tsd_SetReportingMode
// ----------------------------------------------------------------------------
/// This function controls under which conditions the touch screen interrupt 
/// will be generated. It can be when the pen is pressed, when it is hold down 
/// and when it is released. Use the function #tsd_SetCallback() to configure
/// which user function will be called in case of a touch screen interruption.
///
/// @param mode Defines which event will generate a call to the user
/// callback function. See the documentation of the type for details.
// ============================================================================
PUBLIC VOID tsd_SetReportingMode(TSD_REPORTING_MODE_T* mode)
{
#ifdef _USED_TSC_KEY_

    return;

#else // !_USED_TSC_KEY_

    g_userReportingMode.onDown    = mode->onDown;
    g_userReportingMode.onPressed = mode->onPressed;
    g_userReportingMode.onUp      = mode->onUp;
    g_userReportingMode.repetitionPeriod = mode->repetitionPeriod;

    TSD_ASSERT( (g_userReportingMode.repetitionPeriod != 0) 
                || (!g_userReportingMode.onPressed), 
            "Please set the repetitionPeriod when using onPressed!");

    // don't actually enable IRQ before Open.
    if (g_tsdConfig)
    {
        // depending on wanted events we can disable the IRQ.
        if (g_userReportingMode.onDown 
            || g_userReportingMode.onPressed
            || g_userReportingMode.onUp)
        {
            while(!pmd_TsdEnableIrq())
            {
                sxr_Sleep(10);
            }
            tsd_EnableHostPenIrq();
        }
        else
        {
            // no events enabled, stop pen Irq
            tsd_DisableHostPenIrq();
            while(!pmd_TsdDisableIrq())
            {
                sxr_Sleep(10);
            }
        }
    }

#endif // !_USED_TSC_KEY_
}


// ============================================================================
//  tsd_GetReportingMode
// ----------------------------------------------------------------------------
/// This function returns the touch screen reporting mode. Refer to the type 
/// documentation for its interpretation.
///
/// @param mode A pointer to receive the current reporting mode.
// ============================================================================
PUBLIC VOID tsd_GetReportingMode(TSD_REPORTING_MODE_T* mode)
{
    TSD_ASSERT( mode != NULL, "Pointer mode is NULL!");
    mode->onDown    = g_userReportingMode.onDown;
    mode->onUp      = g_userReportingMode.onUp;
    mode->onPressed = g_userReportingMode.onPressed;
    mode->repetitionPeriod = g_userReportingMode.repetitionPeriod;
}

#endif

// ============================================================================
//  tsd_SetCalibPoints
// ----------------------------------------------------------------------------
/// This function fixes the calibration parameters of the touch screen driver.
/// From an array of 3 samples per point, it sets an array of
/// 3 Display points and another array of 3 Screen points needed for calibration
///
/// @param calibPoints array of 3 calibration points
//  @return TSD_ERR_INVALID_CALIBRATION if the calibration matrix is invalid.
///
/// This function include a fatal assert that will be triggered if the coordinates
/// of the expected points do not allow a valid calibration (divide by 0)
/// this occurs when the point are aligned, or some have the same value for X or Y
// ============================================================================
PUBLIC TSD_ERR_T tsd_SetCalibPoints(TSD_CALIBRATION_POINT_T calibPoints[3])
{
#ifndef _USED_TSC_KEY_
    UINT8  i;

    // Fix tab of 3 calibration points from tab calibPoints[3] of 3 

    for(i = 0; i < 3; i++)
    {
    // fill tab of 3 samples for each point N°1 N°2 and N°3
        g_tabCalibratedExpectedPoint[i].x = calibPoints[i].expected.x;
        g_tabCalibratedExpectedPoint[i].y = calibPoints[i].expected.y;
        g_tabCalibratedMeasuredPoint[i].x  = calibPoints[i].measured.x;
        g_tabCalibratedMeasuredPoint[i].y  = calibPoints[i].measured.y;
    }

    g_isCalibrationDone = SetCalibrationMatrix(g_tabCalibratedExpectedPoint, g_tabCalibratedMeasuredPoint
                                  , &g_calibrationMatrix);

    if(g_isCalibrationDone)
    { 
        return TSD_ERR_NO;
    }
#endif
    return TSD_ERR_INVALID_CALIBRATION;
}

