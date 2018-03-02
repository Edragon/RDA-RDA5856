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

                                                                      
// ***********************************************************************
//                                                                        
//  FILE NAME                                                             
//                                                                        
//       myl1.c                                                           
//                                                                        
//  DESCRIPTION                                                           
//                                                                        
//       Reduced L1 source code.                                          
//                                                                        
//                                           
//                                                                        
// ***********************************************************************

#include "cs_types.h"

#include "pal_gsm.h"

#include "calib_m.h"


//  ----------------- L1 relative defines.  

#define L1_FCH_WIN_QB_POS (5000 - 625 - PAL_PLL_LOCK_TIME)

#define L1S_SACCH0_BUFFER  4
#define L1S_TCH0_BUFFER    1
#define L1S_PDCH_BUFFER    0
#define L1S_PTCCH_BUFFER   4
#define L1S_SDCCH_BUFFER   4

#define L1S_FCH_WIN_IDX    0
#define L1S_SCH_WIN_IDX    0
#define L1S_RX_NB_WIN_IDX  2
#define L1S_TX_NB_WIN_IDX  1

#define TCH_BURST(i)    ((1 << 3) | i) // Time to program burst i for reception.
#define SL1_BST(i)        ((1 << 3) | i) // Time to program burst i for reception.
#define FRAME_DECODE    (1 << 4)    // Time to read the decoding result.
#define SL1_DEC            (1 << 4)    // Time to read the decoding result.
#define FRAME_ENCODE    (1 << 6)    // Time to encode

typedef enum {
    FOF_FOUND,
    FOF_SEARCH,
    FOF_NOT_FOUND
} myFOf_t;



//  ----------------- The global context variable. 

//  Pointer to the context, uncached. 
EXPORT VOLATILE CALIB_STUB_CTX_T* g_calibStubGlobalCtxPtr;

//  Local context, used to pass some variables. 
EXPORT CALIB_STUB_CTX_T g_calibStubLocalCtx;



//  ----------------- The L1 functions. 

//  Initialize PAL with the selected band. 
VOID myl1_BandInit(UINT8 Band);

//  Functions used for Rx statistics. 
VOID myl1_Synch(VOID);
UINT32  myl1_Fn(UINT16 T1, UINT8 T2, UINT8 T3);
VOID myl1_Idle(VOID);
VOID myl1_GetNBEqualisationResult(pal_NBurstResult_t *NBurstResult);

//  Monitor the frequency on a specified ARFCN and Band. 
myFOf_t myl1_GetFOf(BOOL init, UINT16 Arfcn, UINT8 Band, UINT8 ExpPower, INT32 *MeanFOf);

//  Monitor the power on a specified ARFCN and Band and Expected Power. 
BOOL myl1_Monit(UINT16 Arfcn, UINT8 Band, UINT8 ExpPower, UINT8 T2,
                UINT8 *MonPower, UINT8 *NBPower);

//  Send bursts of modulated stuff. 
VOID myl1_SendBurst(UINT16 Arfcn, UINT8 Pcl, UINT8 Band, UINT32 Tsc);

//  Send bursts of modulated stuff, at Dac power (index or value). 
VOID myl1_SendBurstDac(UINT16 Arfcn, UINT16 DacId, UINT8 Band, UINT32 Tsc);



//  ----------------- Misc functions. 

VOID my_msg2str(UINT8* msgp,char* strp,UINT8 size);

