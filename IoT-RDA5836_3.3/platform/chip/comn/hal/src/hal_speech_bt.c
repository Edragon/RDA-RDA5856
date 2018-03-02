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
/// @file hal_speech.c                                                        //
/// That file implements the HAL Speech FIFO.                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"
#include "string.h"

#include "hal_speech_bt.h"
#include "hal_sys.h"
#include "halp_debug.h"


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================

#define HAL_SPEECH_BT_RX_TX_HALF_FULL  g_halSpeechBtFifoPushRxIdx = 0; \
                                    g_halSpeechBtFifoPopRxIdx  = 0 - (HAL_SPEECH_BT_FIFO_DEPTH/2);

#define HAL_SPEECH_BT_TX_HALF_FULL     g_halSpeechBtFifoPopRxIdx  = g_halSpeechBtFifoPushRxIdx -1;

#define HAL_SPEECH_BT_RX_HALF_FULL     g_halSpeechBtFifoPushRxIdx  = g_halSpeechBtFifoPopRxIdx + 1 + (HAL_SPEECH_BT_FIFO_DEPTH/2);

#define HAL_SPEECH_BT_RX_OVF_TX_UDF    ((g_halSpeechBtFifoPushRxIdx  - g_halSpeechBtFifoPopRxIdx) >= (HAL_SPEECH_BT_FIFO_DEPTH))

#define HAL_SPEECH_BT_TX_OVF_RX_UDF    ((g_halSpeechBtFifoPushRxIdx  - g_halSpeechBtFifoPopRxIdx) == (0))

#undef HAL_FUNC_INTERNAL
#define HAL_FUNC_INTERNAL

#ifdef USE_VOC_CVSD//BUFF_CHECK_SCO
#if (USE_XCPU_CVSD == 0)
#define HAL_SPEECH_BT_FILL_INVALID_DATA   (0x55)
#else
#define HAL_SPEECH_BT_FILL_INVALID_DATA   (0x00)
#endif
#else
#define HAL_SPEECH_BT_FILL_INVALID_DATA   (0x00)
#endif

// =============================================================================
//  TYPES
// =============================================================================



// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// =============================================================================
// g_halSpeechBtFifo
// -----------------------------------------------------------------------------
/// The speech FIFO
// =============================================================================
PROTECTED HAL_SPEECH_BT_FIFO_T *g_halSpeechBtFifo;



// =============================================================================
// g_halSpeechBtFifoPushRxIdx
// -----------------------------------------------------------------------------
/// Rx push index. This is also the Tx pop index 
// =============================================================================
PRIVATE UINT32 g_halSpeechBtFifoPushRxIdx  = 0;
PRIVATE UINT32 g_halSpeechBtFifoPushRxLength  = 0;


// =============================================================================
// g_halSpeechBtFifoPopRxIdx
// -----------------------------------------------------------------------------
/// Rx pop index. This is also the Tx push index.
// =============================================================================
PRIVATE UINT32 g_halSpeechBtFifoPopRxIdx  = 0;
PRIVATE UINT32 g_halSpeechBtFifoPopRxLength  = 0;


// =============================================================================
// g_halSpeechBtFifoPushRxHappened
// -----------------------------------------------------------------------------
/// Rx push happened global reminder. (From the 'PAL' side.)
// =============================================================================
PRIVATE BOOL g_halSpeechBtFifoPushRxHappened = FALSE;



// =============================================================================
// g_halSpeechBtFifoPopRxHappened
// -----------------------------------------------------------------------------
/// Rx pop happened global reminder. (From the 'PAL' side.)
// =============================================================================
PRIVATE BOOL g_halSpeechBtFifoPopRxHappened = FALSE;




// =============================================================================
//  FUNCTIONS
// =============================================================================

//#include "string.h"
//#include "spc_mailbox.h"

#define HAL_SPEECH_BT_MAX_MUTED_COUNT 10

// Use CoolWatcher to enable/disable muting voice frame
VOLATILE UINT32 g_voiceMuteBtEnabled = 0;

VOLATILE UINT32 g_halSpeechBtMutedCount = 0;

VOID hal_Speech_BtMuteVoiceFrame(HAL_SPEECH_BT_OUT_T *popd)
{
	//dbg_TraceOutputText(0 , " hal_Speech_BtMuteVoiceFrame ");
	
}



// =============================================================================
// hal_Speech_BtFifoReset
// -----------------------------------------------------------------------------
/// Initializes the speech related mechanisms in HAL (Speech Fifo, ...)
// =============================================================================
PUBLIC VOID hal_Speech_BtFifoReset(VOID *ptr)
{
    //HAL_PROFILE_FUNCTION_ENTER(hal_Speech_BtFifoReset);
    //UINT32 i;

    //dbg_TraceOutputText(0 , " hal_Speech_BtFifoReset ");
    
    // Init global variables
    g_halSpeechBtFifoPushRxHappened = FALSE;
    g_halSpeechBtFifoPopRxHappened  = FALSE;

    g_halSpeechBtMutedCount = 0;
	
    g_halSpeechBtFifoPushRxIdx  = 0;
    g_halSpeechBtFifoPushRxLength  = 0;
    g_halSpeechBtFifoPopRxIdx  = 0;
    g_halSpeechBtFifoPopRxLength  = 0;

    g_halSpeechBtFifo = (HAL_SPEECH_BT_FIFO_T*)ptr;
    memset(g_halSpeechBtFifo, HAL_SPEECH_BT_FILL_INVALID_DATA, sizeof(HAL_SPEECH_BT_FIFO_T));
    // Ideally, pop is made HAL_SPEECH_BT_FIFO_LENGTH/2
    // behind push (and push is as far behind pop ...)
    HAL_SPEECH_BT_RX_TX_HALF_FULL;
      
    // Reset FIFO state.
    #if 0
    for (i=0 ; i<HAL_SPEECH_BT_FIFO_DEPTH ; i++)
    {
        g_halSpeechBtFifo->fifo[i].rx.codecMode = HAL_INVALID;
        g_halSpeechBtFifo->fifo[i].tx.encMode = HAL_INVALID;
    }
    #endif
    // Reset overflow status bits.
    g_halSpeechBtFifo->rxOverflowTxUnderflow = FALSE;
    g_halSpeechBtFifo->txOverflowRxUnderflow = FALSE;

    //HAL_PROFILE_FUNCTION_EXIT(hal_Speech_BtFifoReset);
}


// =============================================================================
// hal_Speech_BtFifoInvalidateRx
// -----------------------------------------------------------------------------
/// Invalidate all Rx frames from the fifo without changing their mode
// =============================================================================
INLINE VOID hal_Speech_BtFifoInvalidateRx(VOID)
{
    //dbg_TraceOutputText(0 , " hal_Speech_BtFifoInvalidateRx ");

}


// =============================================================================
// hal_Speech_BtFifoInvalidateTx
// -----------------------------------------------------------------------------
/// Invalidate all Tx frames from the fifo without changing their mode
// =============================================================================
INLINE VOID hal_Speech_BtFifoInvalidateTx(VOID)
{
    //dbg_TraceOutputText(0 , " hal_Speech_BtFifoInvalidateTx ");
}

// =============================================================================
// hal_Speech_BtPushRxPopTx
// -----------------------------------------------------------------------------
/// PAL side of the FIFO accessor. The PAL speech operation is done under FINT IRQ.
///
/// @param pushedRx Pointer to the received trame to push in the FIFO
/// @param popedTx  Pointer to the buffer where the trame to send will
/// be copied.
// =============================================================================

PUBLIC VOID HAL_FUNC_INTERNAL hal_Speech_BtPushRxPopTx(CONST UINT8* pushedRx,
                                  UINT8* popedTx,UINT16 length)
{
   // //dbg_TraceOutputText(0," push rxxxx");
    //HAL_TRACE(HAL_VOC_TRC, 0, "push rx");

   // push a valid Rx frame from PAL
   // g_halSpeechBtFifo->fifo[g_halSpeechBtFifoPushRxIdx % HAL_SPEECH_BT_FIFO_DEPTH].rx = *pushedRx;
   // pop a Tx frame from the fifo (could be invalid if g_halSpeechBtFifoPopRxHappened == 0)
   //*popedTx = g_halSpeechBtFifo->fifo[g_halSpeechBtFifoPushRxIdx % HAL_SPEECH_BT_FIFO_DEPTH].tx;
   // //dbg_TraceOutputText(0,"hal_Speech_BtPushRxPopTx %d , %d ,%d",g_halSpeechBtFifoPushRxLength,length,HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF);
    if(g_halSpeechBtFifoPushRxLength%HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF < length)
    {
		if (!g_halSpeechBtFifoPopRxHappened)
		{
			// Initial life of the FIFO: sliding mode with only pushed Rx from PAL side.
			// Pop (from VoiS side) hasn't happened yet.
			//dbg_TraceOutputText(0,"  mic buff not poped  ");
			g_halSpeechBtFifoPopRxIdx++;        
		}
		else
		{
			// If we try to push a Rx frame at a place that is still not popped, the VoiS stream is considered stopped,
			// so we go back to the initial state of the fifo (sliding mode only pushed Rx from PAL side)
			#if 1
			if  (HAL_SPEECH_BT_RX_OVF_TX_UDF)
			{
				//dbg_TraceOutputText(0,"hal_Speech_BtPushRxPopTx   Fifo reset on PushRx ");
				g_halSpeechBtFifo->rxOverflowTxUnderflow = TRUE;
				//  hal_SpeechFifoInvalidateTx();
				g_halSpeechBtFifoPopRxHappened  = FALSE;
				HAL_SPEECH_BT_TX_HALF_FULL;
			}
			#endif
		}
		
		g_halSpeechBtFifoPushRxIdx++;
    }

    if(g_halSpeechBtFifoPushRxLength+length > HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF*HAL_SPEECH_BT_FIFO_DEPTH)
    	{
		g_halSpeechBtFifoPushRxLength = 0;
				
	}

	//memset((UINT8* )&(g_halSpeechBtFifo->fifo.rx[0])+g_halSpeechBtFifoPushRxLength, 0,length);
	memcpy(g_halSpeechBtFifo->fifo.rx + g_halSpeechBtFifoPushRxLength/2, (UINT8* )pushedRx,length );
	
	//memset((UINT8* )popedTx, 0,length);
	memcpy(popedTx, g_halSpeechBtFifo->fifo.tx+g_halSpeechBtFifoPushRxLength/2 ,length );

	g_halSpeechBtFifoPushRxLength += length ;
	
    if(g_voiceMuteBtEnabled != 0)
    {
        if(g_halSpeechBtMutedCount++ < HAL_SPEECH_BT_MAX_MUTED_COUNT)
        {
            //hal_Speech_BtMuteVoiceFrame(&popedTx);
		//dbg_TraceOutputText(0,"  less than  HAL_SPEECH_");

		if(g_halSpeechBtMutedCount == HAL_SPEECH_BT_MAX_MUTED_COUNT)
		{
			//dbg_TraceOutputText(0,"  equl  HAL_SPEECH ");
		}
        }
    }

    // update fifo's Push Rx state
    g_halSpeechBtFifoPushRxHappened = TRUE;

    //HAL_PROFILE_FUNCTION_EXIT(hal_Speech_BtPushRxPopTx);

}


// =============================================================================
// hal_Speech_BtPopRxPushTx
// -----------------------------------------------------------------------------
/// VoiS side of the FIFO accessor. The VoiS speech operation is done under VoC IRQ.
///
/// @param pushedRx Pointer where to copy the received trame.
/// @param popedTx  Pointer to the trame to send to push in the FIFO.
// =============================================================================
PUBLIC VOID hal_Speech_BtPopRxPushTx(UINT8* popedRx,
                                  CONST UINT8* pushedTx)
{
    //UINT32 status;
    //HAL_TRACE(HAL_VOC_TRC, 0, "pop rx");
//   //dbg_TraceOutputText(0,"pop rx");

    // FINT priority is higher than VoC IRQ. So, only
    // VoiS can be interrupted by PAL speech operation.
    //status = hal_SysEnterCriticalSection();
    // update fifo's Pop Tx state
    if (!g_halSpeechBtFifoPushRxHappened)
    {
       //dbg_TraceOutputText(0,"hal_Speech_BtPopRxPushTx send no data");
	memset((UINT8* )popedRx, HAL_SPEECH_BT_FILL_INVALID_DATA,HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF );
	//push a valid Tx frame from VoiS
	//memset((UINT8* )&(g_halSpeechBtFifo->fifo.tx[(g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH)*HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF ]), 0,HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF);
	memcpy((UINT8* )&(g_halSpeechBtFifo->fifo.tx[(g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH)*HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF ]), (UINT8* )pushedTx,HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF);
	//g_halSpeechBtFifo->fifo[g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH].tx = *pushedTx;

	//update fifo's Push Tx state
	g_halSpeechBtFifoPopRxHappened = TRUE;
	g_halSpeechBtFifoPopRxIdx++;

	// Initial life of the FIFO: only pushed Rx ("sliding" mode).
	// Pop (from VOC side) hasn't happened yet.
	//g_halSpeechBtFifoPushRxIdx++;

	return ;
    }
    else
    {
        // If one pointer is too fast (underflow), we go back to the "sliding"
        // mode with a reset of the fifo. This should not be interrupted.
    }

    // pop a Rx frame from the fifo (invalid if g_halSpeechBtFifoPushRxHappened == 0)
    //*popedRx = g_halSpeechBtFifo->fifo.rx[g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH];
  
    //hal_Speech_BtTraceRx(popedRx);
       memset((UINT8* )popedRx, HAL_SPEECH_BT_FILL_INVALID_DATA,HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF );


	
   if(g_halSpeechBtFifoPushRxHappened&&!g_voiceMuteBtEnabled) //if no valid rx data, just set the memory zero 
	    memcpy((UINT8* )popedRx, (UINT8* )(g_halSpeechBtFifo->fifo.rx+(g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH)*HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF/2 ),HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF );

	//push a valid Tx frame from VoiS
   // memset((UINT8* )&(g_halSpeechBtFifo->fifo.tx[(g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH)*HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF ]), 0,HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF);
    memcpy((UINT8* )(g_halSpeechBtFifo->fifo.tx+(g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH)*HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF/2), pushedTx,HAL_SPEECH_BT_FRAME_SIZE_PCM_BUF);
    //g_halSpeechBtFifo->fifo[g_halSpeechBtFifoPopRxIdx % HAL_SPEECH_BT_FIFO_DEPTH].tx = *pushedTx;

    //update fifo's Push Tx state
    g_halSpeechBtFifoPopRxHappened = TRUE;
    g_halSpeechBtFifoPopRxIdx++;
    
    //hal_SysExitCriticalSection(status);

    //HAL_PROFILE_FUNCTION_EXIT(hal_Speech_BtPopRxPushTx);
}


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================
    
INT32* hal_Speech_BtGetRxAddr(VOID)
{
	return (INT32*)(g_halSpeechBtFifo->fifo.rx);
}
INT32* hal_Speech_BtGetTxAddr(VOID)
{
	return (INT32*)(g_halSpeechBtFifo->fifo.tx);
}




