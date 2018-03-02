/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


//#define MP3_ENC_Code_size 3800
#define MP3_ENC_ConstX_size 1084
#define MP3_ENC_ConstY_size 972
#define MP3_ENC_Const_rqmy_size 52
#define MP3_ENC_Const_Zig_size 48





// VoC_directive: PARSER_OFF
void count_bit(void);
void new_choose_table(void);
void quantize(void);
void calc_runlen(void);
void count1_bitcount(void);
void subdivide(void);
void bigv_tab_select(void);
void bigv_bitcount(void);
void bin_search_StepSize(void);
void inner_loop(void);
void ResvMaxBits (void);
void ResvAdjust(void);
void ResvFrameEnd(void);
void cii_mdct18(void);
void L3_mdct_sub(void);
void L3_window_filter_subband(void);
void L3_iteration_loop(void);
void CS_L3_format_bitstream(void);
void L3_compress(void);
void CS_encodeSideInfo(void);
void CS_encodeMainData(void);
void CS_Huffmancodebits(void);
void CS_HuffmanCode(void);
void CS_L3_huffman_coder_count1(void);
void CS_BF_BitstreamFrame(void);
void CS_store_side_info(void);
void CS_main_data(void);
void CS_WriteMainDataBits(void);
void CS_write_side_info(void);
void CS_putbits(void);
void CS_writePartMainData(void);
void MP3_reset(void);




// VoC_directive: PARSER_ON