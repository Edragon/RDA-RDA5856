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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   gif_decoder.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   header file for GIF decoder 
 *   
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef GIF_DECODER_H
#define GIF_DECODER_H

// base address
#define GIF_DECODER_BASE		GIF_base

// register address
#define GIF_GCT_BASE_ADDR			GIF_DECODER_BASE+0x4
#define GIF_LCT_BASE_ADDR			GIF_DECODER_BASE+0x8
#define GIF_STACK_BASE_ADDR		GIF_DECODER_BASE+0xc
#define GIF_GCE_REG					GIF_DECODER_BASE+0x10	// input by sw.
#define GIF_TREE_BASE_ADDR		   GIF_DECODER_BASE+0x14
#define GIF_DEC_CTL_ADDR			GIF_DECODER_BASE+0x18
#define GIF_STATUS_REG				GIF_DECODER_BASE+0x1c
#define GIF_START_REG				GIF_DECODER_BASE+0x20
#define GIF_INT_CTL_REG				GIF_DECODER_BASE+0x24
#define GIF_ORGB_BASE_ADDR		   GIF_DECODER_BASE+0x30
#define GIF_RESET_REG				GIF_DECODER_BASE+0x34
#define GIF_MIN_BUF_RANGE			GIF_DECODER_BASE+0x38	// not used
#define GIF_MAX_BUF_RANGE			GIF_DECODER_BASE+0x3c	// not used
#define GIF_INFILE_START			GIF_DECODER_BASE+0x40
#define GIF_INFILE_END				GIF_DECODER_BASE+0x44
#define GIF_IMG_REG_1				GIF_DECODER_BASE+0x48	// not used
#define GIF_IMG_REG_2				GIF_DECODER_BASE+0x4c	// not used
#define GIF_IMG_REG_3				GIF_DECODER_BASE+0x50	// not used
#define GIF_LSD_REG_1				GIF_DECODER_BASE+0x54
#define GIF_LSD_REG_2				GIF_DECODER_BASE+0x58

// bit positions
#define GIF_GCE_TC_FLG				0x1		// transparency control flag
#define GIF_GCE_TC_INDEX			0x1E		// transparency color index

#define GIF_DEC_CTL_RS				0x1		// restricted size(0:small,1:large)
#define GIF_DEC_CTL_OUT				0x2		// ouput path(1:resizer, 0:memory)

#define GIF_STATUS_FINISH_FILE	0x1
#define GIF_STATUS_FINISH_FRMAE	0x2		// decoding complete 
#define GIF_STATUS_TEXT				0x4		// Text extension block is present
#define GIF_STATUS_GCE				0x8		// Graphic control extension is present
#define GIF_STATUS_END				0x10		// Gif decoder reach the end of the file

#define GIF_IMG_REG3_LCT_SIZE				0x7	// local color table size
#define GIF_IMG_REG3_LCT_INTERLACE		0x40	// is interlaced
#define GIF_IMG_REG3_LCT_FLG				0x80	// is LCT present

#define GIF_LSD2_PXL_RATIO			0xFF			// pixel aspect ratio
#define GIF_LSD2_BG					0xFF00		// background color
#define GIF_LSD2_GCT_SIZE			0x70000		// GCT size
#define GIF_LSD2_GCT_SORT			0x80000		// is GCT sorted
#define GIF_LSD2_BPP					0x700000		// bits per pixel
#define GIF_LSD2_GCT_FLG			0x800000		// is GCT present

#define GIF_GCE_DISPOSAL			0x1c

// constants 
#define GIF_RESET_KEY				0x1201
#define GIF_STACK_SIZE				(0x1000+4)
#define GIF_TREE_SIZE				(0x4000+4)
#define GIF_GCT_SIZE					(0x200)
#define GIF_LCT_SIZE					(0x200)
#define GIF_GUARD_PATTERN			0x12345678
//#define GIF_TIMEOUT_PERIOD			200	// x 10ms
#define GIF_SOFTKEY					0xFFFF

// return values
#define GIF_FINISH_FRAME		0
#define GIF_LAST_FRAME			1
#define GIF_OUT_OF_RANGE		2
#define GIF_NO_FRAME				3

#define GIF_DECODE_TIMEOUT		-1
#define GIF_TRUNCATED_FILE		-2
#define GIF_FORMAT_ERROR		-3
#define GIF_MEM_INSUFFICEITN  -4
#define GIF_HW_DECODE_FAIL		-5

// macro
#define GIF_START()		DRV_WriteReg(GIF_START_REG, 1);
#define GIF_RESET()		DRV_WriteReg32(GIF_RESET_REG, GIF_RESET_KEY)

typedef enum{
	GIF_MEMORY = 0,
	GIF_RESIZER = 1
}gif_out_location_enum;

typedef enum{
	GIF_STATE_READY,	
	GIF_STATE_BUSY			
}gif_state_enum;

typedef enum{
	GIF_NO_ACTION,
	GIF_NOT_DISPOSAL,
	GIF_RESTORE_BG,
	GIF_RESTORE_PRE,

	GIF_CSD_NO_ACTION   // don't force the disposal method
}gif_disposal_enum;

// structures
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
typedef struct {
	UINT32 stack_size;
	UINT32 tree_size;
	UINT32 GCT_size;
	UINT32 LCT_size;
	
	UINT8 *stack;
	UINT8 *tree;
	UINT8 *GCT;
	UINT8 *LCT;
}gif_resource_struct;
#endif
typedef struct {

	// image info
	UINT16 	width;		// image width
	UINT16 	height;		// image height
	UINT16	x;				// image top 
	UINT16	y;				// image left
	UINT16 	lwidth;		// logical screen width
	UINT16 	lheight;		// logical screen height

	UINT8	LCT_flag;	// is LCT(Local Color Table) present
	UINT8 	LCT_size;	// LCT size 
	UINT8	interlace;	// is interlaced
	UINT8	bg;			// background color(index)
	UINT8	bpp;			// bits per pixel
	UINT8	GCT_flag;	// is GCT(Global Color Table present
	UINT8	GCT_size;	// GCT size		

	UINT8 transparent_index;
	UINT8 transparent_flag;

	UINT8  	delay_time;
	UINT16 	bg_color;		// backgroud color in RGB565
}gif_info_struct;

typedef struct{
	UINT8 *img_adrs;
	UINT32 img_size;
	UINT16 frame_number; // specify the frmae nubmer of gif file(input), 0 based
	gif_disposal_enum disposal_force; 	// force the disposal method instead of specified in gif file.	
	BOOL reset; // force the gif decoder to decode from the 1st frame 

	UINT32 shadow_adrs;
	UINT16 shadow_width,shadow_height;
	INT16 clip_x1, clip_y1, clip_x2, clip_y2;
	INT16 dest_x, dest_y;
	UINT32 work_adrs,work_size;	
	BOOL  decode_test;
}gif_para_struct;

typedef struct{
	gif_out_location_enum out_location;	// specify output to memory or resizer
	UINT8 gpt_handle;					// handler of gpt timer 
	UINT8 timeout_period;				// time out period of gpt timer
	volatile BOOL is_timeout;						// flag to indicate whther the gpt is expired	
	gif_state_enum state;	
	BOOL trailer;							// indicate is tralier is reached	
	UINT16 frame_counter;				// specify the frame number of gif file (out)
	gif_info_struct gif_info;	
	UINT8 *img_adrs,*img_end,*current_adrs;				// specify the current frame address 	
}gif_dcb_struct;

extern void GIF_Reset(void);
extern INT32 GIF_DecodeFirst(UINT8* src, void* dest, UINT32 size);
extern INT32  GIF_DecodeNext(void* dest);
extern void GIF_DecodeEnd(void);
extern INT32 GIF_Display_HW(gif_para_struct *s, gif_info_struct *info);

#endif // GIF_DECODER_H


