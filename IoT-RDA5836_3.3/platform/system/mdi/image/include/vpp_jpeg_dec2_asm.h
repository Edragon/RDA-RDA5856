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

/*****
for jpeg2 voc decoder.
sheen
****/

#ifndef VPP_JPEG_DEC2_ASM_H
#define VPP_JPEG_DEC2_ASM_H

#include "cs_types.h"


#ifdef SHEEN_VC_DEBUG

#define VOC_CFG_DMA_EADDR_MASK (0xFFFFFFFF)
#define NoCacheAddr
#else

#define VOC_CFG_DMA_EADDR_MASK (0xFFFFFF<<2)
#define NoCacheAddr(a) ((UINT32)(a)|0x20000000)
#endif

//copy from MAP_ADDR.h when modify.
#define RAM_X_BEGIN_ADDR               0x0000
#define RAM_Y_BEGIN_ADDR               0x4000 
#define JPEG2_GLOBAL_X_BEGIN_ADDR                         		(0 + RAM_X_BEGIN_ADDR)
#define JPEG2_GLOBAL_Y_BEGIN_ADDR                         		(74 + RAM_Y_BEGIN_ADDR)

//JPEG_GLOBAL_X_STRUCT
#define JPEG_GLOBAL_X_STRUCT                    			(512 + JPEG2_GLOBAL_X_BEGIN_ADDR)
#define JPEG_GX_bs_cache                        			(0 + JPEG_GLOBAL_X_STRUCT) //2 short
#define JPEG_GX_bs_buf_extaddr                  			(2 + JPEG_GLOBAL_X_STRUCT) //2 short
#define JPEG_GX_bs_bitcnt                       			(4 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_output_height                   			(5 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_dest_buffer_EXT                 			(6 + JPEG_GLOBAL_X_STRUCT) //2 short
#define JPEG_GX_dest_bufferU_EXT                			(8 + JPEG_GLOBAL_X_STRUCT) //2 short
#define JPEG_GX_dest_bufferV_EXT                			(10 + JPEG_GLOBAL_X_STRUCT) //2 short
#define JPEG_GX_width                           			(12 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_height                          			(13 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_dest_width                      			(14 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_dest_height                     			(15 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCU_vert_offset                 			(16 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCU_rows_per_iMCU_row           			(17 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCU_ctr                         			(18 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCUs_per_row                    			(19 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_total_iMCU_rows                 			(20 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_blocks_in_MCU                   			(21 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_comps_in_scan                   			(22 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_restart_interval                			(23 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_restarts_to_go                  			(24 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_insufficient_data               			(25 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCU_cut_row_start               			(26 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCU_cut_col_start               			(27 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCU_cut_row_end                 			(28 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_MCU_cut_col_end                 			(29 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_component_needed                			(30 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_MCU_blocks                      			(33 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_DCT_scaled_size                 			(36 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_MCU_width                       			(39 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_MCU_height                      			(42 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_last_col_width                  			(45 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_last_row_height                 			(48 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_MCU_sample_width                			(51 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_width_in_blocks                 			(54 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_v_samp_factor                   			(57 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_h_samp_factor                   			(60 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_bs_half_consume                 			(63 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_dc_tbl_no                       			(64 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_ac_tbl_no                       			(67 + JPEG_GLOBAL_X_STRUCT) //3 short
#define JPEG_GX_MCU_membership                  			(70 + JPEG_GLOBAL_X_STRUCT) //10 short
#define JPEG_GX_dc_needed                       			(80 + JPEG_GLOBAL_X_STRUCT) //10 short
#define JPEG_GX_ac_needed                       			(90 + JPEG_GLOBAL_X_STRUCT) //10 short
#define JPEG_GX_finish_output                   			(100 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_process_data                    			(101 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_decompress_data                 			(102 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_decode_mcu                      			(103 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_post_process_data               			(104 + JPEG_GLOBAL_X_STRUCT) //1 short
#define JPEG_GX_reserve1                        			(105 + JPEG_GLOBAL_X_STRUCT) //1 short


#define JPEG_GX_bs_need_dma                     			(618 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_out_row_ctr                     			(619 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_output_scanline                 			(620 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_yoffset                         			(621 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_input_iMCU_row                  			(622 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_MCU_col_num                     			(623 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_blkn                            			(624 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_ci                              			(625 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_useful_width                    			(626 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_useful_height                   			(627 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_start_col                       			(628 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_output_col                      			(629 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_output_ptr                      			(630 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_yindex                          			(631 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_xindex                          			(632 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_MCU_buffer_ptr                  			(633 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_last_dc_val                     			(634 + JPEG2_GLOBAL_X_BEGIN_ADDR) //4 short
#define JPEG_GX_YUV_output_buffer_ptr           			(638 + JPEG2_GLOBAL_X_BEGIN_ADDR) //3 short
#define JPEG_GX_decode_mcu_blkn                 			(641 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_ci_01                           			(642 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_huffval_ptr                     			(643 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_maxcode_ptr                     			(644 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_valoffset_ptr                   			(645 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_look_nbits_ptr                  			(646 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_look_sym_ptr                    			(647 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_s                               			(648 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_iquant_dct_table_ptr            			(649 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_output_buffer_stride            			(650 + JPEG2_GLOBAL_X_BEGIN_ADDR) //1 short
#define JPEG_GX_YUV_output_buffer_stride        			(651 + JPEG2_GLOBAL_X_BEGIN_ADDR) //3 short
#define JPEG_GX_MCU_buffer                      			(654 + JPEG2_GLOBAL_X_BEGIN_ADDR) //640 short
#define JPEG_GX_YUV_output_buffer               			(1294 + JPEG2_GLOBAL_X_BEGIN_ADDR) //7680 short

//GLOBAL_VARS
#define JPEG_GY_CONST_START                     			(0 + JPEG2_GLOBAL_Y_BEGIN_ADDR) //16 short
//JPEG_GY_CONSTY_STRUCT
#define JPEG_GY_CONSTY_STRUCT                   			(16 + JPEG2_GLOBAL_Y_BEGIN_ADDR)
#define JPEG_GY_dc_maxcode0                     			(0 + JPEG_GY_CONSTY_STRUCT) //36 short
#define JPEG_GY_dc_valoffset0                   			(36 + JPEG_GY_CONSTY_STRUCT) //34 short
#define JPEG_GY_dc_look_nbits0                  			(70 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_dc_look_sym0                    			(198 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_dc_maxcode1                     			(326 + JPEG_GY_CONSTY_STRUCT) //36 short
#define JPEG_GY_dc_valoffset1                   			(362 + JPEG_GY_CONSTY_STRUCT) //34 short
#define JPEG_GY_dc_look_nbits1                  			(396 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_dc_look_sym1                    			(524 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_ac_maxcode0                     			(652 + JPEG_GY_CONSTY_STRUCT) //36 short
#define JPEG_GY_ac_valoffset0                   			(688 + JPEG_GY_CONSTY_STRUCT) //34 short
#define JPEG_GY_ac_look_nbits0                  			(722 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_ac_look_sym0                    			(850 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_ac_maxcode1                     			(978 + JPEG_GY_CONSTY_STRUCT) //36 short
#define JPEG_GY_ac_valoffset1                   			(1014 + JPEG_GY_CONSTY_STRUCT) //34 short
#define JPEG_GY_ac_look_nbits1                  			(1048 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_ac_look_sym1                    			(1176 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_dc_huffval0                     			(1304 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_dc_huffval1                     			(1432 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_ac_huffval0                     			(1560 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_ac_huffval1                     			(1688 + JPEG_GY_CONSTY_STRUCT) //128 short
#define JPEG_GY_jpeg_natural_order              			(1816 + JPEG_GY_CONSTY_STRUCT) //64 short
#define JPEG_GY_iquant_dct_tableY               			(1880 + JPEG_GY_CONSTY_STRUCT) //64 short
#define JPEG_GY_iquant_dct_tableUV              			(1944 + JPEG_GY_CONSTY_STRUCT) //64 short



#endif

