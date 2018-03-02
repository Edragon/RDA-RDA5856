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

/*******************************************************************************
 * Filename:
 * ---------
 *   img_api.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary exported functions of image module.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES =========*/
#ifdef __MED_IMG_MOD__


/* system includes */
#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"


/* local includes */
#include "med_global.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "img_main.h"

#include "media_others.h"

static kal_int32 img_result;
static kal_uint32 img_result_arg0;
static kal_uint32 img_result_arg1;
static kal_uint16 decoded_image_width;

/*==== FUNCTIONS ===========*/


void img_set_result(kal_int32 result)
{
   img_result=result;
}
void img_set_result_ext(kal_int32 result,kal_uint32 arg0,kal_uint32 arg1)
{
   img_result=result;	
   img_result_arg0 = arg0;
   img_result_arg1 = arg1;
}

void img_set_decoded_width(kal_uint16 width)
{
   decoded_image_width=width;
}

kal_int32 media_img_decode(module_type src_mod_id, void* decode)
{
   img_result=MED_RES_OK;
   img_send_decode_req(src_mod_id, decode);
   IMG_WAIT_EVENT(IMG_EVT_DECODE);
   ((img_decode_struct*)decode)->decoded_image_width=decoded_image_width;
   //img_send_decode_finish_ind(MED_RES_OK);
   return img_result;
}

kal_int32 media_img_exif_decode(module_type src_mod_id, void* exif_decode)
{
   img_result=MED_RES_OK;
   img_send_exif_decode_req(src_mod_id, exif_decode);
   IMG_WAIT_EVENT(IMG_EVT_EXIF_DECODE);
   return img_result;

}

kal_int32 media_img_encode(module_type src_mod_id, img_encode_struct* encode)
{
   img_result=MED_RES_OK;
   img_send_encode_req(src_mod_id, encode);
   IMG_WAIT_EVENT(IMG_EVT_ENCODE);
   return img_result;
}

void media_img_stop(module_type src_mod_id)
{
   img_send_stop_req(src_mod_id);
   IMG_WAIT_EVENT(IMG_EVT_STOP);
}

kal_int32 media_img_resize(module_type src_mod_id, img_resize_struct *resize)
{
   img_result=MED_RES_OK;
   img_send_resize_req(src_mod_id, resize);
   IMG_WAIT_EVENT(IMG_EVT_RESIZE);
   return img_result;
}

#endif /* __MED_IMG_MOD__ */

#endif /* MED_NOT_PRESENT */



