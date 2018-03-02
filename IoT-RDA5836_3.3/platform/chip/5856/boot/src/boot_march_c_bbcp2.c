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
#include "cs_types.h"

#include "global_macros.h"
#include "bb_cp2.h"

#define MEM_READ_UNCACHE(addr)          ( *(volatile unsigned long *)((addr) | (0xa0000000)) )
#define MEM_WRITE_UNCACHE(addr, data)   ( *(volatile unsigned long *)((addr) | (0xa0000000)) ) = data

#define ZEROS       0xaaaaaaaa
#define ONES        0x55555555


#if 0 //debug

#define Stop(a,b,c) {\
        ct_setW0(a);\
        ct_setW0(b);\
        ct_setW1(c);\
        return 1;\
    }

#else
#define Stop(a,b,c) return 1
#endif



/* Must compiled in Mips32 */
int boot_MarchCBBCp2(void)
{

// March-C test 4 loops:
//    0: <-> W0
//    1:  -> R0; W1
//    2:  -> R1; W0
//    3: <-> R0
//

    unsigned int i ;
    unsigned int ReadVal ;
    unsigned int WriteVal[1];

    WriteVal[0] = ZEROS ;
    WriteVal[1] = ONES ;
//----------------------------------
//            BEGIN LOOP 0

    // Initialize Puncturing RAM with zero's.  First loop
    // First, give the start address for puncturing LRAM (always 0)
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, 0);
    // Write into puncturing LRAM
    for (i=0; i<22; i++)
        CT_BB_CP2_WR_GNRL_REG_MEM(BB_CP2_LRAM_DATA, WriteVal);

    // Initialize Data RAM with zero's.  First loop
    // First, give the start address for data LRAM 
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, (1<<5));
    // Write into data LRAM
    for (i=0; i<16; i++)
        CT_BB_CP2_WR_GNRL_REG_MEM(BB_CP2_LRAM_DATA, WriteVal);

//          END LOOP 0
//----------------------------------
//----------------------------------
//            BEGIN LOOP 1


    // Read Puncturing LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, 0);
    for (i=0; i<22; i++)
    {
        CT_BB_CP2_RD_GNRL_REG_GPR(BB_CP2_LRAM_DATA, ReadVal);
        if (ReadVal != ZEROS )
            Stop(WriteVal[0], ReadVal, ZEROS);
    }
    // Write into puncturing LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, 0);
    for (i=0; i<22; i++)
        CT_BB_CP2_WR_GNRL_REG_MEM(BB_CP2_LRAM_DATA, WriteVal+1);

    // Read Data LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, (1<<5));
    for (i=0; i<16; i++)
    {
        CT_BB_CP2_RD_GNRL_REG_GPR(BB_CP2_LRAM_DATA, ReadVal);
        if (ReadVal != ZEROS )
            Stop(WriteVal[0], ReadVal, ZEROS);
    }
    // Write into data LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, (1<<5));
    for (i=0; i<16; i++)
        CT_BB_CP2_WR_GNRL_REG_MEM(BB_CP2_LRAM_DATA, WriteVal+1);

//          END LOOP 1
//----------------------------------
//----------------------------------
//          BEGIN LOOP 2


    // Read Puncturing LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, 0);
    for (i=0; i<22; i++)
    {
        CT_BB_CP2_RD_GNRL_REG_GPR(BB_CP2_LRAM_DATA, ReadVal);
        if (ReadVal != ONES )
            Stop(WriteVal[1], ReadVal, ONES);
    }
    // Read Data LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, (1<<5));
    for (i=0; i<16; i++)
    {
        CT_BB_CP2_RD_GNRL_REG_GPR(BB_CP2_LRAM_DATA, ReadVal);
        if (ReadVal != ONES )
            Stop(WriteVal[1], ReadVal, ONES);
    }
    // Write into data LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, (1<<5));
    for (i=0; i<16; i++)
        CT_BB_CP2_WR_GNRL_REG_MEM(BB_CP2_LRAM_DATA, WriteVal);
    // Write into puncturing LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, 0);
    for (i=0; i<22; i++)
        CT_BB_CP2_WR_GNRL_REG_MEM(BB_CP2_LRAM_DATA, WriteVal);

//      END LOOP 2
//----------------------------------
//----------------------------------
//          BEGIN LOOP 3


    // Read Puncturing LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, 0);
    for (i=0; i<22; i++)
    {
        CT_BB_CP2_RD_GNRL_REG_GPR(BB_CP2_LRAM_DATA, ReadVal);
        if (ReadVal != ZEROS )
            Stop(WriteVal[0], ReadVal, ZEROS);
    }
    // Read Data LRAM
    CT_BB_CP2_WR_CTRL_REG(BB_CP2_LRAM_ADDR, (1<<5));
    for (i=0; i<16; i++)
    {
        CT_BB_CP2_RD_GNRL_REG_GPR(BB_CP2_LRAM_DATA, ReadVal);
        if (ReadVal != ZEROS )
            Stop(WriteVal[0], ReadVal, ZEROS);
    }

//      END LOOP 3
//----------------------------------

    return 0; // ok
}

