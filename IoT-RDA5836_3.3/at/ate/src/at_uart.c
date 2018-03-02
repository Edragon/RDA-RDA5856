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


// REVISION HISTORY:
// NAME              DATE                REMAKS
// Lixp             2012-2-21       Created initial version 1.0
//
// //////////////////////////////////////////////////////////////////////////////

#ifdef CHIP_IS_MODEM
#include "hal_ap_comm.h"
#else
#include "hal_uart.h"
#endif
#include "hal_host.h"
#if defined(WIN32)
#define HAL_UNCACHED_BSS
#else
#include "csw_csp.h"
#include "sxr_tls.h"
#include "hal_mem_map.h"
#endif /* WIN32 */
#include "at_cp.h"

#include "at_common.h"
#include "at_sa.h"
#include "at_uart.h"
#include "at_module.h"
#include "at_parser.h"
#include "cos.h"
#include "event.h"
#if defined(__AT_IN_DEBUG_HOST__)
#include "at_map.h"
#endif /* __AT_IN_DEBUG_HOST__ */

#include "dm.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#ifndef WIN32
#include "rdabt_sharemem.h"
#endif
#endif

#ifdef ADAPTIVE_BAUD_RATE
/* Some Parameters of SCM module */
#define UART_BAUDRATE_MAX_NUM  6
HAL_UART_BAUD_RATE_T g_UART_Current_Baudrate[UART_BAUDRATE_MAX_NUM] = { HAL_UART_BAUD_RATE_9600,
                                                                        HAL_UART_BAUD_RATE_14400,
                                                                        HAL_UART_BAUD_RATE_19200,
                                                                        HAL_UART_BAUD_RATE_38400,
                                                                        HAL_UART_BAUD_RATE_57600,
                                                                        HAL_UART_BAUD_RATE_115200
                                                                      };
UINT8 g_uart_baudrate_index   = UART_BAUDRATE_MAX_NUM - 1;
BOOL g_uart_baudrate_check_ok = FALSE;
#endif
BOOL rcv_flow_hold = TRUE;

PUBLIC UART_INFO_T  HAL_UNCACHED_BSS g_UartInfo;
UINT8 *g_pReceiveBuf = NULL;
UINT8 *uart_data = NULL;
UINT16 gReceiveBufLen = 0;

//struct
//{
//    UINT8 TX_CircularBuf[UART_PS_RX_BUFF_LEN];
//    UINT8 TX_Buf[UART_PS_RX_BUFF_LEN];
//    UINT16 Get;
//    UINT16 Put;
//    UINT8 NotEmpty;
//} g_Circular_Buf;
//
//struct
//{
//    UINT8 CircuBuf[UART_PS_RX_BUFF_LEN];
//    UINT8 FifoBuf[UART_MAX_FIFO_LEVEL];
//    UINT16 Buf_len;
//    UINT16 Get;
//    UINT16 Put;
//    UINT8 NotEmpty;
//} g_RxBuff;

static UART_CFG g_UartCfg =
{
    UART_BAUD_RATE_115200, UART_8_DATA_BITS, UART_1_STOP_BIT, UART_NO_PARITY, UART_AFC_MODE_RX_FULL
};

#if defined(__AT_IN_DEBUG_HOST__)

//static UINT8 HAL_UNCACHED_BSS atInDbgHstRxBuff[AT_CMD_LINE_BUFF_LEN];
//static UINT8 HAL_UNCACHED_BSS atInDbgHstTxBuff[AT_CMD_LINE_BUFF_LEN];

AT_IN_DBG_HST_ACCESS_T HAL_UNCACHED_DATA g_atInDbgHstAccess = 
{
	AT_DBG_HST_INVAILD,
	0,
	0,
	NULL,
	NULL
};
#if 0
{
	AT_DBG_HST_INVAILD,
	sizeof(atInDbgHstRxBuff),
	sizeof(atInDbgHstTxBuff),
	&atInDbgHstRxBuff,
	&atInDbgHstTxBuff
};
#endif
#endif /* __AT_IN_DEBUG_HOST__ */


/***********************************************************************************************************************
  INTERNAL FUNCTION PROTOTYPE DEFNITION SECTION
 ***********************************************************************************************************************/
uint16 DealWith_UartRxIrqDma(UINT8 id,BOOL  DMA_used, BOOL  Timeout);

static void check_cmd_suffix(VOID);
void check_cmd_suffixdma(UINT8 *rx_buff);
static void data_check_proc(void);
#if defined(__AT_IN_DEBUG_HOST__)
void uart_send_data_to_debug_host(const UINT8* str, const UINT16 len);
#endif /* __AT_IN_DEBUG_HOST__ */

#ifdef CHIP_IS_MODEM
VOID atcmd_EnableRecvIrq(BOOL enable);
#endif



/***********************************************************************************************************************
  FUNCTON IMPLEMENTATION SECTION
 ***********************************************************************************************************************/

/*----------------------------------------------------------------------------------------------------------------------
  RING BUFFER OPERATION SECTION
  ----------------------------------------------------------------------------------------------------------------------*/

#define TC_RING_BUFF_INFO(rb) \
    do { \
        AT_TC(g_sw_UART, "Ring buffer info: addr=%x,rp=%u,wp=%u,len=%u", (rb)->DataBuffer, (rb)->uReadPointer, (rb)->uWritePointer, (rb)->uDataLen); \
    } while (0)

/*----------------------------------------------------------------------------------------------------------------------
  COMMAND AND COMMAND LINE MODE SECTION
  ----------------------------------------------------------------------------------------------------------------------*/

//
// Process the line editing character(s) stored in parameter [s]. An editing character (commonly is BS) backspace and
// erase the previous character if it is present. The following is the algorithm description (a string demo, '#' means
// line editing character):
//
// H e # l # # l o #
// |---| |---|     |---|
// |--------------------|
//
// Based on this rule, I do two phases to process '#' character. In the first phase, I replace the "previous"
// character before '#' character by '#' from tail to head of the string, and in the second phase, erase all the
// '#' characters.
//
// <s>
// [in,out]The target command line string need to process.
// <RETURN>
// N/A
//
void ProcessLineEditingChar(unsigned char* s)
{
    unsigned char ch = 0; // Get line editing character and store it to a local variable.
    INT32 i          = 0, j = 0; // Point to current process character in target string.
    size_t n         = 0; // Counter line line editing character.
    size_t len       = 0; // String length.
    // Command line is empty, exit.
    if (s == NULL || strlen(s) == 0)
        return;
    ch = CHAR_BS;
    // Get the string length of command line for searching operation from tail to head.
    len = strlen(s);
    // Replace the "previous" character by '#' before '#' for every one from tail to head.
    for (i = len - 1; i >= 0; i--)
    {
        // If current character is '#', then increment the counter and skip this charcacter,
        // else will check if counter greater than zero, if it is, that means one or more backspace operation(s) need to
        // do, so do a replace operation and decrease the counter at the same time.
        if (s[i] == ch)
            n++;
        else
        {
            if (n > 0)
            {
                s[i] = ch;
                n--;
            }
        }
    }
    // Erasing all the '#' characters from head to tail of the string. It's a classic moving algorithm.
    i = 0, j = 0;
    while (s[i] != 0)
    {
        if (s[i] == ch)
            i++;
        else
            s[j++] = s[i++];
    }
    // Fill null-terminated character at the tail position.
    s[j] = 0;
}

#ifdef CHIP_IS_MODEM

UINT32 atcmd_ReadData(CONST UINT8 *buf, UINT32 len)
{
    if (len == 0)
    {
        return len;
    }
    UINT8 mode = COMMAND_ECHO_MODE_DEFUALT_VAL;

    if (mode == 1)
    {
        UINT16 n = len;
        UINT16 i = 0;
        CONST UINT8* p = buf;
        while (n > 0)
        {
            i = hal_ApCommSendAtCmd(p, n);
            n -= i;
            p += i;
        }
    }
    if (len >= AT_CMD_LINE_BUFF_LEN)
    {
        asm volatile ("break 1");
        return len;
    }
    // If buffer is full then clear buffer
    if (g_UartInfo.rx_buff_p + len >= AT_CMD_LINE_BUFF_LEN)
    {
        memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
        g_UartInfo.rx_buff_p = 0;
    }
    // Copy buffer
    memcpy(g_UartInfo.rx_buff + g_UartInfo.rx_buff_p, buf, len);
    g_UartInfo.rx_buff_p += len;
    g_UartInfo.rx_buff[g_UartInfo.rx_buff_p] = 0;
    return len;
}

UINT32 atcmd_RecvIrqHandler(CONST UINT8 *buf, UINT32 len)
{
    UINT32 read_data_len = atcmd_ReadData(buf, len);
    if (read_data_len > 0)
    {
        ProcessLineEditingChar(g_UartInfo.rx_buff);
        if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
        {
            g_UartInfo.rx_buff_p = strlen(g_UartInfo.rx_buff);
        }
        data_check_proc();  // Check whether buffer can be sent to ATE task.
    }
    // Tell HAL how many bytes of the data have been processed.
    // If any of the data is remaining, HAL will not clear the interrupt,
    // and this irq handler should disable the interrupt by calling
    // hal_ApCommSetAtCmdRecvCallback(NULL). When AT application can
    // handle more data later, the application can re-enable the interrupt
    // by calling hal_ApCommSetAtCmdRecvCallback(&irqHandler)
    return len;
}

#else // !CHIP_IS_MODEM

typedef union
{
    HAL_UART_IRQ_STATUS_T st;
    UINT32 sui;
} st_type;
st_type irq_status;
extern u8 hal_uart_RxFifoLevel(HAL_UART_ID_T uartId);

#ifdef __2UART_SHARE_AT__

/*FUNCTION:
  Read data from UART device by calling the APIs of HAL layer and store the data into [g_UartInfo.rx_buff] and at the same time
  need to change [g_UartInfo.rx_buff_p] pointer.
PARAMETER:
N/A

RETURN:
Returns the count of read data from UART device, in bytes.**/

UINT16 read_data_from_uart(HAL_UART_ID_T pUart)
{
    UINT8 rcvbuffer[UART_MAX_FIFO_LEVEL] = { 0 };
    UINT8 rcvlen                         = 0;
    UINT16 rcvedlen                      = 0;
    UINT16 total_len                     = 0;
    UINT8 mode                           = COMMAND_ECHO_MODE_DEFUALT_VAL;
    //  AT_TC_ENTER_FUNC(g_sw_UART);
    while (1)
    {
        // 查询FIFO buffer中的数据字节数.
        rcvlen = hal_uart_RxFifoLevel(pUart);
        if (rcvlen == 0)
        {
            // AT_TC(g_sw_UART, "Totally, read data from UART in bytes = %d", total_len);
            return total_len;
        }
        // 试图读取16 bytes.
        rcvedlen = hal_uart_GetData(pUart, rcvbuffer, 16);
#ifdef ADAPTIVE_BAUD_RATE
        if ((g_uart_baudrate_check_ok == FALSE) && (rcvlen > 2))
        {
            // AT or at
            if (0x00 != strstr(rcvbuffer, "at") || 0x00 != strstr(rcvbuffer, "AT"))
                // if((rcvbuffer[0] == 0x41 && rcvbuffer[1] == 0x54) || (rcvbuffer[0] == 0x61 && rcvbuffer[1] == 0x74))
            {
                g_uart_baudrate_check_ok = TRUE;
            }
            else
            {
                if (g_uart_baudrate_index == 0)
                {
                    g_uart_baudrate_index = UART_BAUDRATE_MAX_NUM;
                }
                g_uart_baudrate_index--;
                AT_UartReopen(ds_low);
                return 0;
            }
        }
#endif
        total_len += rcvedlen;
        // AT_TC(g_sw_UART, "Read %d bytes data from UART.", rcvedlen);
        // /CFW_CfgGetEchoMode(&mode);
        if (mode == 1)
        {
            UINT16 n = rcvedlen;
            UINT16 i = 0;
            UINT8* p = rcvbuffer;
            while (n > 0)
            {
                i = hal_uart_SendData(pUart, p, n);
                n -= i;
                p += i;
            }
        }
        /*
           if buff is full then
           clear buffer
           end
         * */
        if (strlen(g_UartInfo.rx_buff) + rcvedlen >= AT_CMD_LINE_BUFF_LEN)
        {
            memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
            g_UartInfo.rx_buff_p = 0;
        }
        /*
           copy rcvbuff to rxbuff
         * */
        memcpy(g_UartInfo.rx_buff + g_UartInfo.rx_buff_p, rcvbuffer, rcvedlen);
        g_UartInfo.rx_buff_p += rcvedlen;
        g_UartInfo.rx_buff[g_UartInfo.rx_buff_p] = 0;
    }
    // No possible to reach here.
    AT_ASSERT(0); // No possible to reach here.
}

VOID irq_Uart1Processor(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    irq_status.sui = 0x00;
    irq_status.st  = status;
    // AT_TC(g_sw_UART, "irq_Uart1Processor!status: 0x%x", irq_status.sui);
    if (status.rxDataAvailable || status.rxTimeout)
    {
        int read_data_len = read_data_from_uart(HAL_UART_1);
        if (read_data_len > 0)
        {
            if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
            {
                g_UartInfo.rx_buff_p = strlen(g_UartInfo.rx_buff);
            }
            data_check_proc();
        }
    }
    // hal_uart_ClearErrorStatus(g_UartInfo.Scom_UartId); //by wulc
    hal_uart_ClearErrorStatus(HAL_UART_1);
    return;
}

VOID irq_Uart2Processor(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    irq_status.sui = 0x00;
    irq_status.st  = status;
    AT_TC(g_sw_UART, "irq_Uart2Processor!status: 0x%x", irq_status.sui);
    if (status.rxDataAvailable || status.rxTimeout)
    {
        int read_data_len = read_data_from_uart(HAL_UART_2);
        if (read_data_len > 0)
        {
            if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
            {
                g_UartInfo.rx_buff_p = strlen(g_UartInfo.rx_buff);  // Lixp temp
            }
            data_check_proc();
        }
    }
    // hal_uart_ClearErrorStatus(g_UartInfo.Scom_UartId);
    hal_uart_ClearErrorStatus(HAL_UART_2);
    return;
}
#else
static UINT16 read_data()
{
    UINT8 rcvbuffer[UART_MAX_FIFO_LEVEL] = { 0 };
    UINT8 rcvlen                         = 0;
    UINT16 rcvedlen                      = 0;
    UINT16 total_len                     = 0;
    UINT8 mode                           = COMMAND_ECHO_MODE_DEFUALT_VAL;
    //  AT_TC_ENTER_FUNC(g_sw_UART);
    while (1)
    {
        // 查询FIFO buffer中的数据字节数.
        rcvlen = hal_uart_RxFifoLevel((HAL_UART_ID_T) g_UartInfo.Scom_UartId);
        if (rcvlen == 0)
        {
            // AT_TC(g_sw_UART, "Totally, read data from UART in bytes = %d", total_len);
            return total_len;
        }
        // 试图读取16 bytes.
        rcvedlen = hal_uart_GetData(g_UartInfo.Scom_UartId, rcvbuffer, 16);
#ifdef ADAPTIVE_BAUD_RATE
        if ((g_uart_baudrate_check_ok == FALSE) && (rcvlen > 2))
        {
            // AT or at
            if (0x00 != strstr(rcvbuffer, "at") || 0x00 != strstr(rcvbuffer, "AT"))
                // if((rcvbuffer[0] == 0x41 && rcvbuffer[1] == 0x54) || (rcvbuffer[0] == 0x61 && rcvbuffer[1] == 0x74))
            {
                g_uart_baudrate_check_ok = TRUE;
            }
            else
            {
                if (g_uart_baudrate_index == 0)
                    g_uart_baudrate_index = UART_BAUDRATE_MAX_NUM;
                g_uart_baudrate_index--;
                AT_UartReopen(ds_low);
                return 0;
            }
        }
#endif
        total_len += rcvedlen;
        // AT_TC(g_sw_UART, "Read %d bytes data from UART.", rcvedlen);
        // if (!bMuxActiveStatus)
        {
            // /CFW_CfgGetEchoMode(&mode);
            if (mode == 1)
            {
                UINT16 n = rcvedlen;
                UINT16 i = 0;
                UINT8* p = rcvbuffer;
                while (n > 0)
                {
                    i = hal_uart_SendData(g_UartInfo.Scom_UartId, p, n);
                    n -= i;
                    p += i;
                }
            }
        }
        /*
           if buff is full then
           clear buffer
           end
         * */
        if (/*strlen(g_UartInfo.rx_buff)*/g_UartInfo.rx_buff_p + rcvedlen >= AT_CMD_LINE_BUFF_LEN)
        {
            memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
            g_UartInfo.rx_buff_p = 0;
        }
        /*
           copy rcvbuff to rxbuff
         * */
        memcpy(g_UartInfo.rx_buff + g_UartInfo.rx_buff_p, rcvbuffer, rcvedlen);
        g_UartInfo.rx_buff_p += rcvedlen;
        g_UartInfo.rx_buff[g_UartInfo.rx_buff_p] = 0;
    }
    // No possible to reach here.
    AT_ASSERT(0);
}

#ifdef CODEC_SUPPORT
static UINT32 read_raw_data(UINT8 *data_ptr, UINT32 data_len)
{
    UINT8 rcvlen                         = 0;
    UINT16 rcvedlen                      = 0;
    UINT32 total_len                     = 0;
    //  AT_TC_ENTER_FUNC(g_sw_UART);
    while (1)
    {
        // 查询FIFO buffer中的数据字节数.
        rcvlen = hal_uart_RxFifoLevel((HAL_UART_ID_T) g_UartInfo.Scom_UartId);
        if (rcvlen == 0)
        {
            // AT_TC(g_sw_UART, "Totally, read data from UART in bytes = %d", total_len);
            return total_len;
        }

        //rcvlen = MIN(rcvlen, data_len);

        // 试图读取16 bytes.
        rcvedlen = hal_uart_GetData(g_UartInfo.Scom_UartId, data_ptr, data_len);
        total_len += rcvedlen;
        data_ptr += rcvedlen;
        data_len -= rcvedlen;

        if(data_len == 0)
        {
            return total_len;
        }
    }
    // No possible to reach here.
    AT_ASSERT(0);
}
extern UINT32 uart_play_read_raw_data_len;
extern UINT32 AT_MUSIC_Receive_Data(UINT8 *data, UINT32 len);
extern int AT_MUSIC_UartPlay_GetStatus(void);

//#define UART_USE_SWAP_BUF
#ifdef UART_USE_SWAP_BUF
static UINT8 uart_rx_buf[AT_CMD_LINE_BUFF_LEN];
static UINT8 g_atIrqMutex = 0;
#endif


VOID irq_processor(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    BOOL OK_response = FALSE;

    irq_status.sui = 0x00;
    irq_status.st  = status;
    AT_TC(g_sw_UART, "irq_processor!status: 0x%x", irq_status.sui);

    if (error.rxOvflErr)
    {
        //Rx fifo overflow
        hal_HstSendEvent (BOOT_EVENT, 0xddff4401);
        AT_ASSERT(0);
        return;
    }

    if (status.rxDataAvailable || status.rxTimeout)
    {
#ifdef CODEC_SUPPORT
        while (uart_play_read_raw_data_len > 0)
        {
            UINT32 music_add_data_len;
            UINT32 empty_buffer_len;
            UINT8 *empty_buffer_ptr;

#if 1
            empty_buffer_len = mmc_GetWriteBuffer (&empty_buffer_ptr);
            empty_buffer_len = read_raw_data (
                    empty_buffer_ptr,
                    MIN(uart_play_read_raw_data_len, empty_buffer_len));

            music_add_data_len = AT_MUSIC_Receive_Data (empty_buffer_ptr, empty_buffer_len);
#else
            UINT8 rcvbuffer[UART_MAX_FIFO_LEVEL] = {0};
            empty_buffer_ptr = rcvbuffer;
            empty_buffer_len = UART_MAX_FIFO_LEVEL;

            music_add_data_len = read_raw_data(empty_buffer_ptr,
                    MIN(uart_play_read_raw_data_len,
                            empty_buffer_len));

            music_add_data_len = AT_MUSIC_Receive_Data(empty_buffer_ptr, music_add_data_len);
#endif /* 0 */

            uart_play_read_raw_data_len -= music_add_data_len;

            if (uart_play_read_raw_data_len == 0)
            {
                //Raw data end
                COS_EVENT event = { 0 };
                //hal_uart_SendData (g_UartInfo.Scom_UartId, "\r\nOK\r\n", sizeof("\r\nOK\r\n") - 1);
                event.nEventId = UART_RAWDATA_REC_FINISH;
                event.nParam1 = 0;
                COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
                break;
            }
            else if (hal_uart_RxFifoLevel ((HAL_UART_ID_T) g_UartInfo.Scom_UartId))
            {
                //continue read raw data
                continue;
            }
            else
            {
                //No rx data
                hal_uart_ClearErrorStatus (g_UartInfo.Scom_UartId);
                return;
            }
        }
#endif
        UINT16 read_data_len = read_data ();

        if (read_data_len > 0)
        {
            ProcessLineEditingChar(g_UartInfo.rx_buff);
            if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
            {
                g_UartInfo.rx_buff_p = strlen(g_UartInfo.rx_buff);
            }

            data_check_proc();  // Check whether buffer can be sent to ATE task.
        }
    }

    else if (status.rxDmaDone || status.rxDmaTimeout)
    {
        UINT32 uDataSize = AT_CMD_LINE_BUFF_LEN;

        if (status.rxDmaTimeout)
        {
            hal_UartRxDmaPurgeFifo (g_UartInfo.Scom_UartId);
            while (!hal_UartRxDmaFifoEmpty (g_UartInfo.Scom_UartId));
            uDataSize -= hal_UartRxDmaLevel (g_UartInfo.Scom_UartId);
            while (!hal_UartRxDmaStop (g_UartInfo.Scom_UartId));
        }

        hal_HstSendEvent (SYS_EVENT, 0xddff0002);
        hal_HstSendEvent (SYS_EVENT, (uDataSize << 16) | (status.rxDmaDone << 8) | (status.rxDmaTimeout));

#ifdef UART_USE_SWAP_BUF
        while (g_atIrqMutex);
        g_atIrqMutex = 1;

        // copy swap buf to rx buf and start DMA again.
        memcpy(g_UartInfo.rx_buff, uart_rx_buf, uDataSize);
        if (0 == hal_uart_GetData(g_UartInfo.Scom_UartId, uart_rx_buf, AT_CMD_LINE_BUFF_LEN))
        {
            hal_HstSendEvent (SYS_EVENT, 0xddff4407);
            AT_ASSERT(0);
        }
#endif
        if (uart_play_read_raw_data_len > 0)
        {
            UINT32 data_len, music_add_data_len;
            UINT32 empty_buffer_len;
            UINT8 *empty_buffer_ptr;

            music_add_data_len = MIN(uart_play_read_raw_data_len, uDataSize);

            data_len = AT_MUSIC_Receive_Data (g_UartInfo.rx_buff, music_add_data_len);
            if (music_add_data_len != data_len)
            {
                hal_HstSendEvent (SYS_EVENT, 0xddff4403);
                AT_ASSERT(0);
            }

            uart_play_read_raw_data_len -= music_add_data_len;
            uDataSize -= music_add_data_len;

            if (uart_play_read_raw_data_len == 0)
            {
                hal_HstSendEvent (SYS_EVENT, 0xddff0066);
                OK_response = TRUE;
                AT_MUSIC_StopReadRawData();

                //AT cmd after raw data
                if (uDataSize > 0)
                {
                    //Overwrite raw data with AT cmd
                    memcpy (g_UartInfo.rx_buff,
                            g_UartInfo.rx_buff + music_add_data_len,
                            uDataSize);
                    g_UartInfo.rx_buff[uDataSize] = '\0';
                }
                else
                {
                    memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
                }
            }
            else
            {
                hal_HstSendEvent (SYS_EVENT, 0xddff0402);
                hal_HstSendEvent (SYS_EVENT, uDataSize);
                //AT_ASSERT(0 == uDataSize);
            }

            g_UartInfo.rx_buff_p = uDataSize;
        }

        if (uDataSize > 0)
        {
            if((strncmp(g_UartInfo.rx_buff, "AT+MURAWDATA=", 13) == 0)
                && (AT_MUSIC_UartPlay_GetStatus() == 1/*UART_PLAY_STATUS_PLAY*/)
                && (mmc_GetEmptyBytes() >= atoi(g_UartInfo.rx_buff+13)))
            {
                hal_HstSendEvent (SYS_EVENT, 0xddff0078);
                hal_HstSendEvent (SYS_EVENT, atoi(g_UartInfo.rx_buff+13));
                OK_response = TRUE;

                AT_MUSIC_StartReadRawData(atoi(g_UartInfo.rx_buff+13));
                g_UartInfo.rx_buff_p = 0;
            }
            else if (strncmp(g_UartInfo.rx_buff, "AT+", 3) == 0)
            {
                hal_HstSendEvent (SYS_EVENT, 0xddff0079);
                hal_HstSendEvent (SYS_EVENT, uDataSize<<16 | g_UartInfo.data_Mode);
                hal_HstSendEvent (SYS_EVENT, g_UartInfo.rx_buff);

                // AT command max length: 256 bytes.
                if (uDataSize > 256) {
                    uDataSize = 256;
                    g_UartInfo.rx_buff[256] = '\0';
                }

                if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
                {
                    g_UartInfo.rx_buff_p = uDataSize;
                    memset(g_UartInfo.rx_buff+uDataSize, 0x0, AT_CMD_LINE_BUFF_LEN-uDataSize);
                    data_check_proc (); // Check whether buffer can be sent to ATE task.
                    hal_HstSendEvent (SYS_EVENT, 0xddff0179);
                }
            }
            else
            {
                // AT command in rawdata.
                UINT8 *str = (strstr(g_UartInfo.rx_buff, "AT+"));
                int offset = 0;
                if ((str != NULL) && ((offset = str-g_UartInfo.rx_buff) < uDataSize)) {
                    hal_HstSendEvent (SYS_EVENT, 0xddff0405);
                    hal_HstSendEvent (SYS_EVENT, offset);
                    memcpy(g_UartInfo.rx_buff, str, uDataSize-offset);
                    g_UartInfo.rx_buff[uDataSize-offset] = '\0';
                    g_UartInfo.rx_buff_p = uDataSize-offset;
                    data_check_proc (); // Check whether buffer can be sent to ATE task.
                    hal_HstSendEvent (SYS_EVENT, 0xddff0425);
                } else {
                    AT_MUSIC_StopReadRawData();
                    hal_HstSendEvent(SYS_EVENT, 0xddff4406);
                    g_UartInfo.rx_buff_p == 0;
                    OK_response = TRUE;
                }
            }
        }

#ifndef UART_USE_SWAP_BUF
        if (0 == hal_uart_GetData(g_UartInfo.Scom_UartId,
            g_UartInfo.rx_buff+g_UartInfo.rx_buff_p, AT_CMD_LINE_BUFF_LEN-g_UartInfo.rx_buff_p))
        {
            hal_HstSendEvent (SYS_EVENT, 0xddff4407);
            AT_ASSERT(0);
        }
#else
        g_atIrqMutex = 0;
#endif
    }

    if(HAL_ERR_NO != hal_UartGetErrorStatus(g_UartInfo.Scom_UartId))
    {
        hal_HstSendEvent (SYS_EVENT, 0xddff4408);
        hal_HstSendEvent (SYS_EVENT, uart_play_read_raw_data_len);
        hal_HstSendEvent (SYS_EVENT, hal_UartGetErrorStatus(g_UartInfo.Scom_UartId));
        //AT_ASSERT(0);
        hal_UartClearErrorStatus(g_UartInfo.Scom_UartId); // clear error status and process continue.
    }

    if (OK_response)
    {
        hal_HstSendEvent (SYS_EVENT, 0xddff0088);
        hal_uart_SendData (g_UartInfo.Scom_UartId, "\r\nOK\r\n", sizeof("\r\nOK\r\n") - 1);
    }
}

#else /* CODEC_SUPPORT */


VOID irq_processor(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    irq_status.sui = 0x00;
    irq_status.st  = status;
    AT_TC(g_sw_UART, "irq_processor!status: 0x%x", irq_status.sui);

    if (error.rxOvflErr)
    {
        //Rx fifo overflow
        hal_HstSendEvent (BOOT_EVENT, 0xddff4401);
        AT_ASSERT(0);
        return;
    }

    if (status.rxDataAvailable || status.rxTimeout)
    {

        UINT16 read_data_len = read_data();
        if (read_data_len > 0)
        {
            ProcessLineEditingChar(g_UartInfo.rx_buff);
            if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
            {
                g_UartInfo.rx_buff_p = strlen(g_UartInfo.rx_buff);
            }

            data_check_proc();  // Check whether buffer can be sent to ATE task.
        }
    }

    if(HAL_ERR_NO != hal_UartGetErrorStatus(g_UartInfo.Scom_UartId))
    {
        hal_HstSendEvent (SYS_EVENT, hal_UartGetErrorStatus(g_UartInfo.Scom_UartId));
        //AT_ASSERT(0);
        hal_UartClearErrorStatus(g_UartInfo.Scom_UartId); // clear error status and process continue.
    }

}
#endif  /* CODEC_SUPPORT */

#endif

#endif // !CHIP_IS_MODEM


/***********************************************************************
 *this is main entry of SCM module dispose the data from uart, and set
 *the interface to other module.
 *
 ************************************************************************/

#if (LF_PATCH == 1)
/**********************************************************
  Discard the suffix <lf> character of a command line when the <lf> not received by UART driver (hal) in the same interrupt processing. That means the last
  suffix <lf> character is received by the next interrupt processing.

  Commonly the suffix <lf> will be processed in the same Rx interruption like that:
  AT Command Line<cr><lf>
  but sometimes, it maybe the situation like that:
  AT Command Line<cr>......................................dealy...........................<lf>
  in this case, <lf> will be processed in next interruption. :)
 ***********************************************************/
void DiscardSuffixLF(void)
{
    if (strlen(g_UartInfo.rx_buff) == 1 && g_UartInfo.rx_buff[0] == AT_CMD_LF)
    {
        memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
        g_UartInfo.rx_buff_p = 0;
    }
}

#endif

static void data_check_proc()
{
#if (LF_PATCH == 1)
    DiscardSuffixLF();
#endif
    if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
    {
        ProcessLineEditingChar(g_UartInfo.rx_buff);
        check_cmd_suffix();
    }
    //check_cmd_suffix();
}


// Allocate memory and fill receieved data and send an OS message with the new memory block to ATE task.
void SendCmdToATTask(UINT8* rxData,UINT16 length)
{
    PUART_DATA pUartData = NULL;
    UINT16 Data_Len;
    COS_EVENT event = { 0 };
    BOOL r          = FALSE;
    AT_TC(g_sw_UART, "Time out to send  data in Rx buffer.");
    if (length == 0)
        return;

    AT_ASSERT(length <= ATP_CMD_LINE_SIZE);
    Data_Len  = length + 1;
    pUartData = AT_MALLOC(sizeof(UART_DATA) + Data_Len);

    AT_ASSERT((pUartData != NULL));
    AT_MemSet(pUartData, 0, sizeof(UART_DATA) + Data_Len);
    pUartData->DataLen = (UINT32)length;
    memcpy(pUartData->DataBuf, rxData, length);
    pUartData->DataBuf[length] = '\0';
    
    event.nEventId                           = EV_DM_UART_RECEIVE_DATA_IND;
    event.nParam1                            = (UINT32)(pUartData);
    //event.nParam2                            = 0;
    //event.nParam3                            = 0;
    r = COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    if (r == TRUE)
    {
        AT_TC(g_sw_UART, "Sent data to ATE by sending an COS message to the ATE task; .nEventId=%u,nParam1=%u",
              event.nEventId, event.nParam1);
    }
    else
    {
        AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
    }
}
#if (0)

void Ip_data_TimeOut()
{
    PUART_DATA pUartData = NULL;
    UINT16 Data_Len;
    COS_EVENT event = { 0 };
    BOOL r          = FALSE;
    AT_TC(g_sw_UART, "Time out to send  data in Rx buffer.");
    if (g_UartInfo.rx_buff_p == 0)
        return;
    Data_Len  = g_UartInfo.rx_buff_p + 1;
    pUartData = AT_MALLOC(sizeof(UART_DATA) + Data_Len);
    AT_MemSet(pUartData, 0, sizeof(UART_DATA) + Data_Len);
    pUartData->DataLen = (UINT32)Data_Len;
    memcpy(pUartData->DataBuf, g_UartInfo.rx_buff, g_UartInfo.rx_buff_p);
    pUartData->DataBuf[g_UartInfo.rx_buff_p] = AT_CMD_CTRL_Z;
    event.nEventId                           = EV_DM_UART_RECEIVE_DATA_IND;
    event.nParam1                            = (UINT32)(pUartData);
    r = COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    if (r == TRUE)
    {
        AT_TC(g_sw_UART, "Sent data to ATE by sending an COS message to the ATE task; .nEventId=%u,nParam1=%u",
              event.nEventId, event.nParam1);
    }
    else
    {
        AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
    }
    memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
    g_UartInfo.rx_buff_p = 0;
}
#endif /* 0 */

/***********************************************************************
 * check the tail of AT cmd from uart,the function should be
 * called when find the head of uart is right
 ************************************************************************/

BOOL findchr(const char* p, char c)
{
    const char* pr = p;
    while (*pr != 0x00)
    {
        if (*pr == c)
        {
            return TRUE;
        }
        pr++;
    }
    return FALSE;
}

/**************************************************************************************************
 * A line terminated char (default CR) or A/ or a/ command means a valid command line is comming.
 * If can't find these character or string then return, means not find suffix char of a command line.
 ***************************************************************************************************/
static void check_cmd_suffix(VOID)
{
    //PUART_DATA pUartData = NULL;
    //COS_EVENT event      = { 0 };
    //BOOL r               = FALSE;
    UINT8 S3Set          = 0;
    if (g_UartInfo.data_Mode != AT_STATUS_CMD_LINE)
    {
        return;
    }

    S3Set = CHAR_CR;
    // A line terminated char (default CR) or A/ or a/ command means a valid command line is comming. If can't find these
    // character or string then return, means not find suffix char of a command line.
    // AT_TC(g_sw_UART, "Sent data to ATE by sending an COS message to the ATE task; .nEventId=%u,nParam1=%u",
    AT_TC(g_sw_UART, "check_cmd_suffix:S3Set:  0x%x, A/ : %d; a/: %d; ;g_UartInfo.rx_buff: 0x%x", S3Set,
          strstr(g_UartInfo.rx_buff, "A/"), strstr(g_UartInfo.rx_buff, "a/"), g_UartInfo.rx_buff);
    // AT_TC_MEMBLOCK(g_sw_SA, g_UartInfo.rx_buff, 50, 16);
    if (findchr((const char*)g_UartInfo.rx_buff, S3Set) == FALSE &&
        strstr(g_UartInfo.rx_buff, "A/") == 0 && strstr(g_UartInfo.rx_buff, "a/") == 0)
    {
        return;
    }
    // Just a single line terminated character, clear buffer and return.
    AT_TC(g_sw_UART, "check_cmd_suffix:strlen: %d; ;g_UartInfo.rx_buff: 0x%x; S3Set: %d", strlen(g_UartInfo.rx_buff),
          g_UartInfo.rx_buff[0], S3Set);
    if (strlen(g_UartInfo.rx_buff) == 1 && g_UartInfo.rx_buff[0] == S3Set)
        goto lab_ClearRxBuffer;

    SendCmdToATTask(g_UartInfo.rx_buff,strlen(g_UartInfo.rx_buff));
    
lab_ClearRxBuffer:
    AT_TC(g_sw_UART, "lab_ClearRxBuffer.");
    memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
    g_UartInfo.rx_buff_p = 0;
}
#if (0)
void check_cmd_suffixdma(UINT8 *rx_buff)
{
    PUART_DATA pUartData = NULL;
    COS_EVENT event      = { 0 };
    BOOL r               = FALSE;
    UINT8 S3Set          = 0;
    if (g_UartInfo.data_Mode != AT_STATUS_CMD_LINE)
    {
        check_sms_data_taildma(rx_buff, strlen(rx_buff));
        return;
    }
    S3Set = CHAR_CR;
    // A line terminated char (default CR) or A/ or a/ command means a valid command line is comming. If can't find these
    // character or string then return, means not find suffix char of a command line.
    // AT_TC(g_sw_UART, "Sent data to ATE by sending an COS message to the ATE task; .nEventId=%u,nParam1=%u",
    AT_TC(g_sw_UART, "check_cmd_suffixdma:S3Set:  0x%x, A/ : %d; a/: %d; ;rx_buff: 0x%x", S3Set,
          strstr(rx_buff, "A/"), strstr(rx_buff, "a/"), rx_buff);
    // AT_TC_MEMBLOCK(g_sw_SA, rx_buff, 50, 16);
    if (findchr(rx_buff, S3Set) == FALSE &&
        strstr(rx_buff, "A/") == 0 && strstr(rx_buff, "a/") == 0)
    {
        return;
    }
    // Just a single line terminated character, clear buffer and return.
    AT_TC(g_sw_UART, "check_cmd_suffix:strlen: %d; ;rx_buff: 0x%x; S3Set: %d", strlen(rx_buff),
          rx_buff[0], S3Set);
    if (strlen(rx_buff) == 1 && rx_buff[0] == S3Set)
        goto lab_ClearRxBuffer;
    // Allocate memory and fill receieved data and send an OS message with the new memory block to ATE task.
    pUartData = AT_MALLOC(sizeof(UART_DATA) + strlen(rx_buff) + 1);
    AT_MemSet(pUartData, 0, sizeof(UART_DATA) + strlen(rx_buff) + 1);
    pUartData->DataLen = (UINT32)strlen(rx_buff);
    memcpy(pUartData->DataBuf, rx_buff, pUartData->DataLen);
    event.nEventId = EV_DM_UART_RECEIVE_DATA_IND;
    event.nParam1 = (UINT32)(pUartData);
    event.nParam2 = 0;
    event.nParam3 = 0;
    r = COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    if (r == TRUE)
    {
        AT_TC(g_sw_UART, "Sent data to ATE by sending an COS message to the ATE task; .nEventId=%u,nParam1=%u",
              event.nEventId, event.nParam1);
    }
    else
    {
        AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
    }
lab_ClearRxBuffer:
    memset(g_pReceiveBuf, 0, UART_PS_RX_BUFF_LEN);
    gReceiveBufLen = 0;
    AT_TC(g_sw_UART, "lab_ClearRxBuffer.");
}
#endif /* 0 */
#ifdef CFW_GPRS_SUPPORT
extern UINT8 gIPSendFlag;
extern UINT16 gIPSendLen;
#endif

/*----------------------------------------------------------------------------------------------------------------------
  PS MODE SECTION
  ----------------------------------------------------------------------------------------------------------------------*/
#if (0)

void uart_DmaSendData_to_Hal(char in)
{
    UINT16 Block_Len, First_half, Second_half, real_len, real_sent_len, tmp_Get;
    real_sent_len = 0;
    Block_Len     = GET_DATA_BLOCK_LEN(g_Circular_Buf.Get, g_Circular_Buf.Put, UART_PS_RX_BUFF_LEN);
    if ((Block_Len == 0) && (g_Circular_Buf.NotEmpty))
    {
        Block_Len = UART_PS_RX_BUFF_LEN;
    }
    real_len = (Block_Len < UART_MAX_ONE_DMA_LEN) ? Block_Len : UART_MAX_ONE_DMA_LEN;
    if (real_len > 0) /* have data to send */
    {
        if (g_Circular_Buf.Get < g_Circular_Buf.Put)
        {
            memcpy(g_Circular_Buf.TX_Buf, &g_Circular_Buf.TX_CircularBuf[g_Circular_Buf.Get], real_len);
            tmp_Get = MOD_BUFF_LEN(g_Circular_Buf.Get + real_len);
        }
        else
        {
            First_half = UART_PS_RX_BUFF_LEN - g_Circular_Buf.Get;
            if (real_len < First_half)
            {
                memcpy(g_Circular_Buf.TX_Buf, &g_Circular_Buf.TX_CircularBuf[g_Circular_Buf.Get], real_len);
                tmp_Get = MOD_BUFF_LEN(g_Circular_Buf.Get + real_len);
            }
            else
            {
                memcpy(g_Circular_Buf.TX_Buf, &g_Circular_Buf.TX_CircularBuf[g_Circular_Buf.Get], First_half);
                Second_half = real_len - First_half;
                tmp_Get     = MOD_BUFF_LEN(g_Circular_Buf.Get + First_half);
                memcpy(&g_Circular_Buf.TX_Buf[First_half], &g_Circular_Buf.TX_CircularBuf[tmp_Get], Second_half);
                tmp_Get = MOD_BUFF_LEN(tmp_Get + Second_half);
            }
        }
#ifdef CHIP_IS_MODEM
        real_sent_len = 0;
        while (real_sent_len != real_len)
        {
            real_sent_len += (UINT16)hal_ApCommSendAtCmd(g_Circular_Buf.TX_Buf+real_sent_len,
                             real_len-real_sent_len);
            if (in)
            {
                if (real_sent_len != real_len)
                {
                    COS_Sleep(5);
                }
            }
        }
#else // !CHIP_IS_MODEM
#ifdef __2UART_SHARE_AT__
        real_sent_len = hal_uart_SendData(HAL_UART_1, g_Circular_Buf.TX_Buf, real_len);
        real_sent_len = hal_uart_SendData(HAL_UART_2, g_Circular_Buf.TX_Buf, real_len);
#else
        real_sent_len = hal_uart_SendData(g_UartInfo.Scom_UartId, g_Circular_Buf.TX_Buf, real_len);
#endif
#endif // !CHIP_IS_MODEM
        g_Circular_Buf.Get = tmp_Get;
        if (g_Circular_Buf.Get == g_Circular_Buf.Put)
            g_Circular_Buf.NotEmpty = 0;
    }
    else
    {
    }
}

UINT16 put_data_to_CircularBufferDma(CONST UINT8 *pBuff,UINT16 uDataSize)
{
    UINT16 Block_Len, Remain_Len, real_Len, First_half, Second_half;
    UINT32 scStatus = hal_SysEnterCriticalSection();
    Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, g_RxBuff.Buf_len);
    if ((Block_Len == 0) && (g_RxBuff.NotEmpty)) //g_RxBuff.NotEmpty == 1 应该为0，这个时候为1，所以导致死机
    {
        Block_Len = g_RxBuff.Buf_len;
    }
    Remain_Len = g_RxBuff.Buf_len - Block_Len;//Block_Len == 4k
    if(Remain_Len==0)
    {
        hal_SysExitCriticalSection(scStatus);
        asm volatile ("break 1");
        return 0;
    }
    if (uDataSize > Remain_Len)
    {
        hal_SysExitCriticalSection(scStatus);
        asm volatile ("break 1");
        return 0;
    }
    if( (Remain_Len <= (UART_PS_MAX_FIFO_LEVEL+uDataSize)) &&
        (rcv_flow_hold == FALSE) )
    {
#ifdef CHIP_IS_MODEM
        atcmd_EnableRecvIrq(FALSE);
#else
        hal_UartSetRts((HAL_UART_ID_T) g_UartInfo.Scom_UartId, FALSE);
#endif
        rcv_flow_hold = TRUE;
    }
    real_Len = (uDataSize < Remain_Len) ? uDataSize : Remain_Len;
    if (real_Len > 0)
    {
        if (g_RxBuff.Put < g_RxBuff.Get)
        {
            memcpy(&(g_RxBuff.CircuBuf[g_RxBuff.Put]), pBuff, real_Len);
            g_RxBuff.Put = MOD_BUFF_LEN1(g_RxBuff.Put + real_Len,g_RxBuff.Buf_len);
        }
        else
        {
            First_half = g_RxBuff.Buf_len - g_RxBuff.Put;
            if (real_Len < First_half)
            {
                memcpy(&(g_RxBuff.CircuBuf[g_RxBuff.Put]), pBuff, real_Len);
                g_RxBuff.Put = MOD_BUFF_LEN1(g_RxBuff.Put + real_Len, g_RxBuff.Buf_len);
            }
            else
            {
                memcpy(&(g_RxBuff.CircuBuf[g_RxBuff.Put]), pBuff , First_half);
                Second_half = real_Len - First_half;
                g_RxBuff.Put = MOD_BUFF_LEN1((g_RxBuff.Put + First_half),g_RxBuff.Buf_len);
                memcpy(&(g_RxBuff.CircuBuf[g_RxBuff.Put]), &pBuff[First_half], Second_half);
                g_RxBuff.Put = MOD_BUFF_LEN1((g_RxBuff.Put + Second_half),g_RxBuff.Buf_len);
            }
        }
        g_RxBuff.NotEmpty = 1;
    }
    else
    {
        //SXS_TRACE(TSTDOUT,"warning put len %x",real_Len);
    }
    hal_SysExitCriticalSection(scStatus);
    return Remain_Len - real_Len;
}

#ifdef CHIP_IS_MODEM

UINT32 atcmd_RecvIrqHandlerPsMode(CONST UINT8 *buf, UINT32 len)
{
    UINT16 Remain_Len;
    //AT_TC(g_sw_UART, "In atcmd_RecvIrqHandlerPsMode: len = %d", len);
    Remain_Len = put_data_to_CircularBufferDma(buf, len);
    //AT_TC(g_sw_UART, "In atcmd_RecvIrqHandlerPsMode g_RxBuff.Get: %d, g_RxBuff.Put: %d \n", g_RxBuff.Get, g_RxBuff.Put);
    if( (Remain_Len <= UART_PS_MAX_FIFO_LEVEL) && (rcv_flow_hold == FALSE) )
    {
        atcmd_EnableRecvIrq(FALSE);
        rcv_flow_hold = TRUE;
    }
    INT16 Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    if (Block_Len > UART_PS_RX_BUFF_LEN / 4)
    {
        COS_EVENT event = { 0 };
        STOP_RX_TIMER();
        event.nEventId = EV_DM_UART_MUX_RECEIVE_DATA_IND;
        if (COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == FALSE)
        {
            AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
        }
        // AT_TC(g_sw_UART, "!!!atcmd_RecvIrqHandlerPsMode RxBuff.Get:%d, g_RxBuff.Put: %d.", g_RxBuff.Get, g_RxBuff.Put);
    }
    else
    {
        if (g_UartInfo.fPsRxTimer == FALSE)
        {
            START_RX_TIMER();
        }
    }
    // Tell HAL how many bytes of the data have been processed.
    // If any of the data is remaining, HAL will not clear the interrupt,
    // and this irq handler should disable the interrupt by calling
    // hal_ApCommSetAtCmdRecvCallback(NULL). When AT application can
    // handle more data later, the application can re-enable the interrupt
    // by calling hal_ApCommSetAtCmdRecvCallback(&irqHandler)
    return len;
}

VOID atcmd_EnableRecvIrq(BOOL enable)
{
    HAL_AP_COMM_DATA_RECV_HANDLER handler;
    if (enable)
    {
        if (g_UartInfo.DataSpeedMode == ds_low)
        {
            handler = &atcmd_RecvIrqHandler;
        }
        else
        {
            handler = &atcmd_RecvIrqHandlerPsMode;
        }
    }
    else
    {
        handler = NULL;
    }
    hal_ApCommSetAtCmdRecvCallback(handler);
}

#else // !CHIP_IS_MODEM
#if 0
void uart_ReceiveData_from_Hal()
{
    UINT16 Block_Len, Remain_Len, real_Len, First_half, Second_half, uDataSize;
    uDataSize = hal_uart_GetData(g_UartInfo.Scom_UartId, g_RxBuff.FifoBuf, UART_MAX_FIFO_LEVEL);
    AT_TC(g_sw_UART, "In uart_ReceiveData_from_Hal  uDataSize = %d \n", uDataSize);
    Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    if ((Block_Len == 0) && (g_RxBuff.NotEmpty))
    {
        Block_Len = UART_PS_RX_BUFF_LEN;
    }
    Remain_Len = UART_PS_RX_BUFF_LEN - Block_Len;
    real_Len   = (uDataSize < Remain_Len) ? uDataSize : Remain_Len;
    if (real_Len != 0)  /* circular buffer not full */
    {
        if (g_RxBuff.Put < g_RxBuff.Get)
        {
            memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], g_RxBuff.FifoBuf, real_Len);
            g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + real_Len);
        }
        else
        {
            First_half = UART_PS_RX_BUFF_LEN - g_RxBuff.Put;
            if (real_Len < First_half)
            {
                memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], g_RxBuff.FifoBuf, real_Len);
                g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + real_Len);
            }
            else
            {
                memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], g_RxBuff.FifoBuf, First_half);
                Second_half  = real_Len - First_half;
                g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + First_half);
                memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], &g_RxBuff.FifoBuf[First_half], Second_half);
                g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + Second_half);
            }
        }
        g_RxBuff.NotEmpty = 1;
    }
    else
    {
        AT_TC(g_sw_UART, "RX Ring buffer full!!!!!\n");
    }
    AT_TC(g_sw_UART, "In uart_ReceiveData_from_Hal g_RxBuff.Get: %d, g_RxBuff.Put: %d \n", g_RxBuff.Get, g_RxBuff.Put);
}
#endif
INT16 buf_Block_Len = 0;
UINT8 gRxDMABufEmpty[UART_PS_MAX_FIFO_LEVEL] = {0x00,};

UINT16 DealWith_UartRxIrqDma(UINT8 id,BOOL  DMA_used, BOOL  Timeout)
{
    UINT32 uDataSize, Remain_Len=0;
    UINT8 *rxDmaBufEmpty, *data;
    rxDmaBufEmpty =  gRxDMABufEmpty ;
    if( rxDmaBufEmpty == NULL )
        return 0;
    if(DMA_used)
    {
        uDataSize = UART_PS_MAX_FIFO_LEVEL;
        if(Timeout)
        {
            hal_UartRxDmaPurgeFifo(id);
            while(!hal_UartRxDmaFifoEmpty(id));
            uDataSize = UART_PS_MAX_FIFO_LEVEL-hal_UartRxDmaLevel(id);
            while(!hal_UartRxDmaStop(id));
        }
        data = rxDmaBufEmpty;
        Remain_Len = put_data_to_CircularBufferDma(
                         (VOID*)HAL_SYS_GET_UNCACHED_ADDR1((UINT32)data),
                         uDataSize);
        SUL_MemSet8(rxDmaBufEmpty,0x00,UART_PS_MAX_FIFO_LEVEL);
        hal_uart_GetData(id, rxDmaBufEmpty, UART_PS_MAX_FIFO_LEVEL);
    }
    return Remain_Len;
}

#ifdef __2UART_SHARE_AT__
void uart_id_ReceiveData_from_Hal(HAL_UART_ID_T id)
{
    UINT16 Block_Len, Remain_Len, real_Len, First_half, Second_half, uDataSize;
    uDataSize = hal_uart_GetData(id, g_RxBuff.FifoBuf, UART_MAX_FIFO_LEVEL);
    Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    if ((Block_Len == 0) && (g_RxBuff.NotEmpty))
    {
        Block_Len = UART_PS_RX_BUFF_LEN;
    }
    Remain_Len = UART_PS_RX_BUFF_LEN - Block_Len;
    real_Len   = (uDataSize < Remain_Len) ? uDataSize : Remain_Len;
    if (real_Len != 0)  /* circular buffer not full */
    {
        if (g_RxBuff.Put < g_RxBuff.Get)
        {
            memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], g_RxBuff.FifoBuf, real_Len);
            g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + real_Len);
        }
        else
        {
            First_half = UART_PS_RX_BUFF_LEN - g_RxBuff.Put;
            if (real_Len < First_half)
            {
                memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], g_RxBuff.FifoBuf, real_Len);
                g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + real_Len);
            }
            else
            {
                memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], g_RxBuff.FifoBuf, First_half);
                Second_half  = real_Len - First_half;
                g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + First_half);
                memcpy(&g_RxBuff.CircuBuf[g_RxBuff.Put], &g_RxBuff.FifoBuf[First_half], Second_half);
                g_RxBuff.Put = MOD_BUFF_LEN(g_RxBuff.Put + Second_half);
            }
        }
        g_RxBuff.NotEmpty = 1;
    }
    else
    {
        AT_TC(g_sw_UART, "RX Ring buffer full!!!!!\n");
    }
}

static VOID irq_Uart1PsMode(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    irq_status.sui = 0x00;
    irq_status.st  = status;
    if (status.rxDataAvailable || status.rxTimeout)
    {
        INT16 Block_Len;
        uart_id_ReceiveData_from_Hal(HAL_UART_1);
        Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
        if ((Block_Len == 0) && (g_RxBuff.NotEmpty))
        {
            Block_Len = UART_PS_RX_BUFF_LEN;
        }
        if (Block_Len > UART_PS_RX_BUFF_LEN / 2)
        {
            COS_EVENT event = { 0 };
            STOP_RX_TIMER();
            event.nEventId = EV_DM_UART_MUX_RECEIVE_DATA_IND;
            if (rcv_flow_hold == FALSE)
            {
                hal_UartSetRts(HAL_UART_1, FALSE);
                rcv_flow_hold = TRUE;
            }
            if (COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == FALSE)
            {
            }
            AT_TC(g_sw_UART, "!!!irq_PsModeg_RxBuff.Get:%d, g_RxBuff.Put: %d.", g_RxBuff.Get, g_RxBuff.Put);
        }
        else
        {
            if (g_UartInfo.fPsRxTimer == FALSE)
                START_RX_TIMER();
        }
    }
    if(status.rxDmaDone)
    {
        DealWith_UartRxIrqDma(HAL_UART_1,TRUE,FALSE);
    }
    else if(status.rxDmaTimeout)
    {
        DealWith_UartRxIrqDma(HAL_UART_1,TRUE,TRUE);
    }
    INT16 Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    if ((Block_Len == 0) && (g_RxBuff.NotEmpty))
    {
        Block_Len = UART_PS_RX_BUFF_LEN;
    }
    if (Block_Len > UART_PS_RX_BUFF_LEN / 2)
    {
        COS_EVENT event = { 0 };
        STOP_RX_TIMER();
        event.nEventId = EV_DM_UART_MUX_RECEIVE_DATA_IND;
        if (rcv_flow_hold == FALSE)
        {
            hal_UartSetRts((HAL_UART_ID_T) HAL_UART_1, FALSE);
            rcv_flow_hold = TRUE;
        }
        if (COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == FALSE)
        {
            AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
        }
    }
    else
    {
        if (g_UartInfo.fPsRxTimer == FALSE)
            START_RX_TIMER();
    }
    if (status.txDmaDone)
    {
        AT_TC(g_sw_UART, "!!! receive status.txDmaDone.");
    }
    hal_uart_ClearErrorStatus(HAL_UART_1);
}

static VOID irq_Uart2PsMode(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    irq_status.sui = 0x00;
    irq_status.st  = status;
    if (status.rxDataAvailable || status.rxTimeout)
    {
        INT16 Block_Len;
        uart_id_ReceiveData_from_Hal(HAL_UART_2);
        Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
        if ((Block_Len == 0) && (g_RxBuff.NotEmpty))
        {
            Block_Len = UART_PS_RX_BUFF_LEN;
        }
        if (Block_Len > UART_PS_RX_BUFF_LEN / 2)
        {
            COS_EVENT event = { 0 };
            STOP_RX_TIMER();
            event.nEventId = EV_DM_UART_MUX_RECEIVE_DATA_IND;
            if (rcv_flow_hold == FALSE)
            {
                hal_UartSetRts(HAL_UART_2, FALSE);  // by wulc
                rcv_flow_hold = TRUE;
            }
            if (COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == FALSE)
            {
                AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
            }
            AT_TC(g_sw_UART, "!!!irq_PsModeg_RxBuff.Get:%d, g_RxBuff.Put: %d.", g_RxBuff.Get, g_RxBuff.Put);
        }
        else
        {
            if (g_UartInfo.fPsRxTimer == FALSE)
                START_RX_TIMER();
        }
    }
    if(status.rxDmaDone)
    {
        DealWith_UartRxIrqDma(HAL_UART_2,TRUE,FALSE);
    }
    else if(status.rxDmaTimeout)
    {
        DealWith_UartRxIrqDma(HAL_UART_2,TRUE,TRUE);
    }
    INT16 Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    if ((Block_Len == 0) && (g_RxBuff.NotEmpty))
    {
        Block_Len = UART_PS_RX_BUFF_LEN;
    }
    if (Block_Len > UART_PS_RX_BUFF_LEN / 2)
    {
        COS_EVENT event = { 0 };
        STOP_RX_TIMER();
        event.nEventId = EV_DM_UART_RECEIVE_DATA_IND;
        if (rcv_flow_hold == FALSE)
        {
            hal_UartSetRts((HAL_UART_ID_T) HAL_UART_2, FALSE);
            rcv_flow_hold = TRUE;
        }
        if (COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == FALSE)
        {
            AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
        }
    }
    else
    {
        if (g_UartInfo.fPsRxTimer == FALSE)
            START_RX_TIMER();
    }
    if (status.txDmaDone)
    {
        AT_TC(g_sw_UART, "!!! receive status.txDmaDone.");
    }
    hal_uart_ClearErrorStatus(HAL_UART_2);
}
#else // !__2UART_SHARE_AT__

static VOID irq_PsMode(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error)
{
    irq_status.sui = 0x00;
    irq_status.st  = status;
    UINT16 Remain_Len=0;
#if 0
    if (rcv_flow_hold == FALSE)
    {
        hal_UartSetRts((HAL_UART_ID_T) g_UartInfo.Scom_UartId, FALSE);
        rcv_flow_hold = TRUE;
    }
#endif
    if(status.rxDmaDone)
    {
        Remain_Len=DealWith_UartRxIrqDma(g_UartInfo.Scom_UartId,TRUE,FALSE);
    }
    else if(status.rxDmaTimeout)
    {
        Remain_Len=DealWith_UartRxIrqDma(g_UartInfo.Scom_UartId,TRUE,TRUE);
    }
    if( (Remain_Len <= UART_PS_MAX_FIFO_LEVEL) && (rcv_flow_hold == FALSE) )
    {
        hal_UartSetRts((HAL_UART_ID_T) g_UartInfo.Scom_UartId, FALSE);
        rcv_flow_hold = TRUE;
    }
    INT16 Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    if (Block_Len > UART_PS_RX_BUFF_LEN / 2)
    {
        COS_EVENT event = { 0 };
        STOP_RX_TIMER();
        event.nEventId = EV_DM_UART_MUX_RECEIVE_DATA_IND;
        if (COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) == FALSE)
        {
            AT_TC(g_sw_UART, "Call COS_SendEvent() failed, last error=0x%08x.", COS_GetLastError());
        }
        //AT_TC(g_sw_UART, "!!!irq_PsModeg_RxBuff.Get:%d, g_RxBuff.Put: %d.", g_RxBuff.Get, g_RxBuff.Put);
    }
    else
    {
        if (g_UartInfo.fPsRxTimer == FALSE)
        {
            START_RX_TIMER();
        }
    }
    if (status.txDmaDone)
    {
        AT_TC(g_sw_UART, "!!! receive status.txDmaDone.");
    }
    hal_uart_ClearErrorStatus(g_UartInfo.Scom_UartId);
}

#endif // !__2UART_SHARE_AT__

#endif // !CHIP_IS_MODEM

/*----------------------------------------------------------------------------------------------------------------------
  EXTERNAL API SECTION
  ----------------------------------------------------------------------------------------------------------------------*/

UINT16 uart_PsReadRxData(UINT8* pBuff, UINT16 uBuffSize)
{
    /*********/
    UINT16 Block_Len, First_half, Second_half;
    UINT16 real_len = 0;
    UINT32 scStatus = hal_SysEnterCriticalSection();
    Block_Len = GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    /*
       if ((Block_Len == 0) && (g_RxBuff.NotEmpty))
       {
       Block_Len = UART_PS_RX_BUFF_LEN;
       }
     */
    //read buffer size
    if (pBuff == NULL)
    {
        hal_SysExitCriticalSection(scStatus);
        return Block_Len;
    }
    STOP_RX_TIMER();
    real_len = (uBuffSize <= Block_Len) ? uBuffSize : Block_Len;
    if (real_len > 0) /* have data to send */
    {
        if (g_RxBuff.Get < g_RxBuff.Put)
        {
            memcpy(pBuff, &g_RxBuff.CircuBuf[g_RxBuff.Get], real_len);
            g_RxBuff.Get = MOD_BUFF_LEN(g_RxBuff.Get + real_len);
        }
        else
        {
            First_half = UART_PS_RX_BUFF_LEN - g_RxBuff.Get;
            if (real_len < First_half)
            {
                memcpy(pBuff, &g_RxBuff.CircuBuf[g_RxBuff.Get], real_len);
                g_RxBuff.Get = MOD_BUFF_LEN(g_RxBuff.Get + real_len);
            }
            else
            {
                memcpy(pBuff, &g_RxBuff.CircuBuf[g_RxBuff.Get], First_half);
                Second_half  = real_len - First_half;
                g_RxBuff.Get = MOD_BUFF_LEN(g_RxBuff.Get + First_half);
                memcpy(&pBuff[First_half], &g_RxBuff.CircuBuf[g_RxBuff.Get], Second_half);
                g_RxBuff.Get = MOD_BUFF_LEN(g_RxBuff.Get + Second_half);
            }
        }
        if (g_RxBuff.Get == g_RxBuff.Put)
        {
            //hal_HstSendEvent(0x09300000);
            g_RxBuff.NotEmpty = 0;
            memset(g_RxBuff.CircuBuf,0x00,UART_PS_RX_BUFF_LEN);
        }
    }
    else if (!Block_Len)
    {
        hal_HstSendEvent(0x09300001);
        g_RxBuff.NotEmpty = 0;
        memset(g_RxBuff.CircuBuf,0x00,UART_PS_RX_BUFF_LEN);
    }
    Block_Len=GET_DATA_BLOCK_LEN(g_RxBuff.Get, g_RxBuff.Put, UART_PS_RX_BUFF_LEN);
    if ((Block_Len<UART_PS_MAX_FIFO_LEVEL) && (rcv_flow_hold == TRUE))
    {
#ifdef CHIP_IS_MODEM
        atcmd_EnableRecvIrq(TRUE);
#else
        hal_UartSetRts((HAL_UART_ID_T) g_UartInfo.Scom_UartId, TRUE);
#endif
        rcv_flow_hold = FALSE;
    }
    if (Block_Len != 0)
    {
        START_RX_TIMER();
    }
    hal_SysExitCriticalSection(scStatus);
    return real_len;
}

#include "hal_gpio.h"

UINT16 uart_SendDataToTeNoSleep(UINT8* pBuf, UINT16 len)
{
    UINT16 n = len;
    UINT16 i = 0;
    UINT8* p = pBuf;
    //AT_TC(g_sw_UART, " uart_SendDataToTe %d", len);
    AT_CP_ENTRY(CPID_UART_PT);
    if (pBuf == NULL || len == 0)
    {
        AT_CP_EXIT(CPID_UART_PR);
        return 0;
    }
#ifdef CHIP_IS_MODEM
    i = 0;
    while (i < len)
    {
        p = pBuf + i;
        i += (UINT16)hal_ApCommSendAtCmd(p, len - i);
    }
    // For the return value
    n = 0;
#else // !CHIP_IS_MODEM
    if (!g_UartInfo.CommOpened)
    {
        bool bRet = FALSE;
        bRet = AT_UartOpen(g_UartInfo.Scom_UartId, ds_low);
        if (FALSE == bRet)
            return 0;
    }
    UINT32 status = hal_EnterCriticalSection();
#ifdef __2UART_SHARE_AT__
    while (!(hal_uart_TxFinished(HAL_UART_1)))
    {
        COS_Sleep(10);
    }
    while (!(hal_uart_TxFinished(HAL_UART_2)))
    {
        COS_Sleep(10);
    }
#else
    while (!(hal_uart_TxFinished(g_UartInfo.Scom_UartId)))
    {
        COS_Sleep(10);
    }
#endif
#ifdef __2UART_SHARE_AT__
    while (n > 0)
    {
        // while(hal_uart_TxDmaDone(g_UartInfo.Scom_UartId) == TRUE)
        {
            i = hal_uart_SendData(HAL_UART_1, p, n);
            n -= i;
            p += i;
            COS_Sleep(10);
        }
    }
    n = len;
    i = 0;
    p = pBuf;
    COS_Sleep(10);
    while (n > 0)
    {
        // while(hal_uart_TxDmaDone(g_UartInfo.Scom_UartId) == TRUE)
        {
            i = hal_uart_SendData(HAL_UART_2, p, n);
            n -= i;
            p += i;
            COS_Sleep(10);
        }
    }
#else
    /*
       for(i=0;i<n;i++)
       {
       p += i;
       AT_TC(g_sw_UART, " in uart_SendDataToTe  to  hal pBuf = 0x%x", *p);
       }
     */
    AT_TC(g_sw_UART, " Before hal_uart_SendData dump");
    // SXS_DUMP(_CSW, 0, pBuf, len);
    AT_TC(g_sw_UART, " After  hal_uart_SendData dump");
    while (n > 0)
    {
        // while(hal_uart_TxDmaDone(g_UartInfo.Scom_UartId) == TRUE)
        {
            i = hal_uart_SendData(g_UartInfo.Scom_UartId, p, n);
            n -= i;
            p += i;
            // COS_Sleep(10);
        }
    }
#endif
    hal_ExitCriticalSection(status);
#ifdef __MODEM_LP_MODE_ENABLE__
    AT_SetLPTimer(ATE_LP_TIME_ELAPSE);
#endif
#endif // !CHIP_IS_MODEM
    AT_CP_EXIT(CPID_UART_PR);
    return (len - n);
}
#endif /* 0 */

#ifdef __AT_GPS__
VOID GPS_uart_SendData(UINT8* pBuf, UINT16 len,HAL_UART_ID_T uart_id)
{
    UINT16 n = len;
    UINT16 i = 0;
    UINT8* p = pBuf;
    if (pBuf == NULL || len == 0)
    {
        return 0;
    }
    while(!hal_UartTxDmaDone(uart_id));
    while (n > 0)
    {
        i = hal_uart_SendData(uart_id, p, n);
        if (g_UartInfo.DataSpeedMode == ds_high)
            while (!hal_UartTxDmaDone(uart_id));
        else
            while (!hal_UartTxFinished(uart_id));
        n -= i;
        p += i;
    }
#ifdef __MODEM_LP_MODE_ENABLE__
    AT_SetLPTimer(ATE_LP_TIME_ELAPSE);
#endif
    return (len - n);
}


VOID uart_SendDataGPSCallBack(UINT8* pDataToSend, UINT16 data_len)
{
    uart_SendDataToTe(pDataToSend, data_len);
}

#endif

BOOL uart_SendDataPPPCallBack(UINT8* pData, UINT16 size, UINT8 nDLCI)
{
    uart_SendDataToTe(pData, size);
    return TRUE;
}

VOID uart_SendDataBTCallBack(UINT8* pDataToSend, UINT16 data_len)
{
    uart_SendDataToTe(pDataToSend, data_len);
}

#if defined(__AT_IN_BT_SPP__)
extern UINT16 uart_SendDataToBTSpp(UINT8* pBuf, UINT16 len);
#endif /* __AT_IN_BT_SPP__ */


UINT16 uart_SendDataToTe(UINT8* pBuf, UINT16 len)
{
    UINT16 n = len;
#if defined (CHIP_IS_MODEM) || !defined (__AT_IN_DEBUG_HOST__)
    UINT16 i = 0;
    UINT8* p = pBuf;
#endif

    AT_TC(g_sw_UART, " uart_SendDataToTe %d", len);
    AT_CP_ENTRY(CPID_UART_PT);
    if (pBuf == NULL || len == 0)
    {
        AT_CP_EXIT(CPID_UART_PT);
        return 0;
    }

#if defined(__AT_IN_BT_SPP__)
    //uart_SendDataToBTSpp(pBuf,len);
#endif /* __AT_IN_BT_SPP__ */

#ifdef CHIP_IS_MODEM

    i = 0;
    while (i < len)
    {
        p = pBuf + i;
        i += (UINT16)hal_ApCommSendAtCmd(p, len - i);
        if (i < len)
        {
            COS_Sleep(5);
            // hal_TimDelay(10)
        }
    }
    // For the return value
    n = 0;
#elif defined(__AT_IN_DEBUG_HOST__)
    uart_send_data_to_debug_host(pBuf, len);
#else // !CHIP_IS_MODEM

    if (!g_UartInfo.CommOpened)
    {
        bool bRet = FALSE;
        bRet = AT_UartOpen(g_UartInfo.Scom_UartId, ds_low);
        if (FALSE == bRet)
        {
            AT_CP_EXIT(CPID_UART_PT);
            return 0;
        }
    }
#ifdef __2UART_SHARE_AT__

    while (n > 0)
    {
        i = hal_uart_SendData(HAL_UART_1, p, n);
        if (g_UartInfo.DataSpeedMode == ds_high)
            while (!hal_UartTxFinished(HAL_UART_1) || (!hal_UartTxDmaDone(HAL_UART_1)));
        else
            while (!hal_UartTxFinished(HAL_UART_1)) ;
        n -= i;
        p += i;
    }
    n = len;
    i = 0;
    p = pBuf;
    while (n > 0)
    {
        i = hal_uart_SendData(HAL_UART_2, p, n);
        if (g_UartInfo.DataSpeedMode == ds_high)
            while (!hal_UartTxFinished(HAL_UART_2) || (!hal_UartTxDmaDone(HAL_UART_2))) ;
        else
            while (!hal_UartTxFinished(HAL_UART_2)) ;
        n -= i;
        p += i;
    }
#else

    // SXS_DUMP(_CSW, 0, pBuf, len);
    if (g_UartInfo.DataSpeedMode == ds_high)
        while (!hal_UartTxDmaDone(g_UartInfo.Scom_UartId));
    else
        while (!hal_UartTxFinished(g_UartInfo.Scom_UartId));
    while (n > 0)
    {
        i = hal_uart_SendData(g_UartInfo.Scom_UartId, p, n);
        if (g_UartInfo.DataSpeedMode == ds_high)
            while (!hal_UartTxDmaDone(g_UartInfo.Scom_UartId));
        else
            while (!hal_UartTxFinished(g_UartInfo.Scom_UartId));
        n -= i;
        p += i;
    }

#endif

#ifdef __MODEM_LP_MODE_ENABLE__
    AT_SetLPTimer(ATE_LP_TIME_ELAPSE);
#endif
#endif // !CHIP_IS_MODEM
    AT_CP_EXIT(CPID_UART_PT);
    return (len - n);
}

VOID AT_UartInit(UART_CFG* cfg)
{
    memset(&g_UartInfo, 0, sizeof(g_UartInfo));
//    memset(&g_Circular_Buf, 0, sizeof(g_Circular_Buf));
//    memset(&g_RxBuff, 0, sizeof(g_RxBuff));
    g_UartInfo.ComAtflag.FlagToCom = DATA_TO_COM;
#ifdef CHIP_IS_MODEM
    g_UartInfo.Scom_UartId         = 0;
#elif defined(__AT_IN_DEBUG_HOST__)
    g_UartInfo.Scom_UartId         = 0;
#else
    g_UartInfo.Scom_UartId         = UART_DEFAULT_UART;
#endif
    g_UartInfo.CommOpened          = FALSE;
    g_UartInfo.data_Mode           = AT_STATUS_CMD_LINE;
    g_UartInfo.CRvalue_Default     = AT_CMD_CR;
    g_UartInfo.LFvalue_Default     = AT_CMD_LF;
    g_UartInfo.ReqPackStyp         = SCM_REQ_CMD;
    g_UartInfo.PPPRecStatus        = FALSE;
    g_UartInfo.CheckDataFlag       = 0;
    g_UartInfo.Is_data             = FALSE;
    g_UartInfo.DataSpeedMode       = ds_low;
    memcpy(&g_UartCfg, cfg, sizeof(UART_CFG));
}

VOID AT_UartReset(VOID)
{
    AT_UartClose(g_UartInfo.Scom_UartId);
    AT_UartInit(&g_UartCfg);
    AT_UartOpen(g_UartInfo.Scom_UartId, g_UartInfo.DataSpeedMode);
}

BOOL AT_UartOpen(HAL_UART_ID_T id, UART_DS_MODE_T DataSpeedMode)
{
#if !defined CHIP_IS_MODEM && !defined (__AT_IN_DEBUG_HOST__)
    HAL_UART_CFG_T uartCfg;
#endif

    HAL_UART_IRQ_STATUS_T mask;

    memset(&mask, 0x0, sizeof(HAL_UART_IRQ_STATUS_T));

#ifdef CODEC_SUPPORT
    DataSpeedMode = ds_high;
#endif

    if (g_UartInfo.CommOpened == TRUE)
        return TRUE;
    g_UartInfo.CommOpened = TRUE;
#ifdef CHIP_IS_MODEM
    g_UartInfo.DataSpeedMode = DataSpeedMode;
    g_RxBuff.Buf_len = UART_PS_RX_BUFF_LEN;
    g_RxBuff.Get = 0;
    g_RxBuff.Put = 0;
    g_RxBuff.NotEmpty = 0;
    uart_ClearBuffer();
    hal_ApCommClearAtCmdRecvBuf();
    atcmd_EnableRecvIrq(TRUE);
#elif defined(__AT_IN_DEBUG_HOST__)

#if 1 //20170112
	g_atInDbgHstAccess.rxBufPtr = g_UartInfo.rx_buff;
	g_atInDbgHstAccess.rxBufLen = AT_CMD_LINE_BUFF_LEN;

#endif
    memset(g_atInDbgHstAccess.rxBufPtr, 0x0, g_atInDbgHstAccess.rxBufLen);
    //memset(g_atInDbgHstAccess.txBufPtr, 0x0, g_atInDbgHstAccess.txBufLen);
    g_atInDbgHstAccess.state = AT_DBG_HST_PC_WRITE_ENABLE | AT_DBG_HST_HST_WRITE_ENABLE;
#else // !CHIP_IS_MODEM
    AT_ASSERT(id == UART_1 || id == UART_2);

    // UART_BAUD_RATE_115200, UART_8_DATA_BITS, UART_1_STOP_BIT, UART_NO_PARITY, UART_AFC_MODE_RX_FULL
    // uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_8;//HAL_UART_AFC_MODE_DISABLE;
#if  __FORCE_ENABLE_UART_FLOWCONTROL__
    uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_15; // HAL_UART_AFC_MODE_DISABLE;
#else
    uartCfg.afc = HAL_UART_AFC_MODE_DISABLE;  // HAL_UART_AFC_MODE_DISABLE;
#endif
    // uartCfg.afc = HAL_UART_AFC_MODE_DISABLE;
    uartCfg.data   = HAL_UART_8_DATA_BITS;
    uartCfg.irda   = HAL_UART_IRDA_MODE_DISABLE;
    uartCfg.parity = HAL_UART_NO_PARITY;
#ifdef ADAPTIVE_BAUD_RATE
    uartCfg.rate = g_UART_Current_Baudrate[g_uart_baudrate_index];  // HAL_UART_BAUD_RATE_9600;
#else
#ifdef  __AT_GPS__
    uartCfg.rate = HAL_UART_BAUD_RATE_9600;
#else
#ifndef CODEC_SUPPORT
	uartCfg.rate = HAL_UART_BAUD_RATE_115200;
#else
    //uartCfg.rate = HAL_UART_BAUD_RATE_921600;
    uartCfg.rate = HAL_UART_BAUD_RATE_3250000;
#endif
#endif
#endif
    uartCfg.rx_trigger =  HAL_UART_RX_TRIG_QUARTER;
    uartCfg.stop       = HAL_UART_1_STOP_BIT;

    if (DataSpeedMode == ds_low)
    {
        uartCfg.rx_mode = HAL_UART_TRANSFERT_MODE_DIRECT_IRQ;
        uartCfg.tx_mode = HAL_UART_TRANSFERT_MODE_DIRECT_IRQ;
        mask.rxDataAvailable = 1;
        mask.rxTimeout    = 1;
    }
    else
    {
        uartCfg.tx_mode = HAL_UART_TRANSFERT_MODE_DMA_IRQ;
        uartCfg.rx_mode = HAL_UART_TRANSFERT_MODE_DMA_IRQ;
        mask.txDmaDone  = 0;
        mask.rxDmaTimeout = 1;
        mask.rxDmaDone = 1;
        mask.rxDataAvailable = 0;
        mask.rxTimeout = 0;
    }
    uartCfg.tx_trigger = HAL_UART_TX_TRIG_EMPTY;
    uart_ClearBuffer();
#ifdef __2UART_SHARE_AT__
    hal_UartOpen(HAL_UART_1, &uartCfg);
    hal_uart_PurgeFifo(HAL_UART_1);
    hal_UartSetRts(HAL_UART_1, TRUE);
    // COS_Sleep(100);
    uartCfg.rate = HAL_UART_BAUD_RATE_115200;
    hal_UartOpen(HAL_UART_2, &uartCfg);
    hal_uart_PurgeFifo(HAL_UART_2);
    hal_UartSetRts(HAL_UART_2, TRUE);
#else
    hal_UartOpen((HAL_UART_ID_T) id, &uartCfg);
    hal_uart_PurgeFifo((HAL_UART_ID_T) id);
    hal_UartSetRts((HAL_UART_ID_T) id, TRUE);
#endif
    if (DataSpeedMode == ds_low)
    {
        switch (id)
        {
            case UART_1:
            case UART_2:
#ifdef __2UART_SHARE_AT__
                hal_UartClearErrorStatus(HAL_UART_1);
                hal_UartIrqSetMask(HAL_UART_1, mask);
                hal_UartIrqSetHandler(HAL_UART_1, irq_Uart1Processor);
                hal_UartClearErrorStatus(HAL_UART_2);
                hal_UartIrqSetMask(HAL_UART_2, mask);
                hal_UartIrqSetHandler(HAL_UART_2, irq_Uart2Processor);
#else
                hal_UartClearErrorStatus((HAL_UART_ID_T) id);
                hal_UartIrqSetMask((HAL_UART_ID_T) id, mask);
                hal_UartIrqSetHandler((HAL_UART_ID_T) id, irq_processor);
#endif
                break;
            default:
                break;
        }
    }
    else
    {
        switch (id)
        {
            case UART_1:
            case UART_2:
#ifdef __2UART_SHARE_AT__
                hal_UartClearErrorStatus(HAL_UART_1);
                hal_UartIrqSetMask(HAL_UART_1, mask);
                // hal_UartIrqSetHandler(HAL_UART_1, irq_Uart1PsMode);
                hal_UartIrqSetHandler(HAL_UART_1, irq_Uart1PsMode);
                hal_UartClearErrorStatus(HAL_UART_2);
                hal_UartIrqSetMask(HAL_UART_2, mask);
                hal_UartIrqSetHandler(HAL_UART_2, irq_Uart2PsMode);
                if(uartCfg.rx_mode == HAL_UART_TRANSFERT_MODE_DMA_IRQ)
                {
                    UINT32 scStatus = hal_SysEnterCriticalSection();
                    hal_uart_GetData(g_UartInfo.Scom_UartId, gRxDMABufEmpty, UART_PS_MAX_FIFO_LEVEL);
                    hal_SysExitCriticalSection(scStatus);
                }
#else
                hal_UartClearErrorStatus((HAL_UART_ID_T) id);
                hal_UartIrqSetMask((HAL_UART_ID_T) id, mask);
                hal_UartIrqSetHandler((HAL_UART_ID_T) id, irq_processor);
                if(uartCfg.rx_mode == HAL_UART_TRANSFERT_MODE_DMA_IRQ)
                {
                    UINT32 scStatus = hal_SysEnterCriticalSection();
#ifdef UART_USE_SWAP_BUF
                    hal_uart_GetData(g_UartInfo.Scom_UartId, uart_rx_buf, AT_CMD_LINE_BUFF_LEN);
#else
                    hal_uart_GetData(g_UartInfo.Scom_UartId, g_UartInfo.rx_buff, AT_CMD_LINE_BUFF_LEN);
#endif
                    hal_SysExitCriticalSection(scStatus);
                }
#endif
                break;
            default:
                break;
        }
    }
    AT_TC(g_sw_UART, "Open uart id=%d", id);
    g_UartInfo.Scom_UartId = id;
    g_UartInfo.DataSpeedMode = DataSpeedMode;
#ifdef __MODEM_LP_MODE_ENABLE__
    csw_SetResourceActivity(CSW_LP_RESOURCE_UNUSED_1, CSW_SYS_FREQ_208M);
#endif
#endif // !CHIP_IS_MODEM

#ifdef CODEC_SUPPORT
    csw_SetResourceActivity(CSW_LP_RESOURCE_UNUSED_1, CSW_SYS_FREQ_104M);
#endif

    return TRUE;
}

BOOL AT_UartReopen(UART_DS_MODE_T DataSpeedMode)
{
    if (AT_UartClose(g_UartInfo.Scom_UartId) == TRUE)
        return AT_UartOpen(g_UartInfo.Scom_UartId, DataSpeedMode);
    else
    {
        AT_TC(g_sw_UART, "close UART%d failed!", g_UartInfo.Scom_UartId);
        return FALSE;
    }
}

BOOL AT_UartClose(UART_ID id)
{
    if (g_UartInfo.CommOpened == FALSE)
        return TRUE;
    g_UartInfo.CommOpened = FALSE;
#ifdef CHIP_IS_MODEM
    atcmd_EnableRecvIrq(FALSE);
#elif defined(__AT_IN_DEBUG_HOST__)
    g_atInDbgHstAccess.state = AT_DBG_HST_INVAILD;
#else // !CHIP_IS_MODEM
    // AT_ASSERT(id == g_UartInfo.Scom_UartId);
    //  AT_TC_ENTER_FUNC(g_sw_UART);
#ifdef __2UART_SHARE_AT__
    if (!(hal_uart_TxFinished(HAL_UART_1)))
    {
        COS_Sleep(1600);
        // return FALSE;
    }
    if (!(hal_uart_TxFinished(HAL_UART_2)))
    {
        COS_Sleep(1600);
        // return FALSE;
    }
    hal_uart_PurgeFifo(HAL_UART_1);
    hal_uart_PurgeFifo(HAL_UART_2);
    hal_uart_Close(HAL_UART_1);
    hal_uart_Close(HAL_UART_2);
#else
    if (!(hal_uart_TxFinished(g_UartInfo.Scom_UartId)))
    {
        COS_Sleep(1600);
        // return FALSE;
    }
    hal_uart_PurgeFifo((HAL_UART_ID_T) g_UartInfo.Scom_UartId);
    hal_uart_Close((HAL_UART_ID_T) g_UartInfo.Scom_UartId);
#endif
    g_UartInfo.DataSpeedMode = ds_low;
#ifdef __MODEM_LP_MODE_ENABLE__
    csw_SetResourceActivity(CSW_LP_RESOURCE_UNUSED_1, CSW_SYS_FREQ_32K);
#endif
#endif // !CHIP_IS_MODEM
    return TRUE;
}

VOID uart_ClearBuffer(VOID)
{
    memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
    g_UartInfo.rx_buff_p = 0;
}

#if defined(__AT_IN_DEBUG_HOST__)

void uart_check_uart_command(void)
{
	if(0 != (g_atInDbgHstAccess.state & AT_DBG_HST_HST_READ_ENABLE))
	{
		UINT16 recv_len;
		UINT8 mode = COMMAND_ECHO_MODE_DEFUALT_VAL;

		//Clear flag
		g_atInDbgHstAccess.state &= ~AT_DBG_HST_HST_READ_ENABLE;
		
		recv_len = strlen(g_atInDbgHstAccess.rxBufPtr);

	    //Echo to terminal
		if(mode == 1)
		{
			uart_send_data_to_debug_host(g_atInDbgHstAccess.rxBufPtr, recv_len);
		}
#if 0 //20170112
        /*
           if buff is full then
           clear buffer
           end
         * */
        if (strlen(g_UartInfo.rx_buff) + recv_len >= AT_CMD_LINE_BUFF_LEN)
        {
            memset(g_UartInfo.rx_buff, 0, AT_CMD_LINE_BUFF_LEN);
            g_UartInfo.rx_buff_p = 0;
        }
        /*
           copy rcvbuff to rxbuff
         * */
        memcpy(g_UartInfo.rx_buff + g_UartInfo.rx_buff_p, g_atInDbgHstAccess.rxBufPtr, recv_len);
        g_UartInfo.rx_buff_p += recv_len;
        g_UartInfo.rx_buff[g_UartInfo.rx_buff_p] = 0;
#endif

        if (recv_len > 0)
        {
            ProcessLineEditingChar(g_UartInfo.rx_buff);
            if (g_UartInfo.data_Mode == AT_STATUS_CMD_LINE)
            {
                g_UartInfo.rx_buff_p = strlen(g_UartInfo.rx_buff);
            }

            data_check_proc();  // Check whether buffer can be sent to ATE task.
        }


        memset(g_atInDbgHstAccess.rxBufPtr, 0x0, g_atInDbgHstAccess.rxBufLen);

		//Enable pc write
		g_atInDbgHstAccess.state |= AT_DBG_HST_PC_WRITE_ENABLE;
	}
}

#if (1)

void uart_send_data_to_debug_host(const UINT8* str, const UINT16 len)
{	
	UINT16 i = 0;
	UINT32 temp;

#define CHAR_TO_UINT32(i)	temp |= (i < len) ? str[i] : 0;\
							temp = (temp << 8);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    rdabt_wait_btcpu_busy();
#endif

    hal_HstSendEvent(0xff,0x21ace000 + (len & 0xfff));

    while(i < len)
    {
    	temp = 0;
    	CHAR_TO_UINT32(i+3);
    	CHAR_TO_UINT32(i+2);
    	CHAR_TO_UINT32(i+1);
    	temp |= str[i];
 
        i+= 4;
	    hal_HstSendEvent(0xff,temp);
    }

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    rdabt_clear_xcpu_busy();
#endif

}
#else

void uart_send_data_to_debug_host(const UINT8* str, const UINT16 len)
{
	if(0 != (g_atInDbgHstAccess.state & AT_DBG_HST_HST_WRITE_ENABLE))
	{
		UINT16 write_len;

		//Clear flag
		g_atInDbgHstAccess.state &= ~AT_DBG_HST_HST_WRITE_ENABLE;

		//Write buffer
		write_len = MIN(len,(g_atInDbgHstAccess.txBufLen - 1));
	    memcpy(g_atInDbgHstAccess.txBufPtr, str, write_len);
		g_atInDbgHstAccess.txBufPtr[write_len] = '\0';

		//Enable pc read
		g_atInDbgHstAccess.state |= AT_DBG_HST_PC_READ_ENABLE;

		//Notify tool read
	    hal_HstSendEvent(BOOT_EVENT,0x21ace000);

	}
	else
	{
        AT_TC(g_sw_UART, "uart_send_data_to_debug_host %s,%d failed!", str, len);
	}

}
#endif /* 0 */



#endif /* __AT_IN_DEBUG_HOST__ */

UART_BAUDRATE uart_GetCfgBaudrate(void)
{
    return g_UartCfg.br;
}

BOOL uart_SetCfgBaudrate(UART_BAUDRATE br)
{
    // ////////////AT_ASSERT(br >= UART_BAUD_RATE_2400 && br <= UART_BAUD_RATE_1843200);
    g_UartCfg.br = br;
    return TRUE;
}

UART_PARITY uart_GetCfgParity(void)
{
    return g_UartCfg.pr;
}

UINT8 uart_GetCfgFormat(void)
{
    if ((g_UartCfg.db == UART_8_DATA_BITS) && (g_UartCfg.sb == UART_2_STOP_BITS))
        return 1;
    else if ((g_UartCfg.db == UART_8_DATA_BITS) && (g_UartCfg.sb == UART_1_STOP_BIT)
             && ((g_UartCfg.pr == UART_ODD_PARITY) || (g_UartCfg.pr == UART_EVEN_PARITY)
                 || (g_UartCfg.pr == UART_SPACE_PARITY) || (g_UartCfg.pr == UART_MARK_PARITY)))
        return 2;
    else if ((g_UartCfg.db == UART_8_DATA_BITS) && (g_UartCfg.sb == UART_1_STOP_BIT))
        return 3;
    else if ((g_UartCfg.db == UART_7_DATA_BITS) && (g_UartCfg.sb == UART_2_STOP_BITS))
        return 4;
    else if ((g_UartCfg.db == UART_7_DATA_BITS) && (g_UartCfg.sb == UART_1_STOP_BIT)
             && ((g_UartCfg.pr == UART_ODD_PARITY) || (g_UartCfg.pr == UART_EVEN_PARITY)
                 || (g_UartCfg.pr == UART_SPACE_PARITY) || (g_UartCfg.pr == UART_MARK_PARITY)))
        return 5;
    else if ((g_UartCfg.db == UART_7_DATA_BITS) && (g_UartCfg.sb == UART_1_STOP_BIT))
        return 6;
    else
    {
        return 0;
    }
}

BOOL uart_SetCfgFormat(UINT8 Format)
{
    switch (Format)
    {
        case 1:
            g_UartCfg.db = UART_8_DATA_BITS;
            g_UartCfg.sb = UART_2_STOP_BITS;
            break;
        case 2:
            g_UartCfg.db = UART_8_DATA_BITS;
            g_UartCfg.sb = UART_1_STOP_BIT;
            g_UartCfg.pr = !UART_NO_PARITY; // TODO ......
            break;
        case 3:
            g_UartCfg.db = UART_8_DATA_BITS;
            g_UartCfg.sb = UART_1_STOP_BIT;
            g_UartCfg.pr = UART_NO_PARITY;
            break;
        case 4:
            g_UartCfg.db = UART_7_DATA_BITS;
            g_UartCfg.sb = UART_2_STOP_BITS;
            break;
        case 5:
            g_UartCfg.db = UART_7_DATA_BITS;
            g_UartCfg.sb = UART_1_STOP_BIT;
            g_UartCfg.pr = !UART_NO_PARITY;
            break;
        case 6:
            g_UartCfg.db = UART_7_DATA_BITS;
            g_UartCfg.sb = UART_1_STOP_BIT;
            g_UartCfg.pr = UART_NO_PARITY;
            break;
        default:
            g_UartCfg.db = UART_8_DATA_BITS;
            g_UartCfg.sb = UART_1_STOP_BIT;
            g_UartCfg.pr = UART_NO_PARITY;
            break;
    }
    return TRUE;
}

BOOL uart_SetParity(UINT8 Parity)
{
    g_UartCfg.pr = Parity;
    return TRUE;
}

BOOL uart_SetAfcMode(UINT8 AfcMode)
{
    g_UartCfg.fc = AfcMode;
    return TRUE;
}
#if 0
// Add by wulc end
// TODO: using timer polling mechanism instead.
UINT16 uart_PsSendData(UINT8* pData, UINT16 uDataSize)
{
    UINT16 Block_Len, Remain_Len, real_Len, First_half, Second_half;
#ifdef __MODEM_LP_MODE_ENABLE__
    AT_SetLPTimer(ATE_LP_TIME_ELAPSE);
#endif
    UINT32 scStatus = COS_EnterCriticalSection();
    Block_Len = GET_DATA_BLOCK_LEN(g_Circular_Buf.Get, g_Circular_Buf.Put, UART_PS_RX_BUFF_LEN);
    if ((Block_Len == 0) && (g_Circular_Buf.NotEmpty))
    {
        Block_Len = UART_PS_RX_BUFF_LEN;
    }
    Remain_Len = UART_PS_RX_BUFF_LEN - Block_Len;
    real_Len = (uDataSize < Remain_Len) ? uDataSize : Remain_Len;
    if (real_Len != 0)  // circular buffer not full
    {
        if (g_Circular_Buf.Put < g_Circular_Buf.Get)
        {
            memcpy(&g_Circular_Buf.TX_CircularBuf[g_Circular_Buf.Get], pData, real_Len);
            g_Circular_Buf.Put = MOD_BUFF_LEN(g_Circular_Buf.Put + real_Len);
        }
        else
        {
            First_half = UART_PS_RX_BUFF_LEN - g_Circular_Buf.Put;
            if (real_Len < First_half)
            {
                memcpy(&g_Circular_Buf.TX_CircularBuf[g_Circular_Buf.Put], pData, real_Len);
                g_Circular_Buf.Put = MOD_BUFF_LEN(g_Circular_Buf.Put + real_Len);
            }
            else
            {
                memcpy(&g_Circular_Buf.TX_CircularBuf[g_Circular_Buf.Put], pData, First_half);
                Second_half        = real_Len - First_half;
                g_Circular_Buf.Put = MOD_BUFF_LEN(g_Circular_Buf.Put + First_half);
                memcpy(&g_Circular_Buf.TX_CircularBuf[g_Circular_Buf.Put], pData + First_half, Second_half);
                g_Circular_Buf.Put = MOD_BUFF_LEN(g_Circular_Buf.Put + Second_half);
            }
        }
        g_Circular_Buf.NotEmpty = 1;
        uart_DmaSendData_to_Hal(1);
    }
    else
    {
        AT_TC(g_sw_UART, "TX Ring buffer full%d,%d,%d!!!!!\n", g_Circular_Buf.Get, g_Circular_Buf.Put, g_Circular_Buf.NotEmpty);
        uart_DmaSendData_to_Hal(1);
    }
    COS_ExitCriticalSection(scStatus);
    return (real_Len);
}
#endif

UINT8 *ATMemstr(UINT8 *haystack, UINT8 *needle, UINT32 nHayStackSize,UINT32 nNeedleSize)
{
    UINT8 *p;
    UINT8 needlesize = nNeedleSize;
    for (p = haystack; p <= (haystack-needlesize+nHayStackSize); p++)
    {
        if (memcmp(p, needle, needlesize) == 0)
            return p; /* found */
    }
    return NULL;
}

