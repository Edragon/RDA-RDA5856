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


#include "edrvp_debug.h"
#include "pn544.h"
#include "nfcd_m.h"
#include "hal_i2c.h"
#include "hal_sys.h"
#include "hal_gpio.h"

#define PN544_BUS_ID 	HAL_I2C_BUS_ID_2
#define PN544_I2C_BPS 	HAL_I2C_BPS_100K
#define I2C_MASTER_ACK              (1<<0)
#define I2C_MASTER_RD               (1<<4)
#define I2C_MASTER_STO              (1<<8)
#define I2C_MASTER_WR               (1<<12)
#define I2C_MASTER_STA              (1<<16)

Reset_packet reset_packet = {0};
Hcp_packet hcp_packet = {0};
Hcp_data1_packet hcp_data1_packet = {0};
Hcp_data2_packet hcp_data2_packet = {0};
Hcp_data3_packet hcp_data3_packet = {0};
Hcp_data4_packet hcp_data4_packet = {0};

volatile char pn544_buf[10] = {0};
UINT8 frame_num = 0;
UINT8 recv_frame_num = 1;
const unsigned short crc16_ccitt_table[256] =
{
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};
/*****CRC计算函数,可将第一个参数reg_init简化掉********/
unsigned short do_crc(unsigned short reg_init, unsigned char *message, unsigned int len)
{
    unsigned short crc_reg = reg_init;
         
    while (len--)
        crc_reg = (crc_reg >> 8) ^ crc16_ccitt_table[(crc_reg ^ *message++) & 0xff];
       
    return crc_reg;
} 


static UINT8 create_pn544_packet(UINT8 ins, UINT8 pid,
	UINT8 param_num, UINT8 param1, UINT8 param2,
	UINT8 param3, UINT8 param4)
{
	UINT16 crc;
	switch(param_num)
	{
		case 0:
			hcp_packet.llc_len = 5;
			hcp_packet.llc_header = ((0x01 << 7) | (frame_num << 3) | recv_frame_num);
			hcp_packet.cb = 1;
			hcp_packet.pid = pid;
			hcp_packet.type = HCP_COMMAND_TYPE;
			hcp_packet.ins = ins;
			crc = do_crc(0xffff, (UINT8 *)&hcp_packet, 4);
			crc^=0xffff;
			hcp_packet.crc[0] = crc&0x00ff;
			hcp_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
			break;
		case 1:
			hcp_data1_packet.llc_len = 6;
			hcp_data1_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_data1_packet.cb = 1;
			hcp_data1_packet.pid = pid;
			hcp_data1_packet.type = HCP_COMMAND_TYPE;
			hcp_data1_packet.ins = ins;
			hcp_data1_packet.data = param1;
			crc = do_crc(0xffff, (UINT8 *)&hcp_data1_packet, 5);
			crc^=0xffff;
			hcp_data1_packet.crc[0] = crc&0x00ff;
			hcp_data1_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
			break;
		case 2:
			hcp_data2_packet.llc_len = 7;
			hcp_data2_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_data2_packet.cb = 1;
			hcp_data2_packet.pid = pid;
			hcp_data2_packet.type = HCP_COMMAND_TYPE;
			hcp_data2_packet.ins = ins;
			hcp_data2_packet.data[0] = param1;//0x05;
			hcp_data2_packet.data[1] = param2;//0x00;
			crc = do_crc(0xffff, (UINT8 *)&hcp_data2_packet, 6);
			crc^=0xffff;
			hcp_data2_packet.crc[0] = crc&0x00ff;
			hcp_data2_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
			break;
		case 3:
			hcp_data3_packet.llc_len = 8;
			hcp_data3_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_data3_packet.cb = 1;
			hcp_data3_packet.pid = pid;
			hcp_data3_packet.type = HCP_COMMAND_TYPE;
			hcp_data3_packet.ins = ins;
			hcp_data3_packet.data[0] = param1;//0x05;
			hcp_data3_packet.data[1] = param2;//0x00;
			hcp_data3_packet.data[2] = param3;//0x05;
			crc = do_crc(0xffff, (UINT8 *)&hcp_data3_packet, 7);
			crc^=0xffff;
			hcp_data3_packet.crc[0] = crc&0x00ff;
			hcp_data3_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
			break;
		case 4:
			hcp_data4_packet.llc_len = 9;
			hcp_data4_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_data4_packet.cb = 1;
			hcp_data4_packet.pid = pid;
			hcp_data4_packet.type = HCP_COMMAND_TYPE;
			hcp_data4_packet.ins = ADM_CREATE_PIPE;
			hcp_data4_packet.data[0] = param1;//0x05;
			hcp_data4_packet.data[1] = param2;//0x00;
			hcp_data4_packet.data[2] = param3;//0x05;
			hcp_data4_packet.data[3] = param4;
			crc = do_crc(0xffff, (UINT8 *)&hcp_data4_packet, 8);
			crc^=0xffff;
			hcp_data4_packet.crc[0] = crc&0x00ff;
			hcp_data4_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
			break;
		default:
			break;
	}
}

static UINT8 create_hcp_packet(UINT8 ins, UINT8 pid, 
		UINT8 param1, UINT8 param2, 
		UINT8 param3, UINT8 param4)
{
	UINT16 crc;
	switch(ins)
	{
		case RESET_FRAME:
			frame_num = 0;
			recv_frame_num = 1;
			reset_packet.llc_len = 5;
			reset_packet.llc_header = 0xf9;
			reset_packet.window_len = 0x04;
			reset_packet.endpoint = 0x00;
			crc = do_crc(0xffff, (UINT8 *)&reset_packet, 4);
			crc^=0xffff;
			reset_packet.crc[0] = crc&0x00ff;
			reset_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
		break;
		case ANY_SET_PARAMETER:
		break;
		case ANY_GET_PARAMETER:
			hcp_data1_packet.llc_len = 6;
			//hcp_data1_packet.llc_header = 0x80 & (frame_num << 3) & recv_frame_num;
			hcp_data1_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_data1_packet.cb = 1;
			hcp_data1_packet.pid = pid;
			hcp_data1_packet.type = HCP_COMMAND_TYPE;
			hcp_data1_packet.ins = ANY_GET_PARAMETER;
			hcp_data1_packet.data = param1;
			crc = do_crc(0xffff, (UINT8 *)&hcp_data1_packet, 5);
			crc^=0xffff;
			hcp_data1_packet.crc[0] = crc&0x00ff;
			hcp_data1_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
		break;
		case ANY_CLEAR_ALL_PIPE:
			hcp_packet.llc_len = 5;
			hcp_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_packet.cb = 1;
			hcp_packet.pid = pid;
			hcp_packet.type = HCP_COMMAND_TYPE;
			hcp_packet.ins = ANY_CLEAR_ALL_PIPE;
			crc = do_crc(0xffff, (UINT8 *)&hcp_packet, 4);
			crc^=0xffff;
			hcp_packet.crc[0] = crc&0x00ff;
			hcp_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
		break;
		case ANY_OPEN_PIPE:	
			hcp_packet.llc_len = 5;
			hcp_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_packet.cb = 1;
			hcp_packet.pid = pid;
			hcp_packet.type = HCP_COMMAND_TYPE;
			hcp_packet.ins = ANY_OPEN_PIPE;
			crc = do_crc(0xffff, (UINT8 *)&hcp_packet, 4);
			crc^=0xffff;
			hcp_packet.crc[0] = crc&0x00ff;
			hcp_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
		break;	
		case ANY_CLOSE_PIPE:
			hcp_packet.llc_len = 5;
			hcp_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_packet.cb = 1;
			hcp_packet.pid = pid;
			hcp_packet.type = HCP_COMMAND_TYPE;
			hcp_packet.ins = ANY_CLOSE_PIPE;
			crc = do_crc(0xffff, (UINT8 *)&hcp_packet, 4);
			crc^=0xffff;
			hcp_packet.crc[0] = crc&0x00ff;
			hcp_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
		break;
		case ANY_DELETE_PIPE:
			hcp_data1_packet.llc_len = 6;
			hcp_data1_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
			hcp_data1_packet.cb = 1;
			hcp_data1_packet.pid = pid;
			hcp_data1_packet.type = HCP_COMMAND_TYPE;
			hcp_data1_packet.ins = ANY_DELETE_PIPE;
			hcp_data1_packet.data = param1;
			crc = do_crc(0xffff, (UINT8 *)&hcp_data1_packet, 5);
			crc^=0xffff;
			hcp_data1_packet.crc[0] = crc&0x00ff;
			hcp_data1_packet.crc[1] = crc>>8&0x00ff;
			frame_num = (frame_num++) & 0x07;
			recv_frame_num = (recv_frame_num++) & 0x07;
		break;
		case ADM_CREATE_PIPE:
		hcp_data3_packet.llc_len = 8;
		hcp_data3_packet.llc_header = ((0x01 << 7) | ((frame_num&0x07) << 3) | (recv_frame_num&0x07));
		hcp_data3_packet.cb = 1;
		hcp_data3_packet.pid = pid;
		hcp_data3_packet.type = HCP_COMMAND_TYPE;
		hcp_data3_packet.ins = ADM_CREATE_PIPE;
		hcp_data3_packet.data[0] = param1;//0x05;
		hcp_data3_packet.data[1] = param2;//0x00;
		hcp_data3_packet.data[2] = param3;//0x05;
		crc = do_crc(0xffff, (UINT8 *)&hcp_data3_packet, 7);
		crc^=0xffff;
		hcp_data3_packet.crc[0] = crc&0x00ff;
		hcp_data3_packet.crc[1] = crc>>8&0x00ff;
		frame_num = (frame_num++) & 0x07;
		recv_frame_num = (recv_frame_num++) & 0x07;
		break;
	}
}

static void pn544_I2cOpen(void) 
{
#ifdef I2C_BASED_ON_GPIO
        gpio_i2c_open();
#else
	  hal_I2cOpen(PN544_BUS_ID);
#endif
}

static void pn544_I2cClose(void)
{
#ifdef I2C_BASED_ON_GPIO
       gpio_i2c_close();
#else
	 hal_I2cClose(PN544_BUS_ID); 
#endif
}

static void pn544_iic_write_data(unsigned char *data, unsigned char datalen)
{
	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
	UINT8 i=0;
	
	hal_I2cSendRawByte(PN544_BUS_ID,(PN544_CHIP_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	for(i=0;i<datalen-1;i++,data++)//data
	{
		halErr=hal_I2cSendRawByte(PN544_BUS_ID,*data,I2C_MASTER_WR);
	}
	halErr=hal_I2cSendRawByte(PN544_BUS_ID,*data,I2C_MASTER_WR|I2C_MASTER_STO);

//	SXS_TRACE(TSTDOUT, "rdabt_iic_rf_write_data: %d\n.",halErr);
}

static void pn544_iic_read_data(unsigned char *data, unsigned char datalen)
{
	UINT8 i=0;
	
	hal_I2cSendRawByte(PN544_BUS_ID,(PN544_CHIP_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(PN544_BUS_ID,((PN544_CHIP_ADDR<<1)|1),I2C_MASTER_WR | I2C_MASTER_STA);

	for(i=0;i<datalen-1;i++,data++)//data
	{	
		(*data)=hal_I2cReadRawByte(PN544_BUS_ID,I2C_MASTER_RD);
	}
	(*data)=hal_I2cReadRawByte(PN544_BUS_ID,I2C_MASTER_RD | I2C_MASTER_ACK | I2C_MASTER_STO);

//	SXS_TRACE(TSTDOUT, "rdabt_iic_rf_read_data: %d\n.");
}

INT32 nfcd_Write(UINT8* data,UINT8 length)
{
    EDRV_TRACE(EDRV_DBG_TRC, 0, "nfcd_write len:%d", length);
    pn544_I2cOpen();

    pn544_iic_write_data(data, length);

    pn544_I2cClose();
    return 0;
}

INT32 nfcd_Read(UINT8* data,UINT8 length)
{
    UINT32 *ack_data;
    pn544_I2cOpen();
    pn544_iic_read_data(data, length);
    pn544_I2cClose();
    if(*data != 0x51)
    {
    	  ack_data = data;
    	  hal_HstSendEvent(SYS_EVENT, *ack_data);
    }
    return 0;
} 

void nfcd_configration()
{
	create_hcp_packet(RESET_FRAME, 0, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&reset_packet, 6);  
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_OPEN_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_CLEAR_ALL_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_OPEN_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ADM_CREATE_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0x20, 0x00, 0x90);
	nfcd_Write((UINT8 *)&hcp_data3_packet, 9);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_OPEN_PIPE, PN544_IDMGT_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_pn544_packet(0x3e, 0x02, 3, 0x00, 0xf8, 0x30, 0);
	nfcd_Write((UINT8 *)&hcp_data3_packet, 9);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_GET_PARAMETER, PN544_IDMGT_PIPE, 0x02, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_data1_packet, 7);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_GET_PARAMETER, PN544_IDMGT_PIPE, 0x03, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_data1_packet, 7);
	nfcd_Read(pn544_buf, 10);
	//swp configration
	
}

void nfcd_GetVersion()
{
	create_hcp_packet(RESET_FRAME, 0, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&reset_packet, 6);  
	nfcd_Read(pn544_buf, 10);
	COS_Sleep(1);
	create_hcp_packet(ANY_OPEN_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	/*
	create_hcp_packet(ANY_CLEAR_ALL_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_OPEN_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ADM_CREATE_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0x05, 0, 0x05);
	nfcd_Write((UINT8 *)&hcp_data3_packet, 9);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_OPEN_PIPE, PN544_IDMGT_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_GET_PARAMETER, PN544_IDMGT_PIPE, 0x01, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_data1_packet, 7);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_GET_PARAMETER, PN544_IDMGT_PIPE, 0x02, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_data1_packet, 7);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_CLOSE_PIPE, PN544_IDMGT_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_DELETE_PIPE, PN544_IDMGT_PIPE, 0x01, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_data1_packet, 7);
	nfcd_Read(pn544_buf, 10);*/
}

void pn544_init()
{
	create_hcp_packet(RESET_FRAME, 0, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&reset_packet, 6);  
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ANY_OPEN_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0, 0, 0);
	nfcd_Write((UINT8 *)&hcp_packet, 6);
	nfcd_Read(pn544_buf, 10);
	create_hcp_packet(ADM_CREATE_PIPE, NFC_HCI_ADMIN_PIPE, 0, 0x05, 0, 0x05);
	nfcd_Write((UINT8 *)&hcp_data3_packet, 9);
	nfcd_Read(pn544_buf, 10);
}

void clear_mem()
{
	memset(pn544_buf, 0, 10);
}


