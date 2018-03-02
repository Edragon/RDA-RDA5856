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


#ifndef __AIF_H__
#define __AIF_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//data
typedef union
{
    U32 v;
    struct
    {
        U32 data0                          :16; /*15:0 , This reg contains data to be read or written by IFC. In mono mode, data0 is before data1. In stereo mode, data0 is in left channel.*/
        U32 data1                          :16; /*31:16, This reg contains data to be read or written by IFC. In mono mode, data1 is after data0. In stereo mode, data1 is in right channel.*/
    } b;
} T_AIF_data;

//ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Audio Interface Enable. 0: DISABLE    if AIF_Tone[0] is also 0, AIF is disabled. 1: ENABLE      AIF Enabled. If AIF_Tone[0] is also '1', Tx fifo continue to fetch and distribute data from IFC when tone is enable. However, these data are not used.*/
        U32 ctrl_reserved_3                :3 ; /*3 :1 , No description.               */
        U32 tx_off                         :1 ; /*4 :4 , Disable AIF Tx functions. Important: if you want to do record only, you must set this bit otherwise AIF state machine will not start. 0: TX_ON    Both Tx Rx enabled. 1: TX_OFF   Rx enabled only, Tx disabled.*/
        U32 ctrl_reserved_2                :3 ; /*7 :5 , No description.               */
        U32 parallel_out_set               :1 ; /*8 :8 , Selects parallel audio interface connected to analog front-end. 0: SERL serial output. 1: PARA parallel output.*/
        U32 parallel_out_clr               :1 ; /*9 :9 , Selects parallel audio interface connected to analog front-end. 0: SERL serial output. 1: PARA parallel output.*/
        U32 parallel_in_set                :1 ; /*10:10, Selects parallel audio interface connected to analog front-end. 0: SERL serial input. 1: PARA parallel input.*/
        U32 parallel_in_clr                :1 ; /*11:11, Selects parallel audio interface connected to analog front-end. 0: SERL serial input. 1: PARA parallel input.*/
        U32 tx_stb_mode                    :1 ; /*12:12, In parallel mode, select AIF Tx Strobe mode. Reserved in serial mode. 0: TX_STB_MID     edge is in middle of data. 1: TX_STB_ALIGN edge is aligned to data edge.*/
        U32 ctrl_reserved_1                :3 ; /*15:13, No description.               */
        U32 out_underflow                  :1 ; /*16:16, This bit indicates if the AIF had needed some data while the Out Fifo was empty. In case of data famine, the last available data will be sent again. Write one to clear the out_underflow status bit. This bit is auto clear.*/
        U32 in_overflow                    :1 ; /*17:17, This bit indicates if the AIF had received some data while the Input Fifo was full. If the Fifo In is full, the newly received data will be lost. Write one to clear the in_overflow status bit. This bit is auto clear.*/
        U32 ctrl_reserved_0                :13; /*30:18, No description.               */
        U32 loop_back                      :1 ; /*31:31, Sets the loop back mode. The feature is for debug only and can not work in DAI mode.0: NORMAL1: LOOPBACK*/
    } b;
} T_AIF_ctrl;

//serial_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 serial_mode                    :2 ; /*1 :0 , Configure serial AIF mode. 0: I2S_PCM1: VOICE2: DAI3: ReservedWhen mode is set DAI, the bit Master Mode should be set to '1', bit Endian_L set to '0'. Data should be sent out on falling edge, which requires either Bclk_Pol = '0' and Half_Cycle_DLY = '1' or Bclk_Pol = '1' and Half_Cycle_DLY = '0'. Bits Tx_DLY and BCKOut_Gate must be configured to '0' and '1'. The DAI mode must NOT be modified after AIF is enabled. */
        U32 i2s_in_sel                     :2 ; /*3 :2 , Select AIF I2S input.0: I2S_IN_01: I2S_IN_12: I2S_IN_2*/
        U32 master_mode                    :1 ; /*4 :4 , configure AIF works in master mode (LRCLK and BCK timing signals are generated internally) or slave mode (LRCLK and BCK timing signals are generated externally).0: SLAVE1: MASTER*/
        U32 lsb                            :1 ; /*5 :5 , When high, the output data format is with the least significant bit first.0: MSB1: LSB*/
        U32 lrck_pol                       :1 ; /*6 :6 , configure LRCK polarity. 0: LEFT_H_RIGHT_L    high level on LRCK means left channel, low level on LRCK means right channel. 1: LEFT_L_RIGHT_H    high level on LRCK means right channel, low level on LRCK means left channel. Note: this bit should be set to '0' (LEFT_H_RIGHT_L) in voice mode.*/
        U32 serial_ctrl_reserved_4         :1 ; /*7 :7 , No description.               */
        U32 rx_dly                         :2 ; /*9 :8 , Indicates the delay between serial data in MSB and LRCK edge. 2'b00: ALIGN    Digital audio in MSB is aligned with LRCLK edge. 2'b01: DLY_1   Digital audio in MSB is 1 cycle delayed to LRCLK edge. 2'b10: DLY_2   Digital audio in MSB is 2 cycle delayed to LRCLK edge. 2'b11: DLY_3   Digital audio in MSB is 3 cycle delayed to LRCLK edge.*/
        U32 tx_dly                         :1 ; /*10:10, configure the delay between serial data out MSB and LRCK edge. 0: ALIGN           Digital audio out MSB is aligned with LRCLK edge. 1: DLY_1          Digital audio out MSB is 1 cycle delayed to LRCLK edge*/
        U32 tx_dly_s                       :1 ; /*11:11, ONLY for slave mode: configure 1 cycle supplementary Tx delay. 0: NO_DLY  No supplementary Tx delay. 1: DLY         One Cycle supplementary Tx delay.*/
        U32 tx_mode                        :2 ; /*13:12, Configure mono or stereo format for Audio data out. This field is used both in serial mode or in parallel EXT mode. 2'b00: STEREO_STEREO               stereo input from IFC, stereo output to pin. 2'b01: MONO_STEREO_CHAN_L   mono input from IFC, stereo output in left channel to pin. This value is reserved in parallel EXT mode. 2'b10: MONO_STEREO_DUPLI       mono input from IFC, stereo output duplicate in both channels to pin. 2'b11: STEREO_TO_MONO            stereo input from IFC, mono output to left and right channel. This mode is only used for parallel stereo interface. if AIF works in DAI or Voice mode, always select "00" mode STEREO_STEREO.*/
        U32 rx_mode                        :1 ; /*14:14, Configure mono or stereo format for Audio data in. 0: STEREO_STEREO              stereo input from pin, stereo output to IFC. 1: STEREO_MONO_FROM_L  stereo input from pin, mono input to IFC selected from left channel. Users can change LRCK polarity to choose mono input from right channel.*/
        U32 serial_ctrl_reserved_3         :1 ; /*15:15, No description.               */
        U32 bck_lrck                       :5 ; /*20:16, configure the ratio of BCK and LRCK cycle from 16 to 31. Voice_Mode: "XXXX": each sample takes 16 + "XXXX" BCLK cycle. Audio_Mode: "XXXX": each sample takes 2*(16 + "XXXX) BCLK cycle. 2 times than Voice Mode because in audio mode each sample occupies two channels.0:   BCK_LRCK_161:   BCK_LRCK_172:   BCK_LRCK_183:   BCK_LRCK_194:   BCK_LRCK_205:   BCK_LRCK_216:   BCK_LRCK_227:   BCK_LRCK_238:   BCK_LRCK_249:   BCK_LRCK_2510: BCK_LRCK_2611: BCK_LRCK_2712: BCK_LRCK_2813: BCK_LRCK_2914: BCK_LRCK_3015: BCK_LRCK_3116: BCK_LRCK_3217: BCK_LRCK_3318: BCK_LRCK_3419: BCK_LRCK_3520: BCK_LRCK_3621: BCK_LRCK_3722: BCK_LRCK_3823: BCK_LRCK_3924: BCK_LRCK_4025: BCK_LRCK_4126: BCK_LRCK_4227: BCK_LRCK_4328: BCK_LRCK_4429: BCK_LRCK_4530: BCK_LRCK_4631: BCK_LRCK_47*/
        U32 serial_ctrl_reserved_2         :3 ; /*23:21, No description.               */
        U32 bck_pol                        :1 ; /*24:24, if Master Mode, invert BCLK out. if slave Mode, invert BCLK in.0: NORMAL1: INVERT*/
        U32 output_half_cycle_dly          :1 ; /*25:25, delayed Audio output data or LRCK by half cycle.0: NO_DLY1: DLY*/
        U32 input_half_cycle_dly           :1 ; /*26:26, delayed Audio input data by half cycle.0: NO_DLY1: DLY*/
        U32 serial_ctrl_reserved_1         :1 ; /*27:27, No description.               */
        U32 bckout_gate                    :1 ; /*28:28, Sets the BckOut gating. This bit decide if AIF continue to output BCK clock after 16-bit data has been sent.0: NO_GATE1: GATED*/
        U32 serial_ctrl_reserved_0         :3 ; /*31:29, No description.               */
    } b;
} T_AIF_serial_ctrl;

//tone
typedef union
{
    U32 v;
    struct
    {
        U32 enable_h                       :1 ; /*0 :0 , When this bit is set, the audio interface is enabled and a comfort tone or DTMF tone is output on the audio interface instead of the regular data, even if the AIF_CTRL[0] enable bit is 0. 0: DISABLE    AIF is disabled if the AIF_CTRL[0] is also 0. 1: ENABLE     AIF is enabled and generates a tone.*/
        U32 tone_select                    :1 ; /*1 :1 , Select whether a DTMF of a comfort tone is generated.0: DTMF1: COMFORT_TONE*/
        U32 tone_reserved_2                :2 ; /*3 :2 , No description.               */
        U32 dtmf_freq_col                  :2 ; /*5 :4 , Frequency of the first DTMF sine wave.0: 1209_Hz1: 1336_Hz2: 1477_Hz3: 1633_Hz*/
        U32 dtmf_freq_row                  :2 ; /*7 :6 , Frequency of the second DTMF sine wave.0: 697_Hz1: 770_Hz2: 852_Hz3: 941_Hz*/
        U32 comfort_freq                   :2 ; /*9 :8 , Frequency of comfort tone.0: 425_Hz1: 950_Hz2: 1400_Hz3: 1800_Hz*/
        U32 tone_reserved_1                :2 ; /*11:10, No description.               */
        U32 tone_gain                      :2 ; /*13:12, Tone attenuation. The Comfort Tone or DTMF is attenuated according to this programmable gain.0: 0_dB1: m3_dB2: m9_dB3: m15_dB*/
        U32 tone_reserved_0                :18; /*31:14, No description.               */
    } b;
} T_AIF_tone;

//side_tone
typedef union
{
    U32 v;
    struct
    {
        U32 side_tone_gain                 :4 ; /*3 :0 , Side Tone attenuation. The side tone is attenuated according to this programmable gain. 0000: mute. 4'b0001: M36_dB. 4'b0010: M33_dB. 4'b0011: M30_dB. 4'b0100: M27_dB. 4'b0101: M24_dB. 4'b0110: M21_dB. 4'b0111: M18_dB. 4'b1000: M15_dB. 4'b1001: M12_dB. 4'b1010: M9_dB. 4'b1011: M6_dB. 4'b1100: M3_dB. 4'b1101: 0_dB. 4'b1110: 3_dB. 4'b1111: 6_dB.*/
        U32 side_tone_reserved_0           :28; /*31:4 , No description.               */
    } b;
} T_AIF_side_tone;

typedef struct
{
    volatile                      T_AIF_data                           data; /*0x0  , RW   , 0x00000000, No description.               */
    volatile                      T_AIF_ctrl                           ctrl; /*0x4  , RW   , 0x00000000, No description.               */
    volatile               T_AIF_serial_ctrl                    serial_ctrl; /*0x8  , RW   , 0x00000010, No description.               */
    volatile                      T_AIF_tone                           tone; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                 T_AIF_side_tone                      side_tone; /*0x10 , RW   , 0x00000000, No description.               */
} T_HWP_AIF_T;
#else
typedef struct
{
    volatile                             U32                           data; /*0x0  , RW   , 0x00000000, No description.               */
    volatile                             U32                           ctrl; /*0x4  , RW   , 0x00000000, No description.               */
    volatile                             U32                    serial_ctrl; /*0x8  , RW   , 0x00000010, No description.               */
    volatile                             U32                           tone; /*0xc  , RW   , 0x00000000, No description.               */
    volatile                             U32                      side_tone; /*0x10 , RW   , 0x00000000, No description.               */
} T_HWP_AIF_T;

//data
#define AIF_DATA0(n)                   (((n)&0xFFFF)<<0)   
#define AIF_DATA0_MASK                 (0xFFFF<<0)         
#define AIF_DATA0_SHIFT                (0)                 

#define AIF_DATA1(n)                   (((n)&0xFFFF)<<16)  
#define AIF_DATA1_MASK                 (0xFFFF<<16)        
#define AIF_DATA1_SHIFT                (16)                


//ctrl
#define AIF_ENABLE                     (1<<0)              
#define AIF_ENABLE_DISABLE             (0<<0)              
#define AIF_ENABLE_ENABLE              (1<<0)              

#define AIF_TX_OFF                     (1<<4)              
#define AIF_TX_OFF_TX_ON               (0<<4)              
#define AIF_TX_OFF_TX_OFF              (1<<4)              

#define AIF_PARALLEL_OUT_SET           (1<<8)              
#define AIF_PARALLEL_OUT_SET_SERL      (0<<8)              
#define AIF_PARALLEL_OUT_SET_PARA      (1<<8)              

#define AIF_PARALLEL_OUT_CLR           (1<<9)              
#define AIF_PARALLEL_OUT_CLR_SERL      (0<<9)              
#define AIF_PARALLEL_OUT_CLR_PARA      (1<<9)              

#define AIF_PARALLEL_IN_SET            (1<<10)             
#define AIF_PARALLEL_IN_SET_SERL       (0<<10)             
#define AIF_PARALLEL_IN_SET_PARA       (1<<10)             

#define AIF_PARALLEL_IN_CLR            (1<<11)             
#define AIF_PARALLEL_IN_CLR_SERL       (0<<11)             
#define AIF_PARALLEL_IN_CLR_PARA       (1<<11)             

#define AIF_TX_STB_MODE                (1<<12)             
#define AIF_TX_STB_MODE_TX_STB_MID     (0<<12)             
#define AIF_TX_STB_MODE_TX_STB_ALIGN   (1<<12)             

#define AIF_OUT_UNDERFLOW              (1<<16)             

#define AIF_IN_OVERFLOW                (1<<17)             

#define AIF_LOOP_BACK                  (1<<31)             
#define AIF_LOOP_BACK_NORMAL           (0<<31)             
#define AIF_LOOP_BACK_LOOPBACK         (1<<31)             


//serial_ctrl
#define AIF_SERIAL_MODE(n)             (((n)&0x3)<<0)      
#define AIF_SERIAL_MODE_I2S_PCM        (((0)&0x3)<<0)      
#define AIF_SERIAL_MODE_VOICE          (((1)&0x3)<<0)      
#define AIF_SERIAL_MODE_DAI            (((2)&0x3)<<0)      
#define AIF_SERIAL_MODE_RESERVED       (((3)&0x3)<<0)      
#define AIF_SERIAL_MODE_MASK           (0x3<<0)            
#define AIF_SERIAL_MODE_SHIFT          (0)                 

#define AIF_I2S_IN_SEL(n)              (((n)&0x3)<<2)      
#define AIF_I2S_IN_SEL_I2S_IN_0        (((0)&0x3)<<2)      
#define AIF_I2S_IN_SEL_I2S_IN_1        (((1)&0x3)<<2)      
#define AIF_I2S_IN_SEL_I2S_IN_2        (((2)&0x3)<<2)      
#define AIF_I2S_IN_SEL_MASK            (0x3<<2)            
#define AIF_I2S_IN_SEL_SHIFT           (2)                 

#define AIF_MASTER_MODE                (1<<4)              
#define AIF_MASTER_MODE_SLAVE          (0<<4)              
#define AIF_MASTER_MODE_MASTER         (1<<4)              

#define AIF_LSB                        (1<<5)              
#define AIF_LSB_MSB                    (0<<5)              
#define AIF_LSB_LSB                    (1<<5)              

#define AIF_LRCK_POL                   (1<<6)              
#define AIF_LRCK_POL_LEFT_H_RIGHT_L    (0<<6)              
#define AIF_LRCK_POL_LEFT_L_RIGHT_H    (1<<6)              

#define AIF_RX_DLY(n)                  (((n)&0x3)<<8)      
#define AIF_RX_DLY_ALIGN               (((0)&0x3)<<8)      
#define AIF_RX_DLY_DLY_1               (((1)&0x3)<<8)      
#define AIF_RX_DLY_DLY_2               (((2)&0x3)<<8)      
#define AIF_RX_DLY_DLY_3               (((3)&0x3)<<8)      
#define AIF_RX_DLY_MASK                (0x3<<8)            
#define AIF_RX_DLY_SHIFT               (8)                 

#define AIF_TX_DLY                     (1<<10)             
#define AIF_TX_DLY_ALIGN               (0<<10)             
#define AIF_TX_DLY_DLY_1               (1<<10)             

#define AIF_TX_DLY_S                   (1<<11)             
#define AIF_TX_DLY_S_NO_DLY            (0<<11)             
#define AIF_TX_DLY_S_DLY               (1<<11)             

#define AIF_TX_MODE(n)                 (((n)&0x3)<<12)     
#define AIF_TX_MODE_STEREO_STEREO      (((0)&0x3)<<12)     
#define AIF_TX_MODE_MONO_STEREO_CHAN_L (((1)&0x3)<<12)     
#define AIF_TX_MODE_MONO_STEREO_DUPLI  (((2)&0x3)<<12)     
#define AIF_TX_MODE_STEREO_TO_MONO     (((3)&0x3)<<12)     
#define AIF_TX_MODE_MASK               (0x3<<12)           
#define AIF_TX_MODE_SHIFT              (12)                

#define AIF_RX_MODE                    (1<<14)             
#define AIF_RX_MODE_STEREO_STEREO      (0<<14)             
#define AIF_RX_MODE_STEREO_MONO_FROM_L (1<<14)             

#define AIF_BCK_LRCK(n)                (((n)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_16       (((0)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_17       (((1)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_18       (((2)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_19       (((3)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_20       (((4)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_21       (((5)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_22       (((6)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_23       (((7)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_24       (((8)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_25       (((9)&0x1F)<<16)    
#define AIF_BCK_LRCK_BCK_LRCK_26       (((10)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_27       (((11)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_28       (((12)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_29       (((13)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_30       (((14)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_31       (((15)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_32       (((16)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_33       (((17)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_34       (((18)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_35       (((19)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_36       (((20)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_37       (((21)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_38       (((22)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_39       (((23)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_40       (((24)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_41       (((25)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_42       (((26)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_43       (((27)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_44       (((28)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_45       (((29)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_46       (((30)&0x1F)<<16)   
#define AIF_BCK_LRCK_BCK_LRCK_47       (((31)&0x1F)<<16)   
#define AIF_BCK_LRCK_MASK              (0x1F<<16)          
#define AIF_BCK_LRCK_SHIFT             (16)                

#define AIF_BCK_POL                    (1<<24)             
#define AIF_BCK_POL_NORMAL             (0<<24)             
#define AIF_BCK_POL_INVERT             (1<<24)             

#define AIF_OUTPUT_HALF_CYCLE_DLY      (1<<25)             
#define AIF_OUTPUT_HALF_CYCLE_DLY_NO_DLY (0<<25)             
#define AIF_OUTPUT_HALF_CYCLE_DLY_DLY  (1<<25)             

#define AIF_INPUT_HALF_CYCLE_DLY       (1<<26)             
#define AIF_INPUT_HALF_CYCLE_DLY_NO_DLY (0<<26)             
#define AIF_INPUT_HALF_CYCLE_DLY_DLY   (1<<26)             

#define AIF_BCKOUT_GATE                (1<<28)             
#define AIF_BCKOUT_GATE_NO_GATE        (0<<28)             
#define AIF_BCKOUT_GATE_GATED          (1<<28)             


//tone
#define AIF_ENABLE_H                   (1<<0)              
#define AIF_ENABLE_H_DISABLE           (0<<0)              
#define AIF_ENABLE_H_ENABLE            (1<<0)              

#define AIF_TONE_SELECT                (1<<1)              
#define AIF_TONE_SELECT_DTMF           (0<<1)              
#define AIF_TONE_SELECT_COMFORT_TONE   (1<<1)              

#define AIF_DTMF_FREQ_COL(n)           (((n)&0x3)<<4)      
#define AIF_DTMF_FREQ_COL_1209_HZ      (((0)&0x3)<<4)      
#define AIF_DTMF_FREQ_COL_1336_HZ      (((1)&0x3)<<4)      
#define AIF_DTMF_FREQ_COL_1477_HZ      (((2)&0x3)<<4)      
#define AIF_DTMF_FREQ_COL_1633_HZ      (((3)&0x3)<<4)      
#define AIF_DTMF_FREQ_COL_MASK         (0x3<<4)            
#define AIF_DTMF_FREQ_COL_SHIFT        (4)                 

#define AIF_DTMF_FREQ_ROW(n)           (((n)&0x3)<<6)      
#define AIF_DTMF_FREQ_ROW_697_HZ       (((0)&0x3)<<6)      
#define AIF_DTMF_FREQ_ROW_770_HZ       (((1)&0x3)<<6)      
#define AIF_DTMF_FREQ_ROW_852_HZ       (((2)&0x3)<<6)      
#define AIF_DTMF_FREQ_ROW_941_HZ       (((3)&0x3)<<6)      
#define AIF_DTMF_FREQ_ROW_MASK         (0x3<<6)            
#define AIF_DTMF_FREQ_ROW_SHIFT        (6)                 

#define AIF_COMFORT_FREQ(n)            (((n)&0x3)<<8)      
#define AIF_COMFORT_FREQ_425_HZ        (((0)&0x3)<<8)      
#define AIF_COMFORT_FREQ_950_HZ        (((1)&0x3)<<8)      
#define AIF_COMFORT_FREQ_1400_HZ       (((2)&0x3)<<8)      
#define AIF_COMFORT_FREQ_1800_HZ       (((3)&0x3)<<8)      
#define AIF_COMFORT_FREQ_MASK          (0x3<<8)            
#define AIF_COMFORT_FREQ_SHIFT         (8)                 

#define AIF_TONE_GAIN(n)               (((n)&0x3)<<12)     
#define AIF_TONE_GAIN_0_DB             (((0)&0x3)<<12)     
#define AIF_TONE_GAIN_M3_DB            (((1)&0x3)<<12)     
#define AIF_TONE_GAIN_M9_DB            (((2)&0x3)<<12)     
#define AIF_TONE_GAIN_M15_DB           (((3)&0x3)<<12)     
#define AIF_TONE_GAIN_MASK             (0x3<<12)           
#define AIF_TONE_GAIN_SHIFT            (12)                


//side_tone
#define AIF_SIDE_TONE_GAIN(n)          (((n)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_MUTE       (((0000)&0xF)<<0)   
#define AIF_SIDE_TONE_GAIN_M36_DB     (((1)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M33_DB     (((2)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M30_DB     (((3)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M27_DB     (((4)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M24_DB     (((5)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M21_DB     (((6)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M18_DB     (((7)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M15_DB     (((8)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M12_DB     (((9)&0xF)<<0)      
#define AIF_SIDE_TONE_GAIN_M9_DB      (((10)&0xF)<<0)     
#define AIF_SIDE_TONE_GAIN_M6_DB      (((11)&0xF)<<0)     
#define AIF_SIDE_TONE_GAIN_M3_DB      (((12)&0xF)<<0)     
#define AIF_SIDE_TONE_GAIN_0_DB       (((13)&0xF)<<0)     
#define AIF_SIDE_TONE_GAIN_3_DB       (((14)&0xF)<<0)     
#define AIF_SIDE_TONE_GAIN_6_DB       (((15)&0xF)<<0)     
#define AIF_SIDE_TONE_GAIN_MASK        (0xF<<0)            
#define AIF_SIDE_TONE_GAIN_SHIFT       (0)                 


#endif

#endif


