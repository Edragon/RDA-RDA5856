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

/*
 * FileName:       drv_spi_chip.c
 * Author:         zhengjing  Version: 1  Date: 2006-7-4
 * Description:    
 * Version:        
 * Function List:  
 *                 1. DMA mode  len must <8k
 			 2. Direct mode  len must <=16
 			 3. No DMA interrupt
 			 4. must read the data in Direct interrupt to clear the interrupt flag  
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include <cswtype.h>
#include <errorcode.h>
#include "chip_id.h"

#include "hal_spi.h"
#include "hal_sys.h"

#include "csw_csp.h"

#include <cos.h>
#include "drv.h"
#include "ts.h"
//#include "spi.h"


// FIXME Add the support for the second SPI !

DRV_SpiCs_t gSPI_Cs = 0;
//extern unsigned long CSW_TRACE(unsigned short nIndex, char * pFormat,...);
extern BOOL COS_SendSysEvent(
  COS_EVENT* pEvent,
  UINT16 nOption
);


#define  	EV_CSW_PRV_SPI      0xf0000001+8


VOID DRV_SpiIsr(UINT16 status);

UINT32 DRV_SPIInit( DRV_SpiCfg*spicfg)
{
	if (spicfg==NULL) return FALSE;
  if (spicfg->irqHandler == NULL) 
      spicfg->irqHandler = DRV_SpiIsr;
    HAL_SPI_CFG_T config;

    config.enabledCS = spicfg->enabledCS;  
    config.csActiveLow = spicfg->polarityCS;
    config.inputEn = spicfg->inputEn;
    config.clkFallEdge = spicfg->clkFallEdge;
    config.clkDelay = spicfg->clkDelay;
    config.doDelay = spicfg->doDelay;
    config.diDelay = spicfg->diDelay;
    config.csDelay = spicfg->csDelay;
    config.csPulse = spicfg->csPulse;
    config.frameSize = spicfg->frameSize;
    config.oeRatio = 0; // TODO Not supported by CSW yet.

    // Safer equivalent:
    config.spiFreq = (HAL_SYS_FREQ_52M/spicfg->clkDivider);

    config.rxTrigger = spicfg->rxTrigger;
    config.txTrigger = spicfg->txTrigger;
    config.rxMode = spicfg->rxMode;
    config.txMode = spicfg->txMode;

    config.mask.rxOvf       = (spicfg->irqMask & DRV_SPI_IRQ_MSK_RX_OVF);
    config.mask.txTh        = (spicfg->irqMask & DRV_SPI_IRQ_MSK_TX_TH);
    config.mask.txDmaDone   = (spicfg->irqMask & DRV_SPI_IRQ_MSK_TX_DMA);
    config.mask.rxTh        = (spicfg->irqMask & DRV_SPI_IRQ_MSK_RX_TH);
    config.mask.rxDmaDone   = (spicfg->irqMask & DRV_SPI_IRQ_MSK_RX_DMA);
    
    // FIXME Handler seems compatible
    config.handler = (HAL_SPI_IRQ_HANDLER_T)spicfg->irqHandler; 
    hal_SpiOpen(HAL_SPI, config.enabledCS, &config);
    return TRUE;
}

VOID DRV_SPIExit(DRV_SpiCs_t cs)
{
      hal_spi_Close(cs);
}

UINT32 DRV_SPIWrite(DRV_SpiCs_t cs,UINT8 *data, UINT16 len)
{
	UINT16 count=0;
	UINT16 num=0;
  gSPI_Cs = cs;
  
	if(data==NULL)	return FALSE;

  if(hal_spi_ActivateCs(cs) == FALSE)
    return FALSE;
	while((count = hal_spi_SendData(cs,data, len))==0); 
	
	while(count !=len)
	 {
		count +=hal_spi_SendData(cs,data+count, len - count);
		if ( num++>100)
			{
				CSW_TRACE(CSW_DRV_TS_ID,"spi write error\n");
        hal_spi_DeActivateCs(cs);
				return FALSE;
			}
	 }
  hal_spi_DeActivateCs(cs);
	return count;
}
UINT32 DRV_SPIRead(DRV_SpiCs_t cs,UINT8 *outbuf,UINT16 len)
{
  UINT16 count = 0;
  UINT16 num   = 0;

  gSPI_Cs = cs;
  if (outbuf == NULL)
    return FALSE;
  if(hal_spi_ActivateCs(cs)== FALSE)
    return FALSE;

  while ((count = hal_spi_GetData(cs, outbuf, len)) == 0)
    ;

  while (count != len)
  {
    count += hal_spi_GetData(cs, outbuf + count, len - count);
    if (num++ > 100)
    {
      CSW_TRACE(CSW_DRV_TS_ID, "spi read error\n");
      hal_spi_DeActivateCs(cs);
      return FALSE;
    }
  }

hal_spi_DeActivateCs(cs);

return count;
}

UINT8 DRV_Spi_TxFinished(DRV_SpiCs_t cs)
 {
	return hal_spi_TxFinished(cs);
 }

void DRV_SPI_ChangeCs( DRV_SpiCfg *spicfg)
{
	//hal_spi_ChangeCs((struct hal_SpiCfg *)spicfg);
}

volatile UINT8 SpiIsrBuf[64];
volatile UINT8 bufIn=0;
volatile UINT8 bufOut=0;
volatile UINT8 DataNb=0;

UINT32 DRV_GetSpiIsrFifoLen(void)
{
	return DataNb;
}
UINT32 DRV_ISRSpiRead(unsigned char *add ,unsigned int len)
{
	UINT32 i;
	if (add==NULL)  return 0;
	if(len>DataNb)  return 0;
	
 	for(i=0;i<len;i++)
 	{
		*(add+i)=*(SpiIsrBuf+(bufOut+i)%64);
 	}
	bufOut=(bufOut+len)%64;
	
	DataNb=(bufIn>=bufOut) ?(bufIn-bufOut):(64-bufOut+bufIn);
	return 1;
}

VOID DRV_SpiIsr(UINT16 status)
{
  COS_EVENT ev;

    union
    {
        HAL_SPI_IRQ_STATUS_T bfStatus;
        UINT32 reg;
    }u;
    u.reg = status;
  if (u.bfStatus.txTh)
  {
    HAL_SPI_IRQ_STATUS_T newStatus=
    {
        .rxOvf      = 0,
        .txTh       = 0,
        .txDmaDone  = 0,
        .rxTh       = 0,
        .rxDmaDone  = 0
    };
    hal_SpiIrqSetMask(HAL_SPI, gSPI_Cs, newStatus);
  }

    if (u.bfStatus.rxTh || u.bfStatus.rxOvf)
  {
    while (1)
    {
      if (hal_spi_GetData(gSPI_Cs, (UINT8*)(SpiIsrBuf + bufIn), 1) == 0)
        break;
      else
        bufIn = (bufIn + 1) % 64;
    }

//    ev.nParam2 = (bufIn >= bufOut) ? (bufIn - bufOut) : (64 - bufOut + bufIn);//for delete nParam2
//    DataNb     = ev.nParam2;//for delete nParam2
        DataNb =  (bufIn >= bufOut) ? (bufIn - bufOut) : (64 - bufOut + bufIn);
  }

  ev.nEventId = EV_CSW_PRV_SPI;
  ev.nParam1  = status;

  COS_SendSysEvent(&ev, 0);

}
VOID DRV_SPISetISRmask(UINT16 mask)
{
    union
    {
        HAL_SPI_IRQ_STATUS_T bfStatus;
        UINT32 reg;
    }u;
    u.reg = mask;
    
    hal_SpiIrqSetHandler(HAL_SPI, gSPI_Cs, (HAL_SPI_IRQ_HANDLER_T)DRV_SpiIsr);
    // FIXME mask *should* be compatible
    hal_SpiIrqSetMask(HAL_SPI, gSPI_Cs, u.bfStatus);
}

