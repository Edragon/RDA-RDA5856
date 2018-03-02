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


#ifndef _REG_LOC_H_
#define _REG_LOC_H_

#define REG_ROOTKEY_COUNT      4              // root key count.  

#define REG_TYPE_STR_BINARY "hex:"         // string of hex type.
#define REG_TYPE_STR_INT32  "int32:"        // string of int32 type.
#define REG_TYPE_STR_INT8   "int8:"           // string of int8 type.

// defaut string line type 
#define REG_DEF_STR_LINE_TYPE_KEY            1       // Root sub key
#define REG_DEF_STR_LINE_TYPE_VALUE        2       // Value name
#define REG_DATA_TYPE_INT8_NUM_LEN        3       // int8 number length.
#define REG_DATA_TYPE_INT32_NUM_LEN      8       // int32 number length.


// The header information struct.
#define REG_MAGIC              0x20060915   // the magic number.              
#define REG_VERSION          0x20101118   // the reg module version. reference the csw version.
#define REG_CHECKSUM       0x33061648   // check sum constant


 // the index is used. 
#define REG_INDEX_INUSE                1    

 // the index is free.
#define REG_INDEX_FREE                 0

// Root key size.
// #define REG_ROOTKEY_SIZE             1

// Root key size.
// #define REG_SKSIZE_SIZE                1

// iSKSize size
// #define REG_VNSIZE_SIZE                1

// iDType size
// #define REG_DTYPE_SIZE                 1

// iDSize size
// #define REG_DSIZE_SIZE                  1

// szSK size
// #define REG_SK_SIZE                        REG_SUBKEY_LEN_MAX

// szVN size
// #define REG_VN_SIZE                        REG_VALUENAME_LEN_MAX

// iCheckSum size
// #define REG_CHECKSUM_SIZE           4

// reserved lenth
#define REG_RESERVED_SIZE            3

// max count opened sub key.
#define REG_OPENKEY_MAX              20

// the default string value type max length. 
#define REG_DEF_STR_TYPE_LEN       8

// the default string root key name length.
#define REG_DEF_STR_ROOT_KEY_LEN   30


#define REG_MAGIC_SIZE                  4      // migic  size    
//#define REG_VERSION_SIZE                4      // version size
//#define REG_H_OFFSET_SIZE               4      // header_offset size
//#define REG_H_SIZE_SIZE                   4      // header_size size
//#define REG_SIZE_LEN                        4      // regsize size. 





// The state for parse the reg default cfg string .
enum REG_PARSE_STATE
    {
        REG_PARSE_STATE_INIT,
        REG_PARSE_STATE_ROOTKEY,
        REG_PARSE_STATE_SUB_KEY,
        REG_PARSE_STATE_VALUENAME,
        REG_PARSE_STATE_EQUAL_TO,
        REG_PARSE_STATE_VALUE_TYPE_BEGIN,
        REG_PARSE_STATE_VALUE_TYPE,        
        REG_PARSE_STATE_VALUE_STR,
        REG_PARSE_STATE_VALUE_HEX,
        REG_PARSE_STATE_VALUE_INT8,
        REG_PARSE_STATE_VALUE_INT32,
        REG_PARSE_STATE_FINISH,
        REG_PARSE_STATE_END,     
    };

typedef struct{
  UINT32  iMagic ;                       // Magic number.	                   
  UINT32  iVer;                           // version number.                    
  UINT32  iHOffset;                     // header offset.                   
  UINT32  iHSize;                        // header size.                               
  UINT32  iRecSize;                    // record size.                  
  UINT32  iRecCount;                  // record count.
  UINT32  iRegSize;                    // reg size.
  UINT16  iOffset_RHSR;             // REG_HKEY_SYSTEM_ROOT offset .                   
  UINT16  iOffset_RHLM;             // REG_HKEY_LOCAL_MACHINE offset.                      
  UINT16  iOffset_RHCC;             // REG_HKEY_CURRENT_COMM offset.   	                   
  UINT16  iOffset_RHCU;             // REG_HKEY_CURRENT_USERS offset.                      	
  UINT8   szServed[28];              // served field.                    
}REG_H_INFO;

#define REG_H_SIZE SIZEOF(REG_H_INFO)

typedef struct
{
  UINT8 iRKey;                                              // root key.
  UINT8 iSKSize;                                           // sub key size.
  UINT8 iVNSize;                                           // value name size. 
  UINT8 iDType;                                            // data type. 
  UINT8 iDSize;                                             // data size. 
  UINT8 szSK[REG_SUBKEY_LEN_MAX ];          // sub key name
  UINT8 szVN[REG_VALUENAME_LEN_MAX ];    // value name    
  UINT32 iCheckSum;
}REG_R_CTR;


// The record information struct.
typedef struct
{
  
  UINT8 iRKey;                                              // root key.
  UINT8 iSKSize;                                            // sub key size.
  UINT8 iVNSize;                                            // value name size. 
  UINT8 iDType;                                             // data type. 
  UINT8 iDSize;                                              // data size. 
  UINT8 szSK[REG_SUBKEY_LEN_MAX ];           // sub key name
  UINT8 szVN[REG_VALUENAME_LEN_MAX];      // value name
  UINT32 iCheckSum;                                     // checksum
  union 
  {
    UINT8   iInt8;
    UINT32 iInt32;
    UINT8  szStr[REG_VALUE_LEN_MAX];
  }u_data;
  UINT8 szServed[REG_RESERVED_SIZE];
}REG_R_INFO;

// Record control info length.
#define REG_R_CTR_LEN  SIZEOF(REG_R_CTR)   // (REG_ROOTKEY_SIZE+ REG_SKSIZE_SIZE+ REG_VNSIZE_SIZE+ REG_DTYPE_SIZE+ REG_DSIZE_SIZE+ REG_SK_SIZE+ REG_VN_SIZE+ REG_CHECKSUM_SIZE)

// Record length.
#define REG_R_LEN          SIZEOF(REG_R_INFO)  // (REG_R_CTR_LEN + REG_VALUE_LEN_MAX + REG_RESERVED_SIZE)

typedef struct 
{
  HANDLE hKey;
  UINT16 iOffset;
}REG_RK_OFFSET;

// Root sub key dscriptor.
typedef struct
{
    UINT32 iFlag;
    HANDLE   iRKey; 
    UINT8 szSK[REG_SUBKEY_LEN_MAX];   
}REG_RSKD;                                  

// Key line information
typedef struct _reg_def_key_line
{
    UINT8 iRKey; 
    UINT8 iSKSize;
    UINT8 szSK[REG_SUBKEY_LEN_MAX];
    UINT8 Reserved[2];
}REG_DEF_KEY_LINE;

// Value name line information
typedef struct _reg_def_value_line
{
    UINT8 iDType;                      // data type. 
    UINT8 iVNSize;                     // value name size.     
    UINT8 iDSize;                      // data size. 
    UINT8 szVN[REG_VALUENAME_LEN_MAX]; // value name 
    union                              
    {
        UINT8  iInt8;
        UINT32 iInt32;
        UINT8  szStr[REG_VALUE_LEN_MAX];
    }u_data;                          // value.
}REG_DEF_VALUE_LINE;


// the result of parse the default line.
typedef struct _reg_def_line_info
{
    UINT32 iType;            //When this field value is  REG_DEF_STR_LINE_TYPE_KEY,  the uData is sKeyLine;
                             //else if the value is REG_LINE_TYPE_VALUE, the uData is sValueLine.
    union{
        REG_DEF_KEY_LINE sKeyLine;
        REG_DEF_VALUE_LINE  sValueLine;
    }uLine;    
}REG_DEF_LINE_INFO;


struct REG_Global_Var
{
    HANDLE hSem;                                     // Reg module semaphore handle
    UINT32  iMediaSize;                             // Media Size,the partition total size.
    UINT32 iHasInited;                              //  Reg module initialized flag: 1 has initialized;0 not initialize.
};


INT32 REG_Read(UINT32 ulAddrOffset,UINT8* pBuffer, UINT32 ulBytesToRead, UINT32* pBytesRead);    
INT32 REG_Write(UINT32 ulAddrOffset,CONST UINT8* pBuffer,UINT32 ulBytesToWrite,UINT32*  pBytesWritten);

INT32 reg_FindValueName(HANDLE hKey,PCSTR pcSubKey,REG_R_CTR* pRCtr, PCSTR pValueName,INT32 iRecOffset);
INT32 reg_GetFreeRSK();
INT32 reg_SetRSK(HANDLE hKey,PCSTR pcSubKey,INT32 iIndex,HANDLE* phKey);
INT32 reg_FreeRSK();

#if 0
INT32 reg_Buff2CTR(UINT8* pStr,REG_R_CTR* pCtr);        
INT32 reg_CTR2Buff(REG_R_CTR* pCtr,UINT8* pStr);        
INT32 reg_Buff2RInfo(UINT8* pStr,REG_R_INFO* pRInfo);   
INT32 reg_RInfo2Buff(REG_R_INFO* pRInfo,UINT8* pStr);   
#endif

INT32 reg_CreateReg(VOID);
VOID  reg_SortRK(REG_RK_OFFSET pRKOffset[]);
INT32 reg_Init_Local(UINT8 iMode);
INT32 reg_GetHeaderInfo(VOID);


INT32 reg_BuffToInt();
INT32 reg_IntToBuff();

INT32 reg_MediaInit();
BOOL  reg_Existed();
INT32 reg_GetLineInfo(const UINT8* pPos,REG_DEF_LINE_INFO* pLineInfo,UINT32* piSize);

UINT32 reg_GetRegSize(VOID);

#ifdef _FS_SIMULATOR_
#define reg_SemInit()  
#define reg_Down()    
#define reg_Up()       
#include<vds_api.h>
#else
#define reg_SemInit() do { g_RegGlobalVar.hSem = COS_CreateSemaphore(1); } while(0)
#define reg_Down()    do { COS_WaitForSemaphore(g_RegGlobalVar.hSem, COS_WAIT_FOREVER); } while(0)
#define reg_Up()      do { COS_ReleaseSemaphore(g_RegGlobalVar.hSem); } while(0)
#endif


//local errorcode
#define _ERR_REG_SUCCESS                                        0      // Success. 

#define _ERR_REG_UNINIT                                          -1001   // the reg module uninit.
#define _ERR_REG_NOENTRY                                       -1002   // no root key.
#define _ERR_REG_RMEDIA_FAILED                            -1003   // write media failed. 
#define _ERR_REG_OVER_POSITION                            -1004   // parameter position is wrong. 
#define _ERR_REG_INVALID_PARAMETER                    -1005   // invalid parameter.


#define _ERR_REG_READMEDIA_FAILED                      -1006   // read the media failed.
#define _ERR_REG_WRITEMEDIA_FAILED                    -1007   // write the media failed.
#define _ERR_REG_NO_MORE_MEMORY                        -1008   // no more memory.
// for parse reg default cfg.
#define _ERR_REG_CFG_ROOT_KEY_UNKNOW              -1030   // unkown root key.
#define _ERR_REG_CFG_VALUE_TYPE_UNKNOW          -1031   // unkown value type.
#define _ERR_REG_CFG_ORDER_WRONG                      -1032   // the word order wrong.
#define _ERR_REG_CFG_STR2HEX_WRONG                  -1033   // str to hex wrong.
#define _ERR_REG_CFG_ROOT_KEY_TOO_LONG          -1034   // root key too long.  
#define _ERR_REG_CFG_SUB_KEY_TOO_LONG            -1035   // sub key too long.
#define _ERR_REG_CFG_VALUE_NAME_TOO_LONG     -1036   // value name too long.
#define _ERR_REG_CFG_VALUE_TYPE_TOO_LONG      -1037   // value type too long.
#define _ERR_REG_CFG_VALUE_TOO_LONG                -1038   // value too long. case int8: - 2; case int32:4 case string and hex: 64.  
#define _ERR_REG_DEV_NOT_EXIST                           -1039   // the device not exist for reg moucle.

#endif

