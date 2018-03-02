/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    string.c		                                              */
/* Description:                                                               */
/*					Utility phone reference definitions.                      */
/*                                                                            */
/* Revision history:                                                          */
/*   Name            Date             Remaks                                  */
/*                                                                            */
/*   li   xingpeng    2005/09/14         version 1.0                          */
/*                    Define Utility string reference basic types.            */
/******************************************************************************/
#include <sul.h>

//
//GSMBCD码,如果转化奇数个数字的话，将会在对应高位补F
//支持'*''#''p'
//13811189836 --> 0x31 0x18 0x11 0x98 0x38 0xF6
//
UINT16 SUL_AsciiToGsmBcd(
    INT8 * pNumber, // input
    UINT8 nNumberLen,
    UINT8 * pBCD // output should >= nNumberLen/2+1
)
{
    UINT8 Tmp;
    UINT32 i;
    UINT32 nBcdSize = 0;
    UINT8 * pTmp = pBCD;
	
    if(pNumber == (CONST INT8 *)NULL || pBCD == (UINT8 *)NULL)
        return FALSE;
	
    SUL_MemSet8(pBCD, 0, nNumberLen>>1);
    
    for(i = 0; i < nNumberLen; i++)
    {
        switch(*pNumber)
        {
        case '*':
            Tmp = (INT8)0x0A;
            break;
        case '#':
            Tmp = (INT8)0x0B;
            break;
         case 'p':
            Tmp = (INT8)0x0C;
            break;
        //--------------------------------------------
        //Modify by lixp 20070626 begin
        //--------------------------------------------
        case 'w':
            Tmp = (INT8)0x0D;
            break;       
        case '+':
            Tmp = (INT8)0x0E;
            break;       
        //--------------------------------------------
        //Modify by lixp 20070626 end
        //-------------------------------------------- 
        default:
            Tmp = (INT8)(*pNumber - '0');
            break;
        }
        if(i % 2)
        {
            *pTmp++ |= (Tmp << 4) & 0xF0;
        }
        else
        {
            *pTmp = (INT8)(Tmp & 0x0F);
        }
        pNumber++;
    }
    
    nBcdSize = nNumberLen>>1;
    
    if(i % 2)
    {
        *pTmp |= 0xF0;
        nBcdSize += 1;
    }
    
    return nBcdSize;
}


//
// 该函数会去掉最后的F，同时将A转化为‘*’将B转化为‘#’将C转化为‘p’
//  --> 13811189836 
//
UINT16 SUL_GsmBcdToAscii (
    UINT8 * pBcd, // input 
    UINT8 nBcdLen,
    UINT8 * pNumber // output
)
{
    UINT8 i;  
	UINT8 * pTmp = pNumber;
    UINT16 nSize = nBcdLen*2;
    
	for(i = 0; i < nBcdLen; i++)
	{
		if((pBcd[i] & 0x0F) == 0x0A)
		{
			*pTmp = '*';
		}
		else if((pBcd[i] & 0x0F) == 0x0B)
		{
			*pTmp ='#';
		}
		else if((pBcd[i] & 0x0F) == 0x0C)
		{
			*pTmp ='p';
		}
    // --------------------------------------------
    // Modify by lixp 20070626 begin
    // --------------------------------------------

    else if ((pBcd[i] & 0x0F) == 0x0D)
    {
        *pTmp = 'w';
    }
    else if ((pBcd[i] & 0x0F) == 0x0E)
    {
        *pTmp = '+';
    }
    // --------------------------------------------
    // Modify by lixp 20070626 end
    // -------------------------------------------- 

		else
		{
			*pTmp =(pBcd[i] & 0x0F) + 0x30;	
		}
		pTmp ++;
		if((pBcd[i])>>4 == 0x0A)
		{
			*pTmp = '*';
		}
		else if((pBcd[i])>>4 == 0x0B)
		{
			*pTmp ='#';
		}
		else if((pBcd[i])>>4 == 0x0C)
		{
			*pTmp ='p';
		}
    // --------------------------------------------
    // Modify by lixp 20070626 begin
    // -------------------------------------------- 

    else if((pBcd[i])>>4 == 0x0D)
		{
			*pTmp ='w';
		}
    else if((pBcd[i])>>4 == 0x0E)
		{
			*pTmp ='+';
		}
    // --------------------------------------------
    // Modify by lixp 20070626 end
    // -------------------------------------------- 

		else
		{
			*pTmp = (pBcd[i]>>4) + 0x30;	
		}
		pTmp++;
	}
    
	pTmp--;
	if(*pTmp == 0x3f)
	{
		*pTmp = 0x00;
        nSize -= 1;
	}

    return nSize;
}  
UINT16 SUL_AsciiToGsmBcdEx(
    INT8 * pNumber, // input
    UINT8 nNumberLen,
    UINT8 * pBCD // output should >= nNumberLen/2+1
)
{
    UINT8 Tmp;
    UINT32 i;
    UINT32 nBcdSize = 0;
    UINT8 * pTmp = pBCD;
	
    if(pNumber == (CONST INT8 *)NULL || pBCD == (UINT8 *)NULL)
        return FALSE;
	
    SUL_MemSet8(pBCD, 0, nNumberLen>>1);
    
    for(i = 0; i < nNumberLen; i++)
    {
        switch(*pNumber)
        {
        case 0x41:
		case 0x61:		
            Tmp = (INT8)0x0A;
            break;
        case 0x42:
		case 0x62:		
            Tmp = (INT8)0x0B;
            break;
         case 0x43:
		 case 0x63:		
            Tmp = (INT8)0x0C;
            break;
        //--------------------------------------------
        //Modify by lixp 20070626 begin
        //--------------------------------------------
        case 0x44:
		case 0x64:		
            Tmp = (INT8)0x0D;
            break;       
        case 0x45:
		case 0x65:		
            Tmp = (INT8)0x0E;
            break;               
		case 0x46:
		case 0x66:	
            Tmp = (INT8)0x0F;
            break;   
        //--------------------------------------------
        //Modify by lixp 20070626 end
        //-------------------------------------------- 
        default:
            Tmp = (INT8)(*pNumber - '0');
            break;
        }
        if(i % 2)
        {
            *pTmp++ |= (Tmp << 4) & 0xF0;
        }
        else
        {
            *pTmp = (INT8)(Tmp & 0x0F);
        }
        pNumber++;
    }
    
    nBcdSize = nNumberLen>>1;
    
    if(i % 2)
    {
        *pTmp |= 0xF0;
        nBcdSize += 1;
    }
    
    return nBcdSize;
}

UINT16 SUL_GsmBcdToAsciiEx (
    UINT8 * pBcd, // input 
    UINT8 nBcdLen,
    UINT8 * pNumber // output
)
{
    UINT8 i;  
	UINT8 * pTmp = pNumber;
    UINT16 nSize = nBcdLen*2;
    
	for(i = 0; i < nBcdLen; i++)
	{
		if((pBcd[i] & 0x0F) == 0x0A)
		{
			*pTmp = 0x41;
		}
		else if((pBcd[i] & 0x0F) == 0x0B)
		{
			*pTmp =0x42;
		}
		else if((pBcd[i] & 0x0F) == 0x0C)
		{
			*pTmp =0x43;
		}
	    // --------------------------------------------
	    // Modify by lixp 20070626 begin
	    // --------------------------------------------

	    else if ((pBcd[i] & 0x0F) == 0x0D)
	    {
	        *pTmp = 0x44;
	    }
	    else if ((pBcd[i] & 0x0F) == 0x0E)
	    {
	        *pTmp = 0x45;
	    }
	    // --------------------------------------------
	    // Modify by lixp 20070626 end
	    // -------------------------------------------- 
	    else if ((pBcd[i] & 0x0F) == 0x0F)
	    {
	        *pTmp = 0x46;
	    }

		else
		{
			*pTmp =(pBcd[i] & 0x0F) + 0x30;	
		}
		
		pTmp ++;
		if((pBcd[i])>>4 == 0x0A)
		{
			//*pTmp = 0x61;
			*pTmp = 0x41;
		}
		else if((pBcd[i])>>4 == 0x0B)
		{
			//*pTmp =0x62;
			*pTmp = 0x42;
		}
		else if((pBcd[i])>>4 == 0x0C)
		{
			//*pTmp =0x63;
			*pTmp = 0x43;
		}
	    // --------------------------------------------
	    // Modify by lixp 20070626 begin
	    // -------------------------------------------- 

	    else if((pBcd[i])>>4 == 0x0D)
		{
				//*pTmp =0x64;
				*pTmp = 0x44;
		}
	    else if((pBcd[i])>>4 == 0x0E)
		{
				//*pTmp =0x65;
				*pTmp = 0x45;
		}
		else if((pBcd[i])>>4 == 0x0F)
		{
				//*pTmp =0x66;
				*pTmp = 0x46;
		}
	    // --------------------------------------------
	    // Modify by lixp 20070626 end
	    // -------------------------------------------- 

		else
		{
			*pTmp = (pBcd[i]>>4) + 0x30;	
		}
		pTmp++;
	}
    
	pTmp--;
	if(*pTmp == 0x3f)
	{
		*pTmp = 0x00;
        nSize -= 1;
	}

    return nSize;
}


