/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**************************************************************************************
* VoC Audio decoder
* 
* August 2007
*
* main.c 
**************************************************************************************/

#include "cs_types.h"

#include "hal_error.h"
#include "hal_voc.h"

#include "vpp_ecc.h"
#include "mcip_debug.h"

#ifdef BT_SSP_VOC_SUPPORT

static const unsigned int mont_a[6] = {0xfffffffc,0xffffffff,0xfffffffb,0xffffffff,0xffffffff,0xffffffff};
static const unsigned int g_x[6]    = {0x82ff1012,0xf4ff0afd,0x43a18800,0x7cbf20eb,0xb03090f6,0x188da80e};
static const unsigned int g_y[6]    = {0x1e794811,0x73f977a1,0x6b24cdd5,0x631011ed,0xffc8da78,0x07192b95};

const UINT32 dh_key_test[6] = {0xf09bb43e,0x57243002,0xa10be800,0x29065c3c,0xe0ec973c,0x52ec1ca6};;

const INT32 G_VppECCCode[] =
{
#include "audiojpeg_dec_common.tab"
};

BOOL vpp_ECC_VocOpen_status = FALSE;

void vpp_ECCSetConst()
{
    INT32 *ptr32;
    INT16 *ptr16;
    INT i;

	ptr32=hal_VocGetPointer(STATIC_MOTION_A);
	*ptr32++=mont_a[0];
	*ptr32++=mont_a[1];
	*ptr32++=mont_a[2];
	*ptr32++=mont_a[3];
	*ptr32++=mont_a[4];
	*ptr32=mont_a[5];

	ptr32  = hal_VocGetPointer(STATIC_TEMP1);	
	*ptr32++ = 1;
	*ptr32++ = 0;
	*ptr32++ = 2;
	*ptr32++ = 0;
	*ptr32++ = 1;
	*ptr32++ = 0;	

	ptr32  = hal_VocGetPointer(STATIC_TEMP2);	
	*ptr32++ = 1;
	*ptr32++ = 0;
	*ptr32++ = 0;
	*ptr32++ = 0;
	*ptr32++ = 0;
	*ptr32++ = 0;	
	
	ptr32  = hal_VocGetPointer(STATIC_MOUNT_1);	
	*ptr32++ = 3;
	*ptr32++ = 0;
	*ptr32++ = 3;
	*ptr32++ = 0;
	*ptr32++ = 0;
	*ptr32++ = 0;	
	ptr32  = hal_VocGetPointer(STATIC_MOUNT_2);	
	*ptr32++ = 2;
	*ptr32++ = 0;
	*ptr32++ = 2;
	*ptr32++ = 0;
	*ptr32++ = 0;
	*ptr32++ = 0;	

	ptr32  = hal_VocGetPointer(STATIC_ADD_SUB_MOD_T);	
	*ptr32++ = -1;
	*ptr32++ = -1;
	*ptr32++ = -2;
	*ptr32++ = -1;
	*ptr32++ = -1;
	*ptr32++ = -1;	

	ptr32  = hal_VocGetPointer(STATIC_MUL_MODE_C_1);	
	*ptr32++ = 1;
	*ptr32++ = 0;
	*ptr32++ = 1;
	*ptr32++ = 0;
	*ptr32++ = 0;
	*ptr32++ = 0;	

	ptr16  = hal_VocGetPointer(STATIC_STEP_NUM);
	*ptr16++ = 192;
	*ptr16++ = 191;	


	ptr16  = hal_VocGetPointer(CONST_SHORT_1);	
	*ptr16++ = 1;
	*ptr16++ = 2;
	*ptr16++ = 3;
	*ptr16++ = 4;
	*ptr16++ = 5;
	*ptr16++ = 6;
	*ptr16++ = 8;
	*ptr16++ = 16;
	*ptr16++ = 32;
	*ptr16++ = 64;
	*ptr16++ = 128;
	*ptr16++ = 0xff;


	ptr32  = hal_VocGetPointer(CONST_32BIT_0);	
	*ptr32++ = 0;		
	*ptr32++ = 1;
	*ptr32++ = 2;
	*ptr32++ = 4;
	*ptr32++ = 8;
	*ptr32++ = 16;
	*ptr32++ = 32;	
	*ptr32++ = 64;	
	*ptr32++ = 128;		
	*ptr32++ = 256;		
	*ptr32++ = 0xfffffffe;		
	*ptr32++ = 0xffffffff;			

	ptr16  = hal_VocGetPointer(VOC_P192_MUL_MOD_Q1);	
    for(i=0;i<16;i++)
        ptr16[i] = 0xfff;
    ptr16[5] = 0xfef;

	ptr16  = hal_VocGetPointer(VOC_P192_MUL_MOD_Q1_SUP);	
    for(i=0;i<16;i++)
        ptr16[i] = 0x000;
    ptr16[0] = 0x001;
    ptr16[5] = 0x010;
}

//=============================================================================
// vpp_ECCOpen function
//-----------------------------------------------------------------------------
HAL_ERR_T vpp_ECCOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler) 
{
    HAL_VOC_CFG_T cfg;
    INT32 * *pDMA;
    INT16 *pFilter;

    cfg.vocCode              = G_VppECCCode;
    cfg.vocCodeSize          = 4096;
    cfg.pcVal                = 0;
    cfg.pcValCriticalSecMin  = 0;
    cfg.pcValCriticalSecMax  = 0;
    cfg.needOpenDoneIrq      = FALSE;
    cfg.irqMask.voc          = (vocIrqHandler) ? 1 : 0;
    cfg.irqMask.dmaVoc       = 0;
    cfg.vocIrqHandler        = vocIrqHandler;
    
    cfg.eventMask.wakeupIfc0 = 0;
    cfg.eventMask.wakeupIfc1 = 0;
    cfg.eventMask.wakeupDmae = 0;
    cfg.eventMask.wakeupDmai = 0;
    cfg.eventMask.wakeupSof0 = 0;
    cfg.eventMask.wakeupSof1 = 0;
    
    // load the VPP AUDIO_DEC  code and configure the VoC resource        
    switch (hal_VocOpen(&cfg))
    {
        // error opening the resource
        case HAL_ERR_RESOURCE_BUSY:                 
            diag_printf("[vpp_AudioJpeg_DEC]##WARNING##Error opening VoC resource\n"); 
            return HAL_ERR_RESOURCE_BUSY;
                                        
        // first open, load the constant tables                            
        case HAL_ERR_RESOURCE_RESET:              
            diag_printf("[vpp_AudioJpeg_DEC]First open.\n");			
            break;

        default:           
            diag_printf("[vpp_AudioJpeg_DEC]No first open.\n"); 
            break;
    }                          

   diag_printf("[vpp_AudioJpeg_DEC]Initializing the DMA addr.\n");

    vpp_ECCSetConst();
    
    vpp_ECC_VocOpen_status = TRUE;
    return  hal_VocWakeup(HAL_VOC_START);
            
}


//=============================================================================
// vpp_AudioJpegDecClose function
//-----------------------------------------------------------------------------
void vpp_ECCClose(void)
{
    if(vpp_ECC_VocOpen_status==TRUE)
	    hal_VocClose();   

    vpp_ECC_VocOpen_status=FALSE;    
    diag_printf("[vpp_AudioJpeg_DEC]Closing VPP\n");
    
}

int step = 0;

void vpp_ECC_VocISR(void)
{
    diag_printf("[vpp_ECC_VocISR] step=%d, tick=%d\n", step, hal_TimGetUpTime());
    hal_HstSendEvent(SYS_EVENT, 0x06070809);
    hal_HstSendEvent(SYS_EVENT, step);
    step++;
} 



void vpp_ECC_Generate_Local_Key(UINT32 *LocalKey)
{
    INT32 * ptr32;

    diag_printf("[vpp_ECC_Generate_Local_Key]Start VPP, tick=%d\n", hal_TimGetUpTime());
    step = 0;
    while(vpp_ECCOpen(vpp_ECC_VocISR)!=HAL_ERR_NO)
        COS_Sleep(100);

	ptr32  = hal_VocGetPointer(P192_FUNC_IN_GX);	
	*ptr32++ = g_x[0];		
	*ptr32++ = g_x[1];
	*ptr32++ = g_x[2];
	*ptr32++ = g_x[3];
	*ptr32++ = g_x[4];
	*ptr32 = g_x[5];
	ptr32  = hal_VocGetPointer(P192_FUNC_IN_GY);	

	*ptr32++ = g_y[0];		
	*ptr32++ = g_y[1];
	*ptr32++ = g_y[2];
	*ptr32++ = g_y[3];
	*ptr32++ = g_y[4];
	*ptr32 = g_y[5];
	ptr32  = hal_VocGetPointer(P192_FUNC_IN_K);	

	*ptr32++ = LocalKey[0];		
	*ptr32++ = LocalKey[1];
	*ptr32++ = LocalKey[2];
	*ptr32++ = LocalKey[3];
	*ptr32++ = LocalKey[4];
	*ptr32 = LocalKey[5];      

    diag_printf("[vpp_ECC_Generate_Local_Key]end, tick=%d\n", hal_TimGetUpTime());
    while(step==0); // wait voc inttrupt
    
    hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);

}


void vpp_ECC_Generate_DHKey(UINT32 *LocalKey, UINT8 *peer_key)
{
    INT8 * ptr8;
    INT32 * ptr32;

    diag_printf("[vpp_ECC_Generate_DHKey]Start VPP, tick=%d\n", hal_TimGetUpTime());

    step = 0;
    while(vpp_ECCOpen(vpp_ECC_VocISR)!=HAL_ERR_NO)
        COS_Sleep(100);

	ptr8  = hal_VocGetPointer(P192_FUNC_IN_GX);
        for(int i=0;i<24;i++)
            *ptr8++ = peer_key[i];
	/**ptr32++ = peer_key[0];		
	*ptr32++ = peer_key[1];
	*ptr32++ = peer_key[2];
	*ptr32++ = peer_key[3];
	*ptr32++ = peer_key[4];
	*ptr32 = peer_key[5];*/
	ptr8  = hal_VocGetPointer(P192_FUNC_IN_GY);
    for(int i=24;i<48;i++)
            *ptr8++ = peer_key[i];

	/* *ptr32++ = peer_key[6];		
	*ptr32++ = peer_key[7];
	*ptr32++ = peer_key[8];
	*ptr32++ = peer_key[9];
	*ptr32++ = peer_key[10];
	*ptr32 = peer_key[11];*/
	ptr32  = hal_VocGetPointer(P192_FUNC_IN_K);	
        /*for(int i=0;i<6;i++)
        {
            *ptr8++ = LocalKey[i]&&0XFF;
            *ptr8++ = (LocalKey[i]>>8)&&0XFF;
            *ptr8++ = (LocalKey[i]>>16)&&0XFF;
            *ptr8++ = (LocalKey[i]>>24)&&0XFF;
        }*/
	*ptr32++ = LocalKey[0];		
	*ptr32++ = LocalKey[1];
	*ptr32++ = LocalKey[2];
	*ptr32++ = LocalKey[3];
	*ptr32++ = LocalKey[4];
	*ptr32 = LocalKey[5];      

    diag_printf("[vpp_ECC_Generate_DHKey]end, tick=%d\n", hal_TimGetUpTime());

    while(step==0); // wait voc inttrupt
    
    hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);
}

HAL_ERR_T vpp_ECC_GetKeyResult(UINT32 *key, UINT count)
{
    UINT32 *ptr32;
    UINT i;
    if(step==2)
    {
        ptr32 = hal_VocGetPointer(P192_FUNC_OUT_PX);
        while(count--)
            *key++ = *ptr32++;
        vpp_ECCClose();
        step = 0;
    }
    else
        return HAL_ERR_RESOURCE_BUSY;

    return HAL_ERR_NO;
}

#endif // BT_SSP_VOC_SUPPORT
