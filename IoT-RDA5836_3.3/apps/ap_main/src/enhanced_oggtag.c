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

#include "ap_common.h"


const UINT8 ogg_TagInfor_keyword[8]       = {0x07,0x03,0x56,0x4f,0x52,0x42,0x49,0x53};// 0x03+"VOBIS"
const UINT8 ogg_Tag_TITLE_keyword[7]    = {0x06,'T','I','T','L','E','='};//title 第一个字节表示长度
const UINT8 ogg_Tag_ARTIST_keyword[8] = {0x07,'A','R','T','I','S','T','='};//ARIST 第一个字节表示长度
const UINT8 ogg_Tag_ALBUM_keyword[7] = {0x06,'A','L','B','U','M','='};//album  第一个字节表示长度

HANDLE ogg_TagFileHandle;
BOOL   ogg_TAGInfoFlag;//为真表示有TAG信息
UINT32  ogg_TagInforlen;//TAG信息的长度
UINT32  ogg_TagInforCount;//TAG信息的项数

//fsb file system use
INT32  FBS_bytepos;//文件位置在当前页中的偏移
UINT32 FBS_pagepos;//文件位置页数
UINT32 FBS_filepos;//文件位置的byte 数
UINT32 FBS_filelen;//文件长度(UINT8 数)
UINT8  FBS_TmpBuf[512];


BOOL GetAllItemInfor(ID3Info_t *ID3Info);
BOOL  ReadLongData(UINT32 *longtypeout);
BOOL oggSearchMem(const INT8 *memBuff, UINT32 scnt);
BOOL  FSeekByte(UINT32 ByteOffset,  HANDLE hFile );
BOOL  FReadByte(INT8 *pdata, HANDLE hFile);
UINT32 FTellByte(HANDLE hFile);


/*********************************************************************************
*      BOOL  ReadLongData(long *longtypeout)
*
* Description :读一个LONG型数据
* 
* input:  long *longtypeout//输出的long 型数据
*
* BOOL return: TRUE: 输出的数据有效
*                 FALSE: 已读到文件尾                
*
*********************************************************************************/
BOOL  ReadLongData(UINT32 *longtypeout)
{
    INT i;
    UINT8 Tmp[4];//用于转换LONG型数据的BUF
    
    for (i=0; i<4; i++)
    {    
        if (!FReadByte(&Tmp[i], ogg_TagFileHandle))
        {
            return FALSE;
        }        
    }
    *longtypeout = *(long *)Tmp;
    return TRUE;
}

//=============================================================
/*
    for UINT8 file system
    
*/
//=============================================================

HANDLE FOpenByte(INT8 *filename, INT8 mod)
{
    HANDLE fhandle;
      
    FBS_bytepos = 0;
    FBS_pagepos = 0;
    FBS_filepos = 0;
    fhandle = FS_Open(filename, mod,0);    
	
    if (fhandle == NULL)
    {
        return NULL;
    }

	//计算出文件长度
	FBS_filelen = FS_GetFileSize(fhandle);

	FS_Read(fhandle,FBS_TmpBuf,512); //读一PAGE数据 

	return fhandle;      
        
}

BOOL  FSeekByte(UINT32 ByteOffset,  HANDLE hFile )
{
    long PageOffset;

    if (ByteOffset >= FBS_filelen)
    {
        //大于文件长度
        return FALSE;
    }

    FBS_filepos = ByteOffset;
    PageOffset = ByteOffset/512;
    FBS_bytepos = ByteOffset%512; 
    
    if (FBS_pagepos == PageOffset)
    {
        //同一PAGE 不用读
        return TRUE;
    }
    FBS_pagepos = PageOffset;

	
    if(ERR_SUCCESS!=FS_Seek(hFile,FBS_pagepos,FS_SEEK_SET))
    {
        return FALSE;
    }	
    return (ERR_SUCCESS==FS_Read(hFile,FBS_TmpBuf,512));//读一PAGE数据          
        
}

BOOL  FReadByte(INT8 *pdata, HANDLE hFile)
{
    *pdata = FBS_TmpBuf[FBS_bytepos];
     FBS_filepos++;
     
    if (FBS_bytepos >= 512-1)
    {
        //如数据不够就读一个PAGE数据
        if (ERR_SUCCESS!=FS_Read(hFile,FBS_TmpBuf,512)) //读一PAGE数据                
        {
            return FALSE; // --->> FALSE
        }
        FBS_bytepos = 0;
    }
    else
    {
         FBS_bytepos++;
    }            
    return TRUE;
}


UINT32 FTellByte(HANDLE hFile)
{
    return FBS_filepos;
}

/*********************************************************************************
*              BOOL GetAllItemInfor(ID3Info_t *ID3Info)
* Description : 取所有信息  
* Arguments :
* Returns     :
* Notes       :
*********************************************************************************/
BOOL oggGetTAGInfo(const TCHAR *filename, ID3Info_t *ID3Info, INT32 string_id)
{
    BOOL result; 

    ogg_TAGInfoFlag = FALSE;
        
    ogg_TagFileHandle = FOpenByte((INT8*)filename, FS_OPEN_NORMAL);  
	
    if(ogg_TagFileHandle == NULL )
    {
        return FALSE;
    }

    //----读TAG头标志
    result = oggSearchMem(ogg_TagInfor_keyword, 0x2000);    
    if(!result) 
    {
        FS_Close(ogg_TagFileHandle);
        return FALSE;
    }

    //读第一项的长度
    result = ReadLongData(&ogg_TagInforlen);
    if (!result)
    {
        FS_Close(ogg_TagFileHandle);
        return FALSE;                    
    }
    
    //这一项数据不要,跳过
    result = FSeekByte(FTellByte(ogg_TagFileHandle) + ogg_TagInforlen, ogg_TagFileHandle);
    if (!result)
    {
        FS_Close(ogg_TagFileHandle);
        return FALSE;                    
    }
    
    //读信息的总项数    
    result = ReadLongData(&ogg_TagInforCount);    
    if ( (!result) && (ogg_TagInforCount == 0) )
    {
        FS_Close(ogg_TagFileHandle);
        return FALSE;                    
    }


    //读取ID3信息
    GetAllItemInfor(ID3Info);
    
    FS_Close(ogg_TagFileHandle);
    return ogg_TAGInfoFlag;    
}


/*********************************************************************************
*                                 BOOL GetAllItemInfor(ID3Info_t *ID3Info)
* Description : 取所有信息  
* Arguments :
* Returns     :
* Notes       :
*********************************************************************************/
BOOL GetAllItemInfor(ID3Info_t *ID3Info)
{
    INT i;
    long readpt_sav;//读指针备份
    long searchcnt;//搜索的区域
    long tmp;
    INT32  j;

    readpt_sav = FTellByte (ogg_TagFileHandle);

    for (i=0; i<ogg_TagInforCount; i++)
    {
        if (!ReadLongData(&ogg_TagInforlen))
        {
            return FALSE;                    
        }
    
        if (!FSeekByte(FTellByte(ogg_TagFileHandle) + ogg_TagInforlen, ogg_TagFileHandle))
        {
            return FALSE;
        }
    }

    searchcnt = FTellByte (ogg_TagFileHandle) - readpt_sav;

    //找 title ------------------------------------------
    if (ID3Info->TIT2_length != 0)
    {
        FSeekByte(readpt_sav, ogg_TagFileHandle);
        if (oggSearchMem(ogg_Tag_TITLE_keyword, searchcnt))            
        {
            tmp = FTellByte (ogg_TagFileHandle);    
            FSeekByte(tmp-6-4, ogg_TagFileHandle);
            ReadLongData(&ogg_TagInforlen)    ;
            j  = (MIN(ogg_TagInforlen, ID3Info->TIT2_length) )- 1;
            FSeekByte(tmp, ogg_TagFileHandle);
            for (i=0; i<j; i++)
            {
                FReadByte(ID3Info->TIT2_buffer + i, ogg_TagFileHandle);
            }
            ID3Info->TIT2_buffer[i] = 0;
            ogg_TAGInfoFlag = TRUE;
        }        
    }    

    //找 ARTIST ------------------------------------------
    if (ID3Info->TPE1_length != 0)
    {
        FSeekByte(readpt_sav, ogg_TagFileHandle);
        if (oggSearchMem(ogg_Tag_ARTIST_keyword, searchcnt))
        {
            tmp = FTellByte (ogg_TagFileHandle);    
            FSeekByte(tmp-7-4, ogg_TagFileHandle);
            ReadLongData(&ogg_TagInforlen)    ;
            j  = (MIN(ogg_TagInforlen, ID3Info->TPE1_length)) - 1;
            FSeekByte(tmp, ogg_TagFileHandle);
            for (i=0; i<j; i++)
            {
                FReadByte(ID3Info->TPE1_buffer + i, ogg_TagFileHandle);
            }
            ID3Info->TPE1_buffer[i] = 0;
            ogg_TAGInfoFlag = TRUE;            
        }
    }    

    //找 ALBUM ------------------------------------------
    if (ID3Info->TALB_length != 0)
    {
		FSeekByte(readpt_sav, ogg_TagFileHandle);
		if (oggSearchMem(ogg_Tag_ALBUM_keyword, searchcnt))
		{
	        tmp = FTellByte (ogg_TagFileHandle);    
	        FSeekByte(tmp-6-4, ogg_TagFileHandle);
	        ReadLongData(&ogg_TagInforlen)    ;
	        j  = (MIN(ogg_TagInforlen, ID3Info->TALB_length) )- 1;
	        FSeekByte(tmp, ogg_TagFileHandle);
	        for (i=0; i<j; i++)
	        {
	            FReadByte(ID3Info->TALB_buffer + i, ogg_TagFileHandle);
	        }
	        ID3Info->TALB_buffer[i] = 0;
	        ogg_TAGInfoFlag = TRUE;                            
        }
    }

    return ogg_TAGInfoFlag;

}

/********************************************************************************
函数原型： BOOL oggSearchMem(const INT8 *memBuff, long scnt)
input:
    const INT8 *memBuff: 要查的BUFF的首址（第一个字节为BUFF的长度）
    long scnt:    源区域的长度    

output:    
    BOOL return: 找到字符串时为真             
    
********************************************************************************/
BOOL oggSearchMem(const INT8 *memBuff, UINT32 scnt)
{
    INT32 j=0;
    INT8 k;
    BOOL result; 
    
    while(scnt-- > 0 )
    {    
		result = FReadByte(&k, ogg_TagFileHandle);    
        if (!result)
        {
            return FALSE;
        }

		if (CheckCharType(k) == CharType_DNER)
		{
		    k = k - 0x20;//变为大写来处理         
		}
                
        if (k != memBuff[j+1])//因为第一个BUFF是数组的长度
        {
            j = 0; 
        }
        else
        {
            j++;
        }
        
        if (j >= memBuff[0])
        {
            //是否全部查找完
            return TRUE;      //--->> TRUE
        }                
    }
    return FALSE;

}









