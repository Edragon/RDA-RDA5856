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
/* resamplesubs.c - sampling rate conversion subroutines */
// Altered version

#include "resample.h"
#include "mcip_debug.h"

   
static UINT16  g_Nx;    
static UINT32 g_dtb;

static INT16 g_OldSample=0;


 /* Sampling rate conversion using linear interpolation for maximum speed. */
 INT32   SrcLinear(INT16 X[], INT16 Y[], UINT32 dtb , UINT32 *Time, UINT16 Nx, INT16 OldX)
{    
	INT16 iconst;    
	INT16 *Xp, *Ystart;    
	INT32 v,x1,x2;        
	UINT32 endTime;              /* When Time reaches EndTime, return to user */        
	
	Ystart = Y;    
	endTime = (1<<Np)*(WORD)Nx;    

	if(*Time < endTime)   
	{       
		iconst = (*Time) & Pmask;       

		if((*Time)>>Np==0)
		{
			x1 = OldX;        
			x2 = X[0];        			
		}
		else
		{		
			Xp = &X[((*Time)>>Np)-1];      /* Ptr to current input sample */  			
			x1 = *Xp++;        
			x2 = *Xp;        
		}
		x1 *= ((1<<Np)-iconst);       
		x2 *= iconst;        
		v = x1 + x2;        
		*Y++ = v>>Np;   /* Deposit output */        
		*Time += dtb;               /* Move to next sample by time increment */    
	}   
	
	while (*Time < endTime)   
	{       
		iconst = (*Time) & Pmask;       
		Xp = &X[((*Time)>>Np)-1];      /* Ptr to current input sample */        
		x1 = *Xp++;        
		x2 = *Xp;        
		x1 *= ((1<<Np)-iconst);       
		x2 *= iconst;        
		v = x1 + x2;        
		*Y++ = v>>Np;   /* Deposit output */        
		*Time += dtb;               /* Move to next sample by time increment */    
	}   
	return (Y - Ystart);            /* Return number of output samples */
}


INT32 ResampleInit(INT32 InSampleRate,INT32 OutSampleRate,INT32 FrameLength,INT32 NbChnanel)
{
	INT32 NumOfOutSample;
	
	g_Nx = (FrameLength/2) ; 
		
	g_Nx=g_Nx/NbChnanel;

	NumOfOutSample=(g_Nx*OutSampleRate)/InSampleRate;

	if((NumOfOutSample&0x7)>4)
	{
		NumOfOutSample=(NumOfOutSample&0xfff8)+0x8;			
	}
	else
	{
		NumOfOutSample=NumOfOutSample&0xfff8;
	}

	g_dtb=((g_Nx*(1<<Np))/NumOfOutSample);	

	if((g_Nx*(1<<Np))>(g_dtb*NumOfOutSample)) 
	{
		g_dtb=g_dtb+1;		
	}
	
	MCI_TRACE (TSTDOUT,0,"g_NubOfOutSample = %d",NumOfOutSample);  

	MCI_TRACE (TSTDOUT,0,"g_dtb = %d",g_dtb);  
	

   	return NumOfOutSample;
}



INT32 ResampleOneFrame(INT16 *InBuf,INT16 * OutBuf)
{
	INT32 NumOfOutSample;
		
	UINT32 Time;          /* Current time/pos in input sample */ 
		
	Time = 0;          /* Current-time pointer for converter */ 

	/* Resample stuff in input buffer */        

	NumOfOutSample=SrcLinear(InBuf,OutBuf,g_dtb,&Time,g_Nx,g_OldSample);        


	Time -= (g_Nx<<Np);       // Move converter Nx samples back in time      

	g_OldSample = InBuf[g_Nx-1]; // that must be re-used         
 
	return NumOfOutSample;
}



