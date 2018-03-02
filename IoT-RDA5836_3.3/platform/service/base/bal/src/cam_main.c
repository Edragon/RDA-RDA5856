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


#include "event.h"
#include "mci.h"
#if defined(CAMERA_SUPPORT)
#include "cam_main.h"
//Local variable
HANDLE g_camSem;

UINT32 cam_result;
VOID cam_set_result(UINT32 result)
{
    cam_result=result;
}

//External variable
#if defined(IP_CAMERA_SUPPORT)

BOOL ip_cam_main(MULTI_MSG_PTR ev)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch( ev->nEventId)
    {
        case EV_CAMERA_IPCAM_START_REQ:
            MCI_IPCamStart((CAM_CAPTURE_STRUCT*) ev->nParam1);
            cam_set_result(TRUE);
            CAM_SET_EVENT(IPCAM_EVT_START);
            break;
            
        case EV_CAMERA_IPCAM_STOP_REQ:
            MCI_IPCamStop();
            cam_set_result(TRUE);
            CAM_SET_EVENT(IPCAM_EVT_STOP);
            break;
            
        case EV_CAMERA_IPCAM_GETFRAME_REQ:

            cam_set_result(MCI_IPCamGetFrame(ev->nParam1,ev->nParam2));
            //break;
            
        case EV_CAMERA_IPCAM_GETFRAME_RSP:
            CAM_SET_EVENT(IPCAM_EVT_GET_FRAME_DATA);
            break;
        
		default:
			return FALSE;
    }

    return TRUE;

}

#endif /* IP_CAMERA_SUPPORT */
/******************************************************************************
 * cam_init: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL cam_init(void)
{
    g_camSem = COS_CreateSemaphore(0);

    return TRUE;
}

/******************************************************************************
 * cam_main: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
BOOL cam_main(MULTI_MSG_PTR ev)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT32 ret;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    MULTI_TRACE(MULTI_CAM_TRC, "cam_main EV.id=%d",ev->nEventId);


    switch( ev->nEventId)
    {
        case EV_CAMERA_POWER_UP_REQ:
            ret = MCI_CamPowerUp(0, NULL);
            cam_set_result(0 == ret);
            CAM_SET_EVENT(CAM_EVT_POWER_UP);
            break;

        case EV_CAMERA_POWER_DOWN_REQ:
            ret = MCI_CamPowerDown();
            cam_set_result(0 == ret);
            CAM_SET_EVENT(CAM_EVT_POWER_DOWN);
            break;

		default:
		    if(0)
		    {
		    }
#if defined(IP_CAMERA_SUPPORT)
		    else if(ip_cam_main(ev))
		    {
		    }
#endif /* IP_CAMERA_SUPPORT */
		    else
		    {
			    return FALSE;
			}
    }

    return TRUE;

}
#endif /* CAMERA_SUPPORT */
