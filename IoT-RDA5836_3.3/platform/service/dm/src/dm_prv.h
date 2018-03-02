/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    dm.h                                                          */
/*                                                                            */
/* Description:                                                               */
/*    DM service declarations, constant definitions and macros                */
/*                                                                            */
/******************************************************************************/

#ifndef __DM_PRV_H__
#define __DM_PRV_H__

BOOL dm_KeyInit();
//void dm_KeyHandler(UINT16 status);
BOOL dm_Init();

BOOL dm_PowerdownInit(VOID);
UINT8 dm_GetStdKey(UINT8 Button);
BOOL dm_PowerOnCheck(VOID);

#endif // _H_

