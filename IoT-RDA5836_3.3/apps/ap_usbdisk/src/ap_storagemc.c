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


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "umss_config.h"

//#include "sxr_mem.h"
//#include "sxs_io.h"

#include "mcd_m.h"

//#include "hal_host.h"

#include <string.h>
#include "event.h"
#include "cos.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// UMSS_STORAGE_MC_HANDLER_T
// -----------------------------------------------------------------------------
/// Mc handler
// =============================================================================
typedef struct
{
    UMSS_STORAGE_SIZE_T  size;
} UMSS_STORAGE_MC_HANDLER_T;

// =============================================================================
// PROTOTYPE FUNCTIONS                                                          
// =============================================================================

PRIVATE VOID*
umss_StorageMcOpen   (VOID);

PRIVATE VOID
umss_StorageMcClose  (VOID* handler);

PRIVATE INT32
umss_StorageMcRead   (VOID* handler, VOID* data, UINT32 lba, UINT32 size);

PRIVATE INT32
umss_StorageMcWrite  (VOID* handler, VOID* data, UINT32 lba, UINT32 size);

PRIVATE UMSS_STORAGE_SIZE_T*
umss_StorageMcGetsize(VOID* handler);

PRIVATE BOOL
umss_StorageMcPresent(VOID* handler);

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

PUBLIC  UMSS_STORAGE_CALLBACK_T g_umssStorageMcCallback = {
    .open    = umss_StorageMcOpen,
    .close   = umss_StorageMcClose,
    .read    = umss_StorageMcRead,
    .write   = umss_StorageMcWrite,
    .getsize = umss_StorageMcGetsize,
    .present = umss_StorageMcPresent
};

// =============================================================================
// PRIVATE FUNCTIONS                                                            
// =============================================================================

PRIVATE BOOL umss_StorageMcPresent(VOID* handler)
{
    UMSS_STORAGE_MC_HANDLER_T* handlerMc;
    MCD_CSD_T                  csd;
    MCD_CARD_SIZE_T            cardSize;

    //hal_HstSendEvent(SYS_EVENT,0x04250031);
    //hal_HstSendEvent(SYS_EVENT,mcd_CardStatus());
    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;
    switch(mcd_CardStatus())
    {
    case MCD_STATUS_NOTOPEN_PRESENT:
    //hal_HstSendEvent(SYS_EVENT,0x04250030);

    #ifndef MCD_SPI_SUPPORT 
        if(mcd_Open(&csd, MCD_CARD_V2, FALSE, FALSE) != MCD_ERR_NO)
        {
            if(mcd_Open(&csd, MCD_CARD_V1, FALSE, FALSE) != MCD_ERR_NO)
            {
                if(mcd_Open(&csd, MCD_CARD_V2, TRUE, FALSE) != MCD_ERR_NO)
                {
                    if(mcd_Open(&csd, MCD_CARD_V1, TRUE, FALSE) != MCD_ERR_NO)
                    {
                        return FALSE;
                    }
                }
            }
        }
     #else
         if(mcd_Open(&csd) != MCD_ERR_NO)
             return FALSE; 
     #endif
        // Caution: There is no break here!
        // The following statement is executed as well.
    case MCD_STATUS_OPEN:
    //hal_HstSendEvent(SYS_EVENT,0x04240002);
    //hal_HstSendEvent(SYS_EVENT,handlerMc->size.sizeBlock);
    //hal_HstSendEvent(SYS_EVENT,handlerMc->size.nbBlock);
        mcd_GetCardSize(&cardSize);
        handlerMc->size.sizeBlock = cardSize.blockLen;
        handlerMc->size.nbBlock   = cardSize.nbBlock;
        return TRUE;
    case MCD_STATUS_NOTPRESENT:
        //return FALSE;
        return TRUE;
    case MCD_STATUS_OPEN_NOTPRESENT:
        // ensure the mcd is closed
        //mcd_Close();
        //return FALSE;
        return TRUE;
    }
    return(TRUE);
}

PRIVATE VOID*  umss_StorageMcOpen()
{
    UMSS_STORAGE_MC_HANDLER_T *handlerMc;

    //cfg = cfg;

    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*)
        COS_Malloc(sizeof(UMSS_STORAGE_MC_HANDLER_T));

	memset(handlerMc, 0, sizeof(UMSS_STORAGE_MC_HANDLER_T));
    //hal_HstSendEvent(SYS_EVENT,0x04240003);
    //hal_HstSendEvent(SYS_EVENT,handlerMc);
    return((VOID*)(handlerMc));
}

PRIVATE VOID   umss_StorageMcClose(VOID* handler)
{
    mcd_Close();

    if(handler)
    {
        COS_Free(handler);
        handler=NULL;
    }
}

PRIVATE INT32 umss_StorageMcRead(VOID* handler, VOID* data,
                                 UINT32 lba, UINT32 nbBlock)
{
    UMSS_STORAGE_MC_HANDLER_T* handlerMc;
    UINT32                     size;
    MCD_ERR_T                  ret;

    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;

    size             = handlerMc->size.sizeBlock*nbBlock;
   
    //ret = mcd_Read(lba*handlerMc->size.sizeBlock, data, size);
    ret = mcd_Read(lba, data, size);
    //hal_HstSendEvent(SYS_EVENT,0x04250020);
    //hal_HstSendEvent(SYS_EVENT,lba);
    //hal_HstSendEvent(SYS_EVENT,nbBlock);
    //hal_HstSendEvent(SYS_EVENT,handlerMc->size.sizeBlock);
    //hal_HstSendEvent(SYS_EVENT,ret);
    //while(1);
    if(ret != MCD_ERR_NO)
    {
        switch(ret)
        {
        case MCD_ERR_DMA_BUSY:
        case MCD_ERR_SPI_BUSY:
            return(-2);
        case MCD_ERR_CARD_TIMEOUT:
        case MCD_ERR_BLOCK_LEN:
        case MCD_ERR_CARD_NO_RESPONSE:
        case MCD_ERR_CARD_RESPONSE_BAD_CRC:
        case MCD_ERR_CMD:
        case MCD_ERR_UNUSABLE_CARD:
        default:
            mcd_Close();
            return(-1);
        }
    }
    return(nbBlock);
}

PRIVATE INT32 umss_StorageMcWrite(VOID* handler, VOID* data,
                                  UINT32 lba, UINT32 nbBlock)
{
    UMSS_STORAGE_MC_HANDLER_T*  handlerMc;
    UINT32                      size;
    MCD_ERR_T                   ret;

    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;

    size             = handlerMc->size.sizeBlock*nbBlock;

    //ret = mcd_Write(lba*handlerMc->size.sizeBlock, data, size);
    ret = mcd_Write(lba, data, size);
    if(ret != MCD_ERR_NO)
    {
        switch(ret)
        {
        case MCD_ERR_DMA_BUSY:
        case MCD_ERR_SPI_BUSY:
            return(-2);
        case MCD_ERR_CARD_TIMEOUT:
        case MCD_ERR_BLOCK_LEN:
        case MCD_ERR_CARD_NO_RESPONSE:
        case MCD_ERR_CARD_RESPONSE_BAD_CRC:
        case MCD_ERR_CMD:
        case MCD_ERR_UNUSABLE_CARD:
        default:
            mcd_Close();
            return(-1);
        }
    }
    return(nbBlock);
}

PRIVATE UMSS_STORAGE_SIZE_T* umss_StorageMcGetsize(VOID* handler)
{
    UMSS_STORAGE_MC_HANDLER_T* handlerMc;

    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;

    return(&handlerMc->size);
}

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================
