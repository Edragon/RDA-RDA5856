/*****************************************************************************
    Name        : csrbc_usb.h
    Title       : Io Control Code definitions   
    Author      : 
    Created     : 
    Copyright   : © 2001-2006 Cambridge Silicon Radio Ltd.  All Rights Reserved.
    Description : Io Control Codes which applications use to communicate to the
                  device driver through DeviceIoControl calls. 

  CVS Identifier
    $Author: jianguo $

    $Log: csrbc_usb.h,v $
    Revision 1.1  2008/02/02 03:49:45  jianguo
    add file of csrbc_usb.h

    Revision 1.8  2001/11/30 17:31:22  pg02
    Changed define from RETRO to LEGACY as it seems more appropriate
    
    Revision 1.7  2001/11/29 16:35:00  pg02
    More code cleanup, added retro ioctl codes for compatibility with older
    software.
    
    Revision 1.6  2001/11/27 16:46:51  pg02
    Tidy up
    
    
*****************************************************************************/

#ifndef _CSRBC_USB_H_
#define _CSRBC_USB_H_

/*****************************************************************************
 DEFINES
*****************************************************************************/

#define CSRBC_IOCTL_INDEX  0x0000


#define IOCTL_CSRBC_SEND_HCI_COMMAND            CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

// for compatibility reasons with older software
#define IOCTL_CSRBC_SEND_HCI_COMMAND_LEGACY CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX, \
                                                    METHOD_OUT_DIRECT, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_HCI_EVENT               CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+1, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_VERSION             CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+2, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_DRIVER_NAME         CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+3, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_CONFIG_DESCRIPTOR     CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   CSRBC_IOCTL_INDEX+4,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)
                                                   
#define IOCTL_CSRBC_GET_DEVICE_DESCRIPTOR     CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   CSRBC_IOCTL_INDEX+5,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)
                                                   
#define IOCTL_CSRBC_RESET_DEVICE                CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   CSRBC_IOCTL_INDEX+6,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                              
                                                   
// for compatibility reasons with older software
#define IOCTL_CSRBC_SEND_HCI_DATA_LEGACY        CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+10, \
                                                    METHOD_OUT_DIRECT, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_SEND_HCI_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+10, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_HCI_DATA                CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+11, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_BLOCK_HCI_EVENT         CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+12, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_BLOCK_HCI_DATA          CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+13, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_SEND_CONTROL_TRANSFER       CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+14,\
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_START_SCO_DATA          CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+16, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_SEND_SCO_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+17, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_RECV_SCO_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+18, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_STOP_SCO_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+19, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS) 

#define IOCTL_CSRBC_SET_EVENT_LENGTH            CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+20, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS) 

#define IOCTL_CSRBC_HCI_HID_SWITCH_COMMAND  CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+21, \
                                                    METHOD_BUFFERED, \
                                                    FILE_ANY_ACCESS) 

#define IOCTL_CSRBC_RECV_SCO_DATA_MD                CTL_CODE(FILE_DEVICE_UNKNOWN, \
                                                    CSRBC_IOCTL_INDEX+22, \
                                                    METHOD_OUT_DIRECT, \
                                                    FILE_ANY_ACCESS)
#endif 

/*****************************************************************************
 End of file (csrbc_usb.h)
*****************************************************************************/

