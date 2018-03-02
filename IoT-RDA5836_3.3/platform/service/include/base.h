/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    base.h                                                        */
/*                                                                            */
/* Description:                                                               */
/*   base  service declarations, constant definitions and macros              */
/*                                                                            */
/******************************************************************************/
//
#if !defined(__BASE_H__)
#define __BASE_H__

//
// the necessary header and pragma defines.
//
#include <cswtype.h>
#include <errorcode.h>
#include <event.h>

#include "../base/include/reg.h"
#include <rfm.h>
#include <rcm.h>

BOOL SRVAPI BS_Init(VOID);
BOOL SRVAPI BS_Exit(VOID);

#endif // _H_
