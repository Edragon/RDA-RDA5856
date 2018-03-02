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
#ifndef AUDIO_MAP_ADDR_H
#define AUDIO_MAP_ADDR_H


/* This file defines all the variables as memory addresses.
   It's created by voc_map automatically.*/

#define RAM_X_BEGIN_ADDR               0x0000
#define RAM_Y_BEGIN_ADDR               0x4000 
#define RAM_I_BEGIN_ADDR               0x10000 

/********************
 **  SECTION_SIZE  **
 ********************/

#define COMMON_GLOBAL_X_SIZE                              		0
#define COMMON_GLOBAL_Y_SIZE                              		74
#define AAC_GLOBAL_X_SIZE                                 		10150
#define AAC_GLOBAL_Y_SIZE                                 		10106
#define AAC_LOCAL_X_SIZE                                  		0
#define AAC_LOCAL_Y_SIZE                                  		0
#define RM_GLOBAL_X_SIZE                                  		3256
#define RM_GLOBAL_Y_SIZE                                  		7540
#define RM_LOCAL_X_SIZE                                   		4096
#define RM_LOCAL_Y_SIZE                                   		1162
#define MP3_GLOBAL_X_SIZE                                 		10156
#define MP3_GLOBAL_Y_SIZE                                 		9572
#define MP3_LOCAL_X_SIZE                                  		68
#define MP3_LOCAL_Y_SIZE                                  		338
#define AMR_GLOBAL_X_SIZE                                 		1120
#define AMR_GLOBAL_Y_SIZE                                 		6532
#define AMR_LOCAL_X_SIZE                                  		578
#define AMR_LOCAL_Y_SIZE                                  		240
#define JPEG_GLOBAL_X_SIZE                                		9184
#define JPEG_GLOBAL_Y_SIZE                                		10104
#define JPEG_LOCAL_X_SIZE                                 		0
#define JPEG_LOCAL_Y_SIZE                                 		0
#define H263zoom_GLOBAL_X_SIZE                            		8272
#define H263zoom_GLOBAL_Y_SIZE                            		9890
#define H263zoom_LOCAL_X_SIZE                             		0
#define H263zoom_LOCAL_Y_SIZE                             		64
#define SBCDEC_GLOBAL_X_SIZE                              		4944
#define SBCDEC_GLOBAL_Y_SIZE                              		8964
#define SBCDEC_LOCAL_X_SIZE                               		0
#define SBCDEC_LOCAL_Y_SIZE                               		0
#define SBCENC_GLOBAL_X_SIZE                              		7404
#define SBCENC_GLOBAL_Y_SIZE                              		9924
#define SBCENC_LOCAL_X_SIZE                               		1786
#define SBCENC_LOCAL_Y_SIZE                               		140
#define DRC_GLOBAL_X_SIZE                                 		10172
#define DRC_GLOBAL_Y_SIZE                                 		9516
#define DRC_LOCAL_X_SIZE                                  		0
#define DRC_LOCAL_Y_SIZE                                  		0
#define COMMON_LOCAL_X_SIZE                               		0
#define COMMON_LOCAL_Y_SIZE                               		0

/**************************
 **  SECTION_BEGIN_ADDR  **
 **************************/

#define COMMON_GLOBAL_X_BEGIN_ADDR                        		(0 + RAM_X_BEGIN_ADDR)
#define COMMON_GLOBAL_Y_BEGIN_ADDR                        		(0 + RAM_Y_BEGIN_ADDR)
#define AAC_GLOBAL_X_BEGIN_ADDR                           		(0 + RAM_X_BEGIN_ADDR)
#define AAC_GLOBAL_Y_BEGIN_ADDR                           		(74 + RAM_Y_BEGIN_ADDR)
#define AAC_LOCAL_X_BEGIN_ADDR                            		(10150 + RAM_X_BEGIN_ADDR)
#define AAC_LOCAL_Y_BEGIN_ADDR                            		(10180 + RAM_Y_BEGIN_ADDR)
#define RM_GLOBAL_X_BEGIN_ADDR                            		(0 + RAM_X_BEGIN_ADDR)
#define RM_GLOBAL_Y_BEGIN_ADDR                            		(74 + RAM_Y_BEGIN_ADDR)
#define RM_LOCAL_X_BEGIN_ADDR                             		(3256 + RAM_X_BEGIN_ADDR)
#define RM_LOCAL_Y_BEGIN_ADDR                             		(7614 + RAM_Y_BEGIN_ADDR)
#define MP3_GLOBAL_X_BEGIN_ADDR                           		(0 + RAM_X_BEGIN_ADDR)
#define MP3_GLOBAL_Y_BEGIN_ADDR                           		(74 + RAM_Y_BEGIN_ADDR)
#define MP3_LOCAL_X_BEGIN_ADDR                            		(10156 + RAM_X_BEGIN_ADDR)
#define MP3_LOCAL_Y_BEGIN_ADDR                            		(9646 + RAM_Y_BEGIN_ADDR)
#define AMR_GLOBAL_X_BEGIN_ADDR                           		(0 + RAM_X_BEGIN_ADDR)
#define AMR_GLOBAL_Y_BEGIN_ADDR                           		(74 + RAM_Y_BEGIN_ADDR)
#define AMR_LOCAL_X_BEGIN_ADDR                            		(1120 + RAM_X_BEGIN_ADDR)
#define AMR_LOCAL_Y_BEGIN_ADDR                            		(6606 + RAM_Y_BEGIN_ADDR)
#define JPEG_GLOBAL_X_BEGIN_ADDR                          		(0 + RAM_X_BEGIN_ADDR)
#define JPEG_GLOBAL_Y_BEGIN_ADDR                          		(74 + RAM_Y_BEGIN_ADDR)
#define JPEG_LOCAL_X_BEGIN_ADDR                           		(9184 + RAM_X_BEGIN_ADDR)
#define JPEG_LOCAL_Y_BEGIN_ADDR                           		(10178 + RAM_Y_BEGIN_ADDR)
#define H263zoom_GLOBAL_X_BEGIN_ADDR                      		(0 + RAM_X_BEGIN_ADDR)
#define H263zoom_GLOBAL_Y_BEGIN_ADDR                      		(74 + RAM_Y_BEGIN_ADDR)
#define H263zoom_LOCAL_X_BEGIN_ADDR                       		(8272 + RAM_X_BEGIN_ADDR)
#define H263zoom_LOCAL_Y_BEGIN_ADDR                       		(9964 + RAM_Y_BEGIN_ADDR)
#define SBCDEC_GLOBAL_X_BEGIN_ADDR                        		(0 + RAM_X_BEGIN_ADDR)
#define SBCDEC_GLOBAL_Y_BEGIN_ADDR                        		(74 + RAM_Y_BEGIN_ADDR)
#define SBCDEC_LOCAL_X_BEGIN_ADDR                         		(4944 + RAM_X_BEGIN_ADDR)
#define SBCDEC_LOCAL_Y_BEGIN_ADDR                         		(9038 + RAM_Y_BEGIN_ADDR)
#define SBCENC_GLOBAL_X_BEGIN_ADDR                        		(0 + RAM_X_BEGIN_ADDR)
#define SBCENC_GLOBAL_Y_BEGIN_ADDR                        		(74 + RAM_Y_BEGIN_ADDR)
#define SBCENC_LOCAL_X_BEGIN_ADDR                         		(7404 + RAM_X_BEGIN_ADDR)
#define SBCENC_LOCAL_Y_BEGIN_ADDR                         		(9998 + RAM_Y_BEGIN_ADDR)
#define DRC_GLOBAL_X_BEGIN_ADDR                           		(0 + RAM_X_BEGIN_ADDR)
#define DRC_GLOBAL_Y_BEGIN_ADDR                           		(74 + RAM_Y_BEGIN_ADDR)
#define DRC_LOCAL_X_BEGIN_ADDR                            		(10172 + RAM_X_BEGIN_ADDR)
#define DRC_LOCAL_Y_BEGIN_ADDR                            		(9590 + RAM_Y_BEGIN_ADDR)
#define COMMON_LOCAL_X_BEGIN_ADDR                         		(10156 + RAM_X_BEGIN_ADDR)
#define COMMON_LOCAL_Y_BEGIN_ADDR                         		(10180 + RAM_Y_BEGIN_ADDR)

/*********************
 **  COMMON_GLOBAL  **
 *********************/

//GLOBAL_VARS
#define Input_mode_addr                         			(0 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Input_reset_addr                        			(1 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Flag_need_bluetooth_ADDR                			(2 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Input_audioItf_ADDR                     			(3 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Input_inStreamBufAddr_addr              			(4 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Input_outStreamBufAddr_addr             			(6 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define DEC_EQ_Type_ADDR                        			(8 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Input_zoom_mode_addr                    			(9 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Source_width_ADDR                       			(10 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define STRUCT_sbc_frame_mode                   			(Source_width_ADDR)
#define Source_height_ADDR                      			(11 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define STRUCT_sbc_frame_allocation             			(Source_height_ADDR)
#define Cut_width_ADDR                          			(12 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define STRUCT_sbc_frame_bitpool                			(Cut_width_ADDR)
#define Cut_height_ADDR                         			(13 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define STRUCT_sbc_frame_frequency              			(Cut_height_ADDR)
#define Zoomed_width_ADDR                       			(14 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Zoomed_height_ADDR                      			(15 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define INPUT_ADDR_U_ADDR                       			(16 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define INPUT_ADDR_V_ADDR                       			(18 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Output_mode_addr                        			(20 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_nbChannel_addr                   			(21 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_output_len_addr                  			(22 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Output_SampleRate_addr                  			(24 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Output_BitRate_addr                     			(26 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_consumedLen_addr                 			(27 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_streamStatus_addr                			(28 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_ERR_Status_addr                  			(29 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_image_ImgWidth_addr              			(30 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_image_ImgHeight_addr             			(31 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_Reserve_for_bar                  			(32 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define STRUCT_SBC_RESET_ADDR                   			((Output_Reserve_for_bar))
#define STRUCT_SBC_RESAMPLE_RATE_ADDR           			((Output_Reserve_for_bar+1))
#define Drc_Code_Load_Enable_addr               			((Output_Reserve_for_bar))
#define Speak_AntiDistortion_Filter_Coef_addr   			(34 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Current_Audio_Mode_addr                 			(36 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Current_Video_Mode_addr                 			(37 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define AAC_Code_ExternalAddr_addr              			(38 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define MP3L12_ConstX_ExternalAddr_addr         			(40 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define AAC_ConstY_ExternalAddr_addr            			(42 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Tab_huffTabSpec_START_addr              			(44 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Tab_cos4sin4tab_START_addr              			(46 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Tab_twidTabOdd_START_addr               			(48 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define Tab_sinWindow_START_addr                			(50 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define MP3_Code_ExternalAddr_addr              			(52 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define MP3_ConstX_ExternalAddr_addr            			(54 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define MP3_ConstY_ExternalAddr_addr            			(56 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define AMR_Dec_Code_ExternalAddr_addr          			(58 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define AMR_Dec_ConstX_ExternalAddr_addr        			(60 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define AMR_Dec_ConstY_ExternalAddr_addr        			(62 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define JPEG_Dec_Code_ExternalAddr_addr         			(64 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define JPEG_Dec_ConstY_ExternalAddr_addr       			(66 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define DRC_MODE_Code_ExternalAddr_addr         			(JPEG_Dec_Code_ExternalAddr_addr)
#define DRC_MODE_Const_ExternalAddr_addr        			(JPEG_Dec_ConstY_ExternalAddr_addr)
#define H263_Zoom_Code_ExternalAddr_addr        			(68 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define H263_Zoom_ConstX_ExternalAddr_addr      			(70 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_ENC_Code_ExternalAddr_addr          			(72 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short

/******************
 **  AAC_GLOBAL  **
 ******************/

//GLOBAL_VARS
#define STATIC_readBuf_addr                     			(0 + AAC_GLOBAL_X_BEGIN_ADDR) //1024 short
#define STRUCT_AACDecInfo_PSInfoBase_overlap_addr			(1024 + AAC_GLOBAL_X_BEGIN_ADDR) //2048 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsLPCBuf_addr			(3072 + AAC_GLOBAL_X_BEGIN_ADDR) //40 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsWorkBuf_addr			(3112 + AAC_GLOBAL_X_BEGIN_ADDR) //40 short
#define STRUCT_AACDecInfo_PSInfoBase_gbCurrent_addr			(3152 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define STRUCT_AACDecInfo_PSInfoBase_prevWinShape_addr			(3154 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_prevWinShape1_addr			(3155 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_bitbuffer_addr                   			(3156 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define GLOBAL_bytesLeft_addr                   			(3158 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_inputbuffiled_ptr_addr           			(3159 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_inputptr_addr                    			(3160 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_BITCACHE_addr                    			(3161 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_TNSFilter_nWindows_addr           			(3162 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_TNSFilter_winLen_addr             			(3163 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_TNSFilter_nSFB_addr               			(3164 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_TNSFilter_maxOrder_addr           			(3165 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_TNSFilter_tnsMaxBand_addr         			(3166 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_TNSFilter_order_addr              			(3167 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_TNSFilter_gbMask_addr             			(3168 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define LOCAL_Dequantize_gbMask_addr            			(3170 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define LOCAL_DecodeSectionData_maxSFB          			(3172 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_gp_M_maxSFB_addr              			(3173 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_CH_addr                       			(3174 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_maxSFB_addr                   			(3175 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
//STRUCT_AACDecInfo_addr
#define STRUCT_AACDecInfo_addr                  			(3176 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_nChans_addr			(0 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_useImpChanMap_addr			(1 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_sampRateIdx_addr			(2 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_prevBlockID_addr      			(3 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_currBlockID_addr      			(4 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_currInstTag_addr      			(5 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_sbDeinterleaveReqd_addr			(6 + STRUCT_AACDecInfo_addr) //4 short
#define STRUCT_AACDecInfo_adtsBlocksLeft_addr   			(10 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_nChans_addr           			(11 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_bitRate_addr          			(12 + STRUCT_AACDecInfo_addr) //2 short
#define STRUCT_AACDecInfo_sampRate_addr         			(14 + STRUCT_AACDecInfo_addr) //2 short
#define STRUCT_AACDecInfo_profile_addr          			(16 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_format_addr           			(17 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_sbrEnabled_addr       			(18 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_tnsUsed_addr          			(19 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_pnsUsed_addr          			(20 + STRUCT_AACDecInfo_addr) //1 short
#define STRUCT_AACDecInfo_frameCount_addr       			(21 + STRUCT_AACDecInfo_addr) //1 short

//STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr			(3198 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_id_addr			(0 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_layer_addr			(1 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_protectBit_addr			(2 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_profile_addr			(3 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_sampRateIdx_addr			(4 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_privateBit_addr			(5 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_channelConfig_addr			(6 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_origCopy_addr			(7 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_home_addr			(8 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_copyBit_addr			(9 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_copyStart_addr			(10 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_frameLength_addr			(11 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_bufferFull_addr			(12 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_fhADTS_numRawDataBlocks_addr			(13 + STRUCT_AACDecInfo_PSInfoBase_fhADTS_addr) //1 short

//STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr			(3212 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_icsResBit_addr			(0 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_winSequence_addr			(1 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_winShape_addr			(2 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_maxSFB_addr			(3 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_sfGroup_addr			(4 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_predictorDataPresent_addr			(5 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_numWinGroup_addr			(6 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo_winGroupLen_addr			(7 + STRUCT_AACDecInfo_PSInfoBase_icsInfo_addr) //8 short

//STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr			(3227 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_icsResBit_addr			(0 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_winSequence_addr			(1 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_winShape_addr			(2 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_maxSFB_addr			(3 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_sfGroup_addr			(4 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_predictorDataPresent_addr			(5 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_numWinGroup_addr			(6 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_icsInfo1_winGroupLen_addr			(7 + STRUCT_AACDecInfo_PSInfoBase_icsInfo1_addr) //8 short

#define RESERVED_FOR_H263                       			(3242 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define STATIC_outbuf_addr                      			(3244 + AAC_GLOBAL_X_BEGIN_ADDR) //2048 short
#define STRUCT_AACDecInfo_PSInfoBase_coef_addr  			(5292 + AAC_GLOBAL_X_BEGIN_ADDR) //2048 short
#define STRUCT_AACDecInfo_PSInfoBase_coef1_addr 			(7340 + AAC_GLOBAL_X_BEGIN_ADDR) //2048 short
#define STRUCT_AACDecInfo_PSInfoBase_commonWin_addr			(9388 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_scaleFactors_addr			(9389 + AAC_GLOBAL_X_BEGIN_ADDR) //120 short
#define STRUCT_AACDecInfo_PSInfoBase_scaleFactors1_addr			(9509 + AAC_GLOBAL_X_BEGIN_ADDR) //120 short
#define STRUCT_AACDecInfo_PSInfoBase_sfbCodeBook_addr			(9629 + AAC_GLOBAL_X_BEGIN_ADDR) //120 short
#define STRUCT_AACDecInfo_PSInfoBase_sfbCodeBook1_addr			(9749 + AAC_GLOBAL_X_BEGIN_ADDR) //120 short
#define STRUCT_AACDecInfo_PSInfoBase_msMaskPresent_addr			(9869 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_msMaskBits_addr			(9870 + AAC_GLOBAL_X_BEGIN_ADDR) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_pnsUsed_addr			(9878 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define STRUCT_AACDecInfo_PSInfoBase_pnsLastVal_addr			(9880 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define STRUCT_AACDecInfo_PSInfoBase_intensityUsed_addr			(9882 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
//STRUCT_AACDecInfo_PSInfoBase_pulseInfo_addr
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo_addr			(9884 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo_pulseDataPresent_addr			(0 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo_numPulse_addr			(1 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo_startSFB_addr			(2 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo_offset_addr			(3 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo_addr) //4 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo_amp_addr			(7 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo_addr) //4 short

//STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_addr
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_addr			(9895 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_pulseDataPresent_addr			(0 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_numPulse_addr			(1 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_startSFB_addr			(2 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_offset_addr			(3 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_addr) //4 short
#define STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_amp_addr			(7 + STRUCT_AACDecInfo_PSInfoBase_pulseInfo1_addr) //4 short

//STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr			(9906 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_tnsDataPresent_addr			(0 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_numFilt_addr			(1 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_coefRes_addr			(9 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_length_addr			(17 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_order_addr			(25 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_dir_addr			(33 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo_coef_addr			(41 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo_addr) //60 short

//STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr			(10007 + AAC_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_tnsDataPresent_addr			(0 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_numFilt_addr			(1 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_coefRes_addr			(9 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_length_addr			(17 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_order_addr			(25 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_dir_addr			(33 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr) //8 short
#define STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_coef_addr			(41 + STRUCT_AACDecInfo_PSInfoBase_tnsInfo1_addr) //60 short

#define LOCAL_DequantBlock_tab4_addr            			(10108 + AAC_GLOBAL_X_BEGIN_ADDR) //8 short
#define LOCAL_Dequantize_sfbTab_addr_addr       			(10116 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_Dequantize_gbCurrent_addr_addr    			(10117 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_Dequantize_numWinGroup_addr       			(10118 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_Dequantize_maxSFB_addr            			(10119 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_gbMask_addr                   			(10120 + AAC_GLOBAL_X_BEGIN_ADDR) //2 short
#define LOCAL_PNS_numWinGroup_addr              			(10122 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_checkCorr_addr                			(10123 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_genNew_addr                   			(10124 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_nSamps_addr                   			(10125 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_gbCurrent_addr_addr           			(10126 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define LOCAL_PNS_reserved_addr_addr            			(10127 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define AACDEC_BAND_INFO_ENABLE_FLAG_ADDR       			(10128 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define AACDEC_BAND_INFO_RESERVED               			(10129 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
//m_AACDEC_BAND_INFO_ADDR
#define m_AACDEC_BAND_INFO_ADDR                 			(10130 + AAC_GLOBAL_X_BEGIN_ADDR)
#define AACDEC_BAND1_INFO                       			(0 + m_AACDEC_BAND_INFO_ADDR) //2 short
#define AACDEC_BAND2_INFO                       			(2 + m_AACDEC_BAND_INFO_ADDR) //2 short
#define AACDEC_BAND3_INFO                       			(4 + m_AACDEC_BAND_INFO_ADDR) //2 short
#define AACDEC_BAND4_INFO                       			(6 + m_AACDEC_BAND_INFO_ADDR) //2 short
#define AACDEC_BAND5_INFO                       			(8 + m_AACDEC_BAND_INFO_ADDR) //2 short
#define AACDEC_BAND6_INFO                       			(10 + m_AACDEC_BAND_INFO_ADDR) //2 short
#define AACDEC_BAND7_INFO                       			(12 + m_AACDEC_BAND_INFO_ADDR) //2 short
#define AACDEC_BAND8_INFO                       			(14 + m_AACDEC_BAND_INFO_ADDR) //2 short

#define AUDIO_MIXING_AAC_ADDR                   			(10146 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define AACDEC_MIXING_RESERVED                  			(10147 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short
#define aac_support_kalaok_flag                 			(10148 + AAC_GLOBAL_X_BEGIN_ADDR) //1 short

//GLOBAL_VARS
#define GLOBAL_NON_CLIP_HISTORYDATA_L_AAC_ADDR  			(0 + AAC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_NON_CLIP_HISTORYDATA_R_AAC_ADDR  			(2 + AAC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_DIGITAL_GAIN_AAC_ADDR            			(4 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_DIGITAL_MAXVALUE_AAC_ADDR        			(5 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Output_Display_Bars_addr                			(6 + AAC_GLOBAL_Y_BEGIN_ADDR) //4 short
#define Tab_twidTabEven_addr                    			(10 + AAC_GLOBAL_Y_BEGIN_ADDR) //1008 short
#define Tab_start_addr                          			(Tab_twidTabEven_addr)
#define Tab_cos1sin1tab_addr                    			(1018 + AAC_GLOBAL_Y_BEGIN_ADDR) //1028 short
#define Tab_bitrevtab_addr                      			(2046 + AAC_GLOBAL_Y_BEGIN_ADDR) //146 short
#define Tab_invQuant3_addr                      			(2192 + AAC_GLOBAL_Y_BEGIN_ADDR) //32 short
#define Tab_invQuant4_addr                      			(2224 + AAC_GLOBAL_Y_BEGIN_ADDR) //32 short
#define Tab_sfBandTotal_addr                    			(2256 + AAC_GLOBAL_Y_BEGIN_ADDR) //12 short
#define Tab_tnsMaxBands_addr                    			(2268 + AAC_GLOBAL_Y_BEGIN_ADDR) //12 short
#define Tab_pow14_addr                          			(2280 + AAC_GLOBAL_Y_BEGIN_ADDR) //8 short
#define Tab_pow14_StereoProcess_addr            			(2288 + AAC_GLOBAL_Y_BEGIN_ADDR) //16 short
#define Tab_pow43_14_addr                       			(2304 + AAC_GLOBAL_Y_BEGIN_ADDR) //128 short
#define Tab_pow43_addr                          			(2432 + AAC_GLOBAL_Y_BEGIN_ADDR) //96 short
#define Tab_poly43lo_addr                       			(2528 + AAC_GLOBAL_Y_BEGIN_ADDR) //10 short
#define Tab_poly43hi_addr                       			(2538 + AAC_GLOBAL_Y_BEGIN_ADDR) //10 short
#define Tab_pow2exp_addr                        			(2548 + AAC_GLOBAL_Y_BEGIN_ADDR) //8 short
#define Tab_pow2frac_addr                       			(2556 + AAC_GLOBAL_Y_BEGIN_ADDR) //16 short
#define Tab_sfBandTabLongOffset_addr            			(2572 + AAC_GLOBAL_Y_BEGIN_ADDR) //12 short
#define Tab_sfBandTabLong_addr                  			(2584 + AAC_GLOBAL_Y_BEGIN_ADDR) //326 short
#define Tab_huffTabSpecInfo_addr                			(2910 + AAC_GLOBAL_Y_BEGIN_ADDR) //242 short
#define Tab_sfBandTabShortOffset_addr           			(3152 + AAC_GLOBAL_Y_BEGIN_ADDR) //12 short
#define Tab_sfBandTabShort_addr                 			(3164 + AAC_GLOBAL_Y_BEGIN_ADDR) //76 short
#define Tab_huffTabScaleFact_addr               			(3240 + AAC_GLOBAL_Y_BEGIN_ADDR) //122 short
#define Tab_huffTabScaleFactInfo_addr           			(3362 + AAC_GLOBAL_Y_BEGIN_ADDR) //22 short
#define Tab_channelMapTab_addr                  			(3384 + AAC_GLOBAL_Y_BEGIN_ADDR) //8 short
#define Tab_sampRateTab_addr                    			(3392 + AAC_GLOBAL_Y_BEGIN_ADDR) //24 short
#define Tab_elementNumChans_addr                			(3416 + AAC_GLOBAL_Y_BEGIN_ADDR) //8 short
#define const_SQRT1_2_addr                      			(3424 + AAC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define const_0x80000000_addr                   			(3426 + AAC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define const_0x7fffffff_addr                   			(3428 + AAC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define const_0_addr                            			(3430 + AAC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define const_1_addr                            			(3432 + AAC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define const_2_addr                            			(3434 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_3_addr                            			(3435 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_4_addr                            			(3436 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_5_addr                            			(3437 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_6_addr                            			(3438 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_7_addr                            			(3439 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_8_addr                            			(3440 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_9_addr                            			(3441 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_10_addr                           			(3442 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_11_addr                           			(3443 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_13_addr                           			(3444 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_14_addr                           			(3445 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_15_addr                           			(3446 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_16_addr                           			(3447 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_32_addr                           			(3448 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define const_0x0fff_addr                       			(3449 + AAC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define STRUCT_AACDecInfo_PSInfoBase_overlap1_addr			(3450 + AAC_GLOBAL_Y_BEGIN_ADDR) //2048 short
#define Tab_DMA_start_addr                      			(5498 + AAC_GLOBAL_Y_BEGIN_ADDR) //4608 short
#define Tab_DMA_start_word_addr                 			(Tab_DMA_start_addr/2)
#define Tab_huffTabSpec_addr                    			(Tab_DMA_start_addr)
#define Tab_cos4sin4tab_addr                    			(Tab_DMA_start_addr)
#define Tab_twidTabOdd_addr                     			(Tab_DMA_start_addr+2304)
#define Tab_Equalizer_long_addr                 			(Tab_twidTabOdd_addr+2016)
#define Tab_Equalizer_long_SampleFrequeyBin_Map_addr			(Tab_Equalizer_long_addr+132)
#define Tab_Equalizer_short_addr                			(Tab_Equalizer_long_SampleFrequeyBin_Map_addr+12)
#define Tab_Equalizer_short_SampleFrequeyBin_Map_addr			(Tab_Equalizer_short_addr+132)
#define Tab_sinWindow_addr                      			(Tab_DMA_start_addr)
#define Tab_kbdWindow_addr                      			(Tab_DMA_start_addr+2304)

/*****************
 **  AAC_LOCAL  **
 *****************/

/*****************
 **  RM_GLOBAL  **
 *****************/

//GLOBAL_VARS
#define RMVB_FOR_MP3_AMR_RAM_X_ALIGNED          			(0 + RM_GLOBAL_X_BEGIN_ADDR) //3244 short
//VPP_RMVB_CFG_STRUCT
#define VPP_RMVB_CFG_STRUCT                     			(3244 + RM_GLOBAL_X_BEGIN_ADDR)
#define GLOBAL_RM_RESET                         			(0 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_RM_STATUS                        			(1 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_MB_NUM                           			(2 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_RV8_FLAG                         			(3 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_PIC_WIDTH                        			(4 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_PIC_HEIGHT                       			(5 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_PITCH                            			(6 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_FILTER_PASS                      			(7 + VPP_RMVB_CFG_STRUCT) //1 short
#define GLOBAL_PARA_START_ADDR                  			(8 + VPP_RMVB_CFG_STRUCT) //2 short
#define GLOBAL_CONTEX_STORE_ADDR                			(10 + VPP_RMVB_CFG_STRUCT) //2 short


//GLOBAL_VARS
#define RMVB_FOR_MP3_AMR_RAM_Y_ALIGNED          			(0 + RM_GLOBAL_Y_BEGIN_ADDR) //7456 short
//RMVB_SHORT_Constant
#define RMVB_SHORT_Constant                     			(7456 + RM_GLOBAL_Y_BEGIN_ADDR)
#define CONST_RMVB_DEC_0x0001_ADDR              			(0 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0002_ADDR              			(1 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0003_ADDR              			(2 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0004_ADDR              			(3 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0005_ADDR              			(4 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0006_ADDR              			(5 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0007_ADDR              			(6 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0008_ADDR              			(7 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0009_ADDR              			(8 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x000A_ADDR              			(9 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x000B_ADDR              			(10 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x000C_ADDR              			(11 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x000D_ADDR              			(12 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x000E_ADDR              			(13 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x000F_ADDR              			(14 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0010_ADDR              			(15 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0014_ADDR              			(16 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0018_ADDR              			(17 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0020_ADDR              			(18 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0040_ADDR              			(19 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0100_ADDR              			(20 + RMVB_SHORT_Constant) //1 short
#define CONST_RMVB_DEC_0x0180_ADDR              			(21 + RMVB_SHORT_Constant) //1 short

//RMVB_LONG_Constant
#define RMVB_LONG_Constant                      			(7478 + RM_GLOBAL_Y_BEGIN_ADDR)
#define CONST_RMVB_DEC_0x00000001_ADDR          			(0 + RMVB_LONG_Constant) //2 short
#define CONST_RMVB_DEC_0x00000004_ADDR          			(2 + RMVB_LONG_Constant) //2 short
#define CONST_RMVB_DEC_0x00000200_ADDR          			(4 + RMVB_LONG_Constant) //2 short
#define CONST_DMA_UNSIGNB2SRD_0x10000000_ADDR   			(6 + RMVB_LONG_Constant) //2 short
#define CONST_DMA_UNSIGNB2SWR_0x50000000_ADDR   			(8 + RMVB_LONG_Constant) //2 short
#define CONST_DMA_SIGNB2SRD_0x30000000_ADDR     			(10 + RMVB_LONG_Constant) //2 short
#define CONST_DMA_SIGNB2SWR_0x70000000_ADDR     			(12 + RMVB_LONG_Constant) //2 short

//RMVB_TABLE_Dquanttab
#define RMVB_TABLE_Dquanttab                    			(7492 + RM_GLOBAL_Y_BEGIN_ADDR)
#define TABLE_dec_single_scan                   			(0 + RMVB_TABLE_Dquanttab) //16 short
#define TABLE_FILTER_DITHER                     			(16 + RMVB_TABLE_Dquanttab) //32 short


/****************
 **  RM_LOCAL  **
 ****************/

//CII_RMVBMbDec
#define CII_RMVBMbDec_X_BEGIN_ADDR              			(0 + RM_LOCAL_X_BEGIN_ADDR)
//RMVB_DATA_PARA_STRUCT
#define RMVB_DATA_PARA_STRUCT                   			(0 + CII_RMVBMbDec_X_BEGIN_ADDR)
#define INTRA16_16                              			(0 + RMVB_DATA_PARA_STRUCT) //1 short
#define INTRA_FLAG                              			(1 + RMVB_DATA_PARA_STRUCT) //1 short
#define MB_TOP                                  			(2 + RMVB_DATA_PARA_STRUCT) //1 short
#define MB_LEFT                                 			(3 + RMVB_DATA_PARA_STRUCT) //1 short
#define MB_TOPRIGHT                             			(4 + RMVB_DATA_PARA_STRUCT) //1 short
#define MB_OFFSETX                              			(5 + RMVB_DATA_PARA_STRUCT) //1 short
#define BIDIR_PRED                              			(6 + RMVB_DATA_PARA_STRUCT) //1 short
#define MB_PSKIP                                			(7 + RMVB_DATA_PARA_STRUCT) //1 short
#define RATIO_FW                                			(8 + RMVB_DATA_PARA_STRUCT) //1 short
#define RATIO_BW                                			(9 + RMVB_DATA_PARA_STRUCT) //1 short
#define INTER16                                 			(10 + RMVB_DATA_PARA_STRUCT) //1 short
#define INTER16_16                              			(11 + RMVB_DATA_PARA_STRUCT) //1 short
#define DEQUANT_Y_QP                            			(12 + RMVB_DATA_PARA_STRUCT) //1 short
#define DEQUANT_Y_QDC0                          			(13 + RMVB_DATA_PARA_STRUCT) //1 short
#define DEQUANT_Y_QDC                           			(14 + RMVB_DATA_PARA_STRUCT) //1 short
#define DEQUANT_C_QDC                           			(15 + RMVB_DATA_PARA_STRUCT) //1 short
#define DEQUANT_C_QP                            			(16 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_ALPHA                            			(17 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_BETA                             			(18 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_BETA2                            			(19 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_BETA2_C                          			(20 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_MB_CLIP0                         			(21 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_MB_CLIP2                         			(22 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_MB_CLIP                          			(23 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_MB_CLIP_LEFT                     			(24 + RMVB_DATA_PARA_STRUCT) //1 short
#define FILTER_MB_CLIP_UP                       			(25 + RMVB_DATA_PARA_STRUCT) //1 short
#define REFDIFF_UP                              			(26 + RMVB_DATA_PARA_STRUCT) //1 short
#define REFDIFF_LEFT                            			(27 + RMVB_DATA_PARA_STRUCT) //1 short
#define BNZS_UP                                 			(28 + RMVB_DATA_PARA_STRUCT) //2 short
#define BNZS_LEFT                               			(30 + RMVB_DATA_PARA_STRUCT) //2 short
#define BNZS                                    			(32 + RMVB_DATA_PARA_STRUCT) //2 short
#define BH_FILTER                               			(34 + RMVB_DATA_PARA_STRUCT) //2 short
#define BV_FILTER                               			(36 + RMVB_DATA_PARA_STRUCT) //2 short
#define H_FILTER                                			(38 + RMVB_DATA_PARA_STRUCT) //2 short
#define V_FILTER                                			(40 + RMVB_DATA_PARA_STRUCT) //2 short
#define MB_CBP                                  			(42 + RMVB_DATA_PARA_STRUCT) //2 short
#define VLD_COF                                 			(44 + RMVB_DATA_PARA_STRUCT) //400 short
#define INTRA_MODE                              			(444 + RMVB_DATA_PARA_STRUCT) //16 short
#define MOTION_VECTORS                          			(460 + RMVB_DATA_PARA_STRUCT) //16 short
#define INTERPOLATE_YBLK_ADDR                   			(476 + RMVB_DATA_PARA_STRUCT) //8 short
#define INTERPOLATE_UBLK_ADDR                   			(484 + RMVB_DATA_PARA_STRUCT) //8 short
#define INTERPOLATE_VBLK_ADDR                   			(492 + RMVB_DATA_PARA_STRUCT) //8 short
#define INTERPOLATE_YBLK_B_ADDR                 			(500 + RMVB_DATA_PARA_STRUCT) //8 short
#define INTERPOLATE_UBLK_B_ADDR                 			(508 + RMVB_DATA_PARA_STRUCT) //8 short
#define INTERPOLATE_VBLK_B_ADDR                 			(516 + RMVB_DATA_PARA_STRUCT) //8 short
#define OUT_BUFFER_Y_PTR                        			(524 + RMVB_DATA_PARA_STRUCT) //2 short
#define OUT_BUFFER_U_PTR                        			(526 + RMVB_DATA_PARA_STRUCT) //2 short
#define OUT_BUFFER_V_PTR                        			(528 + RMVB_DATA_PARA_STRUCT) //2 short

//RMVB_LOCAL_INTER_BLK
#define RMVB_LOCAL_INTER_BLK                    			(530 + CII_RMVBMbDec_X_BEGIN_ADDR)
#define LOCAL_INTERPOLATE_YBLK0                 			(0 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_YBLK1                 			(208 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_YBLK2                 			(416 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_YBLK3                 			(624 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_UBLK0                 			(832 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_UBLK1                 			(872 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_UBLK2                 			(912 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_UBLK3                 			(952 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK0                 			(992 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK1                 			(1032 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK2                 			(1072 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK3                 			(1112 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_YBLK0_B               			(1152 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_YBLK1_B               			(1360 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_YBLK2_B               			(1568 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_YBLK3_B               			(1776 + RMVB_LOCAL_INTER_BLK) //208 short
#define LOCAL_INTERPOLATE_UBLK0_B               			(1984 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_UBLK1_B               			(2024 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_UBLK2_B               			(2064 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_UBLK3_B               			(2104 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK0_B               			(2144 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK1_B               			(2184 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK2_B               			(2224 + RMVB_LOCAL_INTER_BLK) //40 short
#define LOCAL_INTERPOLATE_VBLK3_B               			(2264 + RMVB_LOCAL_INTER_BLK) //40 short

//RMVB_LOCAL_BLK_X
#define RMVB_LOCAL_BLK_X                        			(2834 + CII_RMVBMbDec_X_BEGIN_ADDR)
#define LOCAL_BLOCK14_4                         			(0 + RMVB_LOCAL_BLK_X) //32 short

//RMVB_LOCAL_PRED_BLK_X
#define RMVB_LOCAL_PRED_BLK_X                   			(2866 + CII_RMVBMbDec_X_BEGIN_ADDR)
#define RMVB_LOCAL_MB_TOP                       			(0 + RMVB_LOCAL_PRED_BLK_X) //1 short
#define RMVB_LOCAL_MB_LEFT                      			(1 + RMVB_LOCAL_PRED_BLK_X) //1 short
#define RMVB_LOCAL_LEFT_ARRAY                   			(2 + RMVB_LOCAL_PRED_BLK_X) //8 short
#define FILTER_YLEFT                            			(10 + RMVB_LOCAL_PRED_BLK_X) //64 short
#define FILTER_YUP                              			(74 + RMVB_LOCAL_PRED_BLK_X) //64 short
#define FILTER_ULEFT                            			(138 + RMVB_LOCAL_PRED_BLK_X) //32 short
#define FILTER_UUP                              			(170 + RMVB_LOCAL_PRED_BLK_X) //32 short
#define FILTER_VLEFT                            			(202 + RMVB_LOCAL_PRED_BLK_X) //32 short
#define FILTER_VUP                              			(234 + RMVB_LOCAL_PRED_BLK_X) //32 short
#define YINTRA_UP_ARRAY                         			(266 + RMVB_LOCAL_PRED_BLK_X) //24 short
#define YINTRA_LEFT_ARRAY                       			(290 + RMVB_LOCAL_PRED_BLK_X) //24 short
#define UINTRA_UP_ARRAY                         			(314 + RMVB_LOCAL_PRED_BLK_X) //16 short
#define UINTRA_LEFT_ARRAY                       			(330 + RMVB_LOCAL_PRED_BLK_X) //16 short
#define VINTRA_UP_ARRAY                         			(346 + RMVB_LOCAL_PRED_BLK_X) //16 short
#define VINTRA_LEFT_ARRAY                       			(362 + RMVB_LOCAL_PRED_BLK_X) //16 short
#define Y_ROW_ARRAY                             			(378 + RMVB_LOCAL_PRED_BLK_X) //400 short
#define U_ROW_ARRAY                             			(778 + RMVB_LOCAL_PRED_BLK_X) //200 short
#define V_ROW_ARRAY                             			(978 + RMVB_LOCAL_PRED_BLK_X) //200 short
#define YINTRA_UPLEFT                           			(1178 + RMVB_LOCAL_PRED_BLK_X) //1 short
#define UINTRA_UPLEFT                           			(1179 + RMVB_LOCAL_PRED_BLK_X) //1 short
#define VINTRA_UPLEFT                           			(1180 + RMVB_LOCAL_PRED_BLK_X) //1 short
#define LOCAL_STUFF                             			(1181 + RMVB_LOCAL_PRED_BLK_X) //1 short

//RMVB_PARA_ADDR_STRUCT
#define RMVB_PARA_ADDR_STRUCT                   			(4048 + CII_RMVBMbDec_X_BEGIN_ADDR)
#define VLD_COF_ADDR                            			(0 + RMVB_PARA_ADDR_STRUCT) //1 short
#define INTRA_MODE_ADDR                         			(1 + RMVB_PARA_ADDR_STRUCT) //1 short
#define UVINTRA_UP_ARRAY_ADDR                   			(2 + RMVB_PARA_ADDR_STRUCT) //1 short
#define UVINTRA_LEFT_ARRAY_ADDR                 			(3 + RMVB_PARA_ADDR_STRUCT) //1 short
#define UV_VLD_COF_ADDR                         			(4 + RMVB_PARA_ADDR_STRUCT) //1 short
#define UV_PRED_BLK_ADDR                        			(5 + RMVB_PARA_ADDR_STRUCT) //1 short

//RMVB_LOCAL_PARA_STRUCT
#define RMVB_LOCAL_PARA_STRUCT                  			(4054 + CII_RMVBMbDec_X_BEGIN_ADDR)
#define PITCHX4                                 			(0 + RMVB_LOCAL_PARA_STRUCT) //2 short
#define DOUBLE_FRM                              			(2 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define UV_MB_CBP                               			(3 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define LOCAL_EDGEBIT                           			(4 + RMVB_LOCAL_PARA_STRUCT) //2 short
#define LOCAL_LEFT_EDGEBIT                      			(6 + RMVB_LOCAL_PARA_STRUCT) //2 short
#define LOCAL_UP_EDGEBIT                        			(8 + RMVB_LOCAL_PARA_STRUCT) //2 short
#define LOCAL_LOW_EDGEBIT                       			(10 + RMVB_LOCAL_PARA_STRUCT) //2 short
#define LOCAL_FILTER_EDGE0_UP                   			(12 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define LOCAL_FILTER_EDGE0                      			(13 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define LOCAL_FILTER_EDGE0_LEFT                 			(14 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_CHROMA                           			(15 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define LOCAL_FILTER_EDGE4_UP                   			(16 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define LOCAL_FILTER_EDGE4_DOWN                 			(17 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_DITHER_PT                        			(18 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define LOCAL_NO_FILTER_EDGE4                   			(19 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_BLK_CLIP                         			(20 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_BLK_CLIP_HVAL                    			(21 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_BLK_CLIP_VVAL                    			(22 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_BLK_CLIP_LEFT                    			(23 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_PITCH                            			(24 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_STRONG                           			(25 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_CR                               			(26 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_CL                               			(27 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_AR                               			(28 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define FILTER_AL                               			(29 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET4                      			(30 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET5                      			(31 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET6                      			(32 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET7                      			(33 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET8                      			(34 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET9                      			(35 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET10                     			(36 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET11                     			(37 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET12                     			(38 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET13                     			(39 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET14                     			(40 + RMVB_LOCAL_PARA_STRUCT) //1 short
#define INTRA_PRED_OFFSET15                     			(41 + RMVB_LOCAL_PARA_STRUCT) //1 short


//CII_RMVBMbDec
#define CII_RMVBMbDec_Y_BEGIN_ADDR              			(0 + RM_LOCAL_Y_BEGIN_ADDR)
//RMVB_LOCAL_BLK_Y
#define RMVB_LOCAL_BLK_Y                        			(0 + CII_RMVBMbDec_Y_BEGIN_ADDR)
#define LOCAL_BLOCK4_4                          			(0 + RMVB_LOCAL_BLK_Y) //16 short
#define LOCAL_BLOCKN4_4                         			(16 + RMVB_LOCAL_BLK_Y) //32 short

//RMVB_LOCAL_PRED_BLK_Y
#define RMVB_LOCAL_PRED_BLK_Y                   			(48 + CII_RMVBMbDec_Y_BEGIN_ADDR)
#define RMVB_LOCAL_PRED_BLKY                    			(0 + RMVB_LOCAL_PRED_BLK_Y) //256 short
#define RMVB_LOCAL_PRED_BLKUV                   			(256 + RMVB_LOCAL_PRED_BLK_Y) //128 short
#define RMVB_LOCAL_PRED_BLKY_B                  			(384 + RMVB_LOCAL_PRED_BLK_Y) //256 short
#define RMVB_LOCAL_PRED_BLKUV_B                 			(640 + RMVB_LOCAL_PRED_BLK_Y) //128 short
#define RMVB_LOCAL_PRED_TBUFF                   			(768 + RMVB_LOCAL_PRED_BLK_Y) //336 short
#define RMVB_LOCAL_PIXEL_TMP0                   			(1104 + RMVB_LOCAL_PRED_BLK_Y) //3 short
#define RMVB_LOCAL_PIXEL_TMP1                   			(1107 + RMVB_LOCAL_PRED_BLK_Y) //3 short
#define RMVB_LOCAL_PIXEL_UPLEFT                 			(1110 + RMVB_LOCAL_PRED_BLK_Y) //2 short
#define RMVB_LOCAL_UP_ARRAY_POINT               			(1112 + RMVB_LOCAL_PRED_BLK_Y) //1 short
#define RMVB_LOCAL_LEFT_ARRAY_POINT             			(1113 + RMVB_LOCAL_PRED_BLK_Y) //1 short


/******************
 **  MP3_GLOBAL  **
 ******************/

//GLOBAL_VARS
#define GLOBAL_INPUT0_DATA_ADDR                 			(0 + MP3_GLOBAL_X_BEGIN_ADDR) //1024 short
#define CONST_0x00000800_ADDR                   			(1024 + MP3_GLOBAL_X_BEGIN_ADDR) //2 short
#define TABLE_RAM_X_BEGINE_ADDR                 			(CONST_0x00000800_ADDR)
#define CONST_neg1_ADDR                         			(1026 + MP3_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_0xe0ff_ADDR                       			(1027 + MP3_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_1152_ADDR                         			(1028 + MP3_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_17_ADDR                           			(1029 + MP3_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_576_ADDR                          			(1030 + MP3_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_1024_ADDR                         			(1031 + MP3_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_2048_ADDR                         			(CONST_0x00000800_ADDR)
#define TABLE_hufftab0_ADDR                     			(1032 + MP3_GLOBAL_X_BEGIN_ADDR) //2 short
#define TABLE_hufftab1_ADDR                     			(1034 + MP3_GLOBAL_X_BEGIN_ADDR) //8 short
#define TABLE_hufftab2_ADDR                     			(1042 + MP3_GLOBAL_X_BEGIN_ADDR) //16 short
#define TABLE_hufftab3_ADDR                     			(1058 + MP3_GLOBAL_X_BEGIN_ADDR) //16 short
#define TABLE_hufftab5_ADDR                     			(1074 + MP3_GLOBAL_X_BEGIN_ADDR) //26 short
#define TABLE_hufftab6_ADDR                     			(1100 + MP3_GLOBAL_X_BEGIN_ADDR) //28 short
#define TABLE_hufftab7_ADDR                     			(1128 + MP3_GLOBAL_X_BEGIN_ADDR) //64 short
#define TABLE_hufftab8_ADDR                     			(1192 + MP3_GLOBAL_X_BEGIN_ADDR) //66 short
#define TABLE_hufftab9_ADDR                     			(1258 + MP3_GLOBAL_X_BEGIN_ADDR) //54 short
#define TABLE_hufftab10_ADDR                    			(1312 + MP3_GLOBAL_X_BEGIN_ADDR) //102 short
#define TABLE_hufftab11_ADDR                    			(1414 + MP3_GLOBAL_X_BEGIN_ADDR) //104 short
#define TABLE_hufftab12_ADDR                    			(1518 + MP3_GLOBAL_X_BEGIN_ADDR) //96 short
#define TABLE_hufftab13_ADDR                    			(1614 + MP3_GLOBAL_X_BEGIN_ADDR) //398 short
#define TABLE_hufftab15_ADDR                    			(2012 + MP3_GLOBAL_X_BEGIN_ADDR) //380 short
#define TABLE_hufftab16_ADDR                    			(2392 + MP3_GLOBAL_X_BEGIN_ADDR) //402 short
#define TABLE_hufftab24_ADDR                    			(2794 + MP3_GLOBAL_X_BEGIN_ADDR) //386 short
#define TABLE_mad_huff_pair_table_ADDR          			(3180 + MP3_GLOBAL_X_BEGIN_ADDR) //64 short
#define OUTPUT_PCM_BUFFER1_ADDR                 			(3244 + MP3_GLOBAL_X_BEGIN_ADDR) //2304 short
#define OUTPUT_PCM_BUFFER2_ADDR                 			((1152+OUTPUT_PCM_BUFFER1_ADDR))
#define STATIC_MAD_SBSAMPLE_ADDR                			(5548 + MP3_GLOBAL_X_BEGIN_ADDR) //4608 short
#define GLOBAL_NON_CLIP_CROSSZEROINDEX_MP3      			((STATIC_MAD_SBSAMPLE_ADDR+2304))
#define GLOBAL_NON_CLIP_TEMP_BUFFER_MP3         			(STATIC_MAD_SBSAMPLE_ADDR)
#define LOCAL_X_START_ADDR                      			((4608+STATIC_MAD_SBSAMPLE_ADDR))
#define LOCAL_BX_ADDR                           			(LOCAL_X_START_ADDR)

//GLOBAL_VARS
#define GLOBAL_NON_CLIP_HISTORYDATA_L_MP3_ADDR  			(0 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_NON_CLIP_HISTORYDATA_R_MP3_ADDR  			(2 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_DIGITAL_GAIN_MP3_ADDR            			(4 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_DIGITAL_MAXVALUE_MP3_ADDR        			(5 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define CONST_reseverd_ADDR                     			(6 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
#define TABLE_hufftabA_ADDR                     			(8 + MP3_GLOBAL_Y_BEGIN_ADDR) //28 short
#define TABLE_RAM_Y_BEGINE_ADDR                 			(TABLE_hufftabA_ADDR)
#define TABLE_imdct36_y_tab_ADDR                			(36 + MP3_GLOBAL_Y_BEGIN_ADDR) //18 short
#define TABLE_D_ADDR                            			(54 + MP3_GLOBAL_Y_BEGIN_ADDR) //544 short
#define TABLE_hufftabB_ADDR                     			(598 + MP3_GLOBAL_Y_BEGIN_ADDR) //16 short
#define TABLE_sflen_table_ADDR                  			(614 + MP3_GLOBAL_Y_BEGIN_ADDR) //32 short
#define TABLE_nsfb_table_ADDR                   			(646 + MP3_GLOBAL_Y_BEGIN_ADDR) //72 short
#define TABLE_pretab_ADDR                       			(718 + MP3_GLOBAL_Y_BEGIN_ADDR) //22 short
#define TABLE_cs_ADDR                           			(740 + MP3_GLOBAL_Y_BEGIN_ADDR) //8 short
#define TABLE_ca_ADDR                           			(748 + MP3_GLOBAL_Y_BEGIN_ADDR) //8 short
#define TABLE_imdct_s_ADDR                      			(756 + MP3_GLOBAL_Y_BEGIN_ADDR) //36 short
#define TABLE_window_l_ADDR                     			(792 + MP3_GLOBAL_Y_BEGIN_ADDR) //36 short
#define TABLE_window_s_ADDR                     			(828 + MP3_GLOBAL_Y_BEGIN_ADDR) //12 short
#define TABLE_is_table_ADDR                     			(840 + MP3_GLOBAL_Y_BEGIN_ADDR) //8 short
#define TABLE_is_lsf_table_ADDR                 			(848 + MP3_GLOBAL_Y_BEGIN_ADDR) //30 short
#define TABLE_bitrate_table_ADDR                			(878 + MP3_GLOBAL_Y_BEGIN_ADDR) //80 short
#define TABLE_imdct36_tab_ADDR                  			(958 + MP3_GLOBAL_Y_BEGIN_ADDR) //188 short
#define TABLE_dct32_tab_ADDR                    			(1146 + MP3_GLOBAL_Y_BEGIN_ADDR) //32 short
#define TABLE_imdct36_x_tab_ADDR                			(1178 + MP3_GLOBAL_Y_BEGIN_ADDR) //36 short
#define TABLE_sfb_48000_long_ADDR               			(1214 + MP3_GLOBAL_Y_BEGIN_ADDR) //22 short
#define TABLE_sfb_44100_long_ADDR               			(1236 + MP3_GLOBAL_Y_BEGIN_ADDR) //22 short
#define TABLE_sfb_32000_long_ADDR               			(1258 + MP3_GLOBAL_Y_BEGIN_ADDR) //22 short
#define TABLE_sfb_48000_short_ADDR              			(1280 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfb_44100_short_ADDR              			(1320 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfb_32000_short_ADDR              			(1360 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfb_48000_mixed_ADDR              			(1400 + MP3_GLOBAL_Y_BEGIN_ADDR) //38 short
#define TABLE_sfb_44100_mixed_ADDR              			(1438 + MP3_GLOBAL_Y_BEGIN_ADDR) //38 short
#define TABLE_sfb_32000_mixed_ADDR              			(1476 + MP3_GLOBAL_Y_BEGIN_ADDR) //38 short
#define TABLE_sfb_24000_long_ADDR               			(1514 + MP3_GLOBAL_Y_BEGIN_ADDR) //22 short
#define TABLE_sfb_22050_long_ADDR               			(1536 + MP3_GLOBAL_Y_BEGIN_ADDR) //22 short
#define TABLE_sfb_24000_short_ADDR              			(1558 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfb_22050_short_ADDR              			(1598 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfb_16000_short_ADDR              			(1638 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfb_24000_mixed_ADDR              			(1678 + MP3_GLOBAL_Y_BEGIN_ADDR) //36 short
#define TABLE_sfb_22050_mixed_ADDR              			(1714 + MP3_GLOBAL_Y_BEGIN_ADDR) //36 short
#define TABLE_sfb_16000_mixed_ADDR              			(1750 + MP3_GLOBAL_Y_BEGIN_ADDR) //36 short
#define TABLE_sfb_8000_long_ADDR                			(1786 + MP3_GLOBAL_Y_BEGIN_ADDR) //22 short
#define TABLE_sfb_8000_short_ADDR               			(1808 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfb_8000_mixed_ADDR               			(1848 + MP3_GLOBAL_Y_BEGIN_ADDR) //40 short
#define TABLE_sfbwidth_table_ADDR               			(1888 + MP3_GLOBAL_Y_BEGIN_ADDR) //28 short
#define TABLE_sfb_16000_long_ADDR               			(TABLE_sfb_22050_long_ADDR)
#define TABLE_sfb_12000_long_ADDR               			(TABLE_sfb_16000_long_ADDR)
#define TABLE_sfb_11025_long_ADDR               			(TABLE_sfb_12000_long_ADDR)
#define TABLE_sfb_12000_short_ADDR              			(TABLE_sfb_16000_short_ADDR)
#define TABLE_sfb_11025_short_ADDR              			(TABLE_sfb_12000_short_ADDR)
#define TABLE_sfb_12000_mixed_ADDR              			(TABLE_sfb_16000_mixed_ADDR)
#define TABLE_sfb_11025_mixed_ADDR              			(TABLE_sfb_12000_mixed_ADDR)
#define CONST_2_ADDR                            			((26 + TABLE_imdct36_x_tab_ADDR))
#define CONST_4_ADDR                            			((30 + TABLE_imdct36_x_tab_ADDR))
#define CONST_6_ADDR                            			((32 + TABLE_imdct36_x_tab_ADDR))
#define CONST_8_ADDR                            			((24 + TABLE_imdct36_x_tab_ADDR))
#define CONST_10_ADDR                           			((12 + TABLE_imdct36_x_tab_ADDR))
#define CONST_12_ADDR                           			((6 + TABLE_imdct36_x_tab_ADDR))
#define CONST_14_ADDR                           			((0 + TABLE_imdct36_x_tab_ADDR))
#define CONST_16_ADDR                           			((18 + TABLE_imdct36_x_tab_ADDR))
#define CONST_20_ADDR                           			((1 + TABLE_imdct36_x_tab_ADDR))
#define CONST_28_ADDR                           			((33 + TABLE_imdct36_x_tab_ADDR))
#define CONST_30_ADDR                           			((31 + TABLE_imdct36_x_tab_ADDR))
#define CONST_32_ADDR                           			((27 + TABLE_imdct36_x_tab_ADDR))
#define CONST_36_ADDR                           			((10 + TABLE_imdct36_x_tab_ADDR))
#define CONST_3_ADDR                            			((15 + TABLE_nsfb_table_ADDR))
#define CONST_7_ADDR                            			((14 + TABLE_nsfb_table_ADDR))
#define CONST_15_ADDR                           			((32+ TABLE_nsfb_table_ADDR))
#define CONST_0_WORD32_ADDR                     			((26 + TABLE_nsfb_table_ADDR))
#define CONST_0_ADDR                            			(CONST_0_WORD32_ADDR)
#define CONST_0x0010_ADDR                       			(CONST_16_ADDR)
#define CONST_0x0020_ADDR                       			(CONST_32_ADDR)
#define CONST_1_WORD32_ADDR                     			(TABLE_hufftab0_ADDR)
#define CONST_1_ADDR                            			(CONST_1_WORD32_ADDR)
#define CONST_0x0011_ADDR                       			(CONST_17_ADDR)
#define FOR_DMA_4INT_ALIGNED                    			(1916 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
#define STRUCT_left_dither_error_ADDR           			(1918 + MP3_GLOBAL_Y_BEGIN_ADDR) //6 short
#define STRUCT_right_dither_error_ADDR          			(1924 + MP3_GLOBAL_Y_BEGIN_ADDR) //6 short
#define STRUCT_stats_peak_clipping_ADDR         			(1930 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
#define STRUCT_stats_peak_sample_ADDR           			(1932 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
//STRUCT_MAD_HEADER_ADDR
#define STRUCT_MAD_HEADER_ADDR                  			(1934 + MP3_GLOBAL_Y_BEGIN_ADDR)
#define STRUCT_MAD_HEADER_LAYER_ADDR            			(0 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_MODE_ADDR             			(1 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_MODE_EXTENSION_ADDR   			(2 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_EMPHASIS_ADDR         			(3 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_BITRATE_ADDR          			(4 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_SAMPLERATE_ADDR       			(5 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_CRC_CHECK_ADDR        			(6 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_CRC_TARGET_ADDR       			(7 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_FLAGS_ADDR            			(8 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_HEADER_PRIVATE_BITS_ADDR     			(9 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_TIMER_DURATION_SECONDS_ADDR  			(10 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_TIMER_DURATION_FRACTION_ADDR 			(11 + STRUCT_MAD_HEADER_ADDR) //1 short
#define STRUCT_MAD_FRAME_OPTIONS_ADDR           			(12 + STRUCT_MAD_HEADER_ADDR) //1 short
#define GLOBAL_MAD_SYNTH_PHASE_ADDR             			(13 + STRUCT_MAD_HEADER_ADDR) //1 short

//STRUCT_MAD_STREAM_ADDR
#define STRUCT_MAD_STREAM_ADDR                  			(1948 + MP3_GLOBAL_Y_BEGIN_ADDR)
#define STRUCT_MAD_STREAM_BUFFER_ADDR_ADDR      			(0 + STRUCT_MAD_STREAM_ADDR) //2 short
#define STRUCT_MAD_STREAM_SKIPLEN_ADDR          			(2 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_SYNC_ADDR             			(3 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_FREERATE_ADDR         			(4 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_THIS_FRAME_ADDR_ADDR  			(5 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_NEXT_FRAME_ADDR_ADDR  			(6 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_BITPTR_PTR_BYTE_ADDR_ADDR        			(7 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_BITPTR_PTR_CACHE_ADDR            			(8 + STRUCT_MAD_STREAM_ADDR) //2 short
#define STRUCT_BITPTR_PTR_LEFT_ADDR             			(10 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_BITPTR_ANC_PTR_BYTE_ADDR_ADDR    			(11 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_BITPTR_ANC_PTR_CACHE_ADDR        			(12 + STRUCT_MAD_STREAM_ADDR) //2 short
#define STRUCT_BITPTR_ANC_PTR_LEFT_ADDR         			(14 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_ANC_BITLEN_ADDR       			(15 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_MD_LEN_ADDR           			(16 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_OPTIONS_ADDR          			(17 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_MAD_STREAM_MAD_ERROR_ADDR        			(18 + STRUCT_MAD_STREAM_ADDR) //1 short
#define GLOBLE_WRITE_POINTER_ADDR               			(19 + STRUCT_MAD_STREAM_ADDR) //1 short
#define GLOBAL_NCH_ADDR                         			(20 + STRUCT_MAD_STREAM_ADDR) //1 short
#define GLOBAL_NS_ADDR                          			(21 + STRUCT_MAD_STREAM_ADDR) //1 short
#define GLOBAL_NGR_ADDR                         			(22 + STRUCT_MAD_STREAM_ADDR) //1 short
#define GLOBAL_WRAP_INDEX_ADDR                  			(23 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_SIDEINFO_ADDR                    			(24 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_SIDEINFO_main_data_begin_ADDR    			(STRUCT_SIDEINFO_ADDR)
#define STRUCT_SIDEINFO_private_bits_ADDR       			(25 + STRUCT_MAD_STREAM_ADDR) //1 short
#define STRUCT_SIDEINFO_scfsi_ADDR              			(26 + STRUCT_MAD_STREAM_ADDR) //2 short

//STRUCT_GRANULE0_BEGIN_ADDR
#define STRUCT_GRANULE0_BEGIN_ADDR              			(1976 + MP3_GLOBAL_Y_BEGIN_ADDR)
#define STRUCT_GRANULE0CH0_part2_3_length_ADDR  			(0 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_big_values_ADDR      			(1 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_global_gain_ADDR     			(2 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_scalefac_compress_ADDR			(3 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_flags_ADDR           			(4 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_block_type_ADDR      			(5 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_table_select_ADDR    			(6 + STRUCT_GRANULE0_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE0CH0_subblock_gain_ADDR   			(9 + STRUCT_GRANULE0_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE0CH0_region0_count_ADDR   			(12 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_region1_count_ADDR   			(13 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH0_scalefac_ADDR        			(14 + STRUCT_GRANULE0_BEGIN_ADDR) //40 short
#define STRUCT_GRANULE0CH1_part2_3_length_ADDR  			(54 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_big_values_ADDR      			(55 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_global_gain_ADDR     			(56 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_scalefac_compress_ADDR			(57 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_flags_ADDR           			(58 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_block_type_ADDR      			(59 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_table_select_ADDR    			(60 + STRUCT_GRANULE0_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE0CH1_subblock_gain_ADDR   			(63 + STRUCT_GRANULE0_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE0CH1_region0_count_ADDR   			(66 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_region1_count_ADDR   			(67 + STRUCT_GRANULE0_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE0CH1_scalefac_ADDR        			(68 + STRUCT_GRANULE0_BEGIN_ADDR) //40 short

//STRUCT_GRANULE1_BEGIN_ADDR
#define STRUCT_GRANULE1_BEGIN_ADDR              			(2084 + MP3_GLOBAL_Y_BEGIN_ADDR)
#define STRUCT_GRANULE1CH0_part2_3_length_ADDR  			(0 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_big_values_ADDR      			(1 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_global_gain_ADDR     			(2 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_scalefac_compress_ADDR			(3 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_flags_ADDR           			(4 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_block_type_ADDR      			(5 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_table_select_ADDR    			(6 + STRUCT_GRANULE1_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE1CH0_subblock_gain_ADDR   			(9 + STRUCT_GRANULE1_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE1CH0_region0_count_ADDR   			(12 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_region1_count_ADDR   			(13 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH0_scalefac_ADDR        			(14 + STRUCT_GRANULE1_BEGIN_ADDR) //40 short
#define STRUCT_GRANULE1CH1_part2_3_length_ADDR  			(54 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_big_values_ADDR      			(55 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_global_gain_ADDR     			(56 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_scalefac_compress_ADDR			(57 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_flags_ADDR           			(58 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_block_type_ADDR      			(59 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_table_select_ADDR    			(60 + STRUCT_GRANULE1_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE1CH1_subblock_gain_ADDR   			(63 + STRUCT_GRANULE1_BEGIN_ADDR) //3 short
#define STRUCT_GRANULE1CH1_region0_count_ADDR   			(66 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_region1_count_ADDR   			(67 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define STRUCT_GRANULE1CH1_scalefac_ADDR        			(68 + STRUCT_GRANULE1_BEGIN_ADDR) //40 short
#define PTR_CACHE_ADDR                          			(108 + STRUCT_GRANULE1_BEGIN_ADDR) //2 short
#define PTR_BYTE_ADDR_ADDR                      			(110 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define PTR_LEFT_ADDR                           			(111 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define mad_layer_III_result                    			(112 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define GLOBAL_MAIN_DATA_OFFSET_ADDR            			(113 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define DEC_DMA_DATA_SINGLE                     			(114 + STRUCT_GRANULE1_BEGIN_ADDR) //2 short
#define mp123_support_kalaok_flag               			(116 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define GLOBAL_NON_CLIP_FLAG                    			(117 + STRUCT_GRANULE1_BEGIN_ADDR) //1 short
#define AUDIO_MIXING_MP3_ADDR                   			(118 + STRUCT_GRANULE1_BEGIN_ADDR) //2 short

#define STATIC_MAD_OVERLAP_ADDR                 			(2204 + MP3_GLOBAL_Y_BEGIN_ADDR) //2304 short
#define GLOBAL_MAD_SYNTH_FILTER_ADDR            			(4508 + MP3_GLOBAL_Y_BEGIN_ADDR) //2048 short
#define STATIC_MAD_MAIN_DATA_ADDR               			(6556 + MP3_GLOBAL_Y_BEGIN_ADDR) //900 short
#define FOR_EQ_MODEL_ADDR                       			(7456 + MP3_GLOBAL_Y_BEGIN_ADDR) //308 short
#define MP3DEC_BAND_INFO_ENABLE_FLAG_ADDR       			(7764 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3DEC_BAND_INFO_RESERVED               			(7765 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
//m_MP3DEC_BAND_INFO_ADDR
#define m_MP3DEC_BAND_INFO_ADDR                 			(7766 + MP3_GLOBAL_Y_BEGIN_ADDR)
#define MP3DEC_BAND1_INFO                       			(0 + m_MP3DEC_BAND_INFO_ADDR) //2 short
#define MP3DEC_BAND2_INFO                       			(2 + m_MP3DEC_BAND_INFO_ADDR) //2 short
#define MP3DEC_BAND3_INFO                       			(4 + m_MP3DEC_BAND_INFO_ADDR) //2 short
#define MP3DEC_BAND4_INFO                       			(6 + m_MP3DEC_BAND_INFO_ADDR) //2 short
#define MP3DEC_BAND5_INFO                       			(8 + m_MP3DEC_BAND_INFO_ADDR) //2 short
#define MP3DEC_BAND6_INFO                       			(10 + m_MP3DEC_BAND_INFO_ADDR) //2 short
#define MP3DEC_BAND7_INFO                       			(12 + m_MP3DEC_BAND_INFO_ADDR) //2 short
#define MP3DEC_BAND8_INFO                       			(14 + m_MP3DEC_BAND_INFO_ADDR) //2 short

#define FOR_MP3dec_DRC_Previous_Frame_ADDR      			(7782 + MP3_GLOBAL_Y_BEGIN_ADDR) //1152 short
#define FOR_DRC_RAM_Y_ALIGNED                   			((FOR_MP3dec_DRC_Previous_Frame_ADDR+1152))
//MP3_DEC_GLOBAL_STRUCT_DRC_PARAM
#define MP3_DEC_GLOBAL_STRUCT_DRC_PARAM         			(8934 + MP3_GLOBAL_Y_BEGIN_ADDR)
#define MP3_DEC_DRC_G_Y_alc_enable              			(0 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_thres             			(1 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_width             			(2 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_R                 			(3 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_R1                			(4 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_R2                			(5 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_limit             			(6 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_M                 			(7 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_alpha1            			(8 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_alpha2            			(9 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_noise_gate        			(10 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_alpha_max         			(11 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_Thr_dB            			(12 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_mm_gain           			(13 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_param_channel           			(14 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define MP3_DEC_DRC_G_Y_reserved                			(15 + MP3_DEC_GLOBAL_STRUCT_DRC_PARAM) //1 short

#define MP3_DEC_DRC_G_Y_alg_gain                			(8950 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3_DEC_DRC_G_Y_crosszero_window        			(8951 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3_DEC_DRC_G_Y_peak_last1              			(8952 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
#define MP3_DEC_DRC_G_Y_peak_last2              			(8954 + MP3_GLOBAL_Y_BEGIN_ADDR) //2 short
#define MP3_DEC_DRC_G_Y_history_peak1           			(8956 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3_DEC_DRC_G_Y_history_peak2           			(8957 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3_DEC_DRC_G_Y_crosszero_offset1       			(8958 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3_DEC_DRC_G_Y_crosszero_offset2       			(8959 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3_DEC_DRC_G_Y_gain_smooth1            			(8960 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define MP3_DEC_DRC_G_Y_gain_smooth2            			(8961 + MP3_GLOBAL_Y_BEGIN_ADDR) //1 short
#define FOR_JEPG_DEC_RAM_Y_ALIGNED              			(8962 + MP3_GLOBAL_Y_BEGIN_ADDR) //610 short
#define LOCAL_Y_START_ADDR                      			((610+ FOR_JEPG_DEC_RAM_Y_ALIGNED))
#define TABLE_offset_table_ADDR                 			(FOR_JEPG_DEC_RAM_Y_ALIGNED)
#define TABLE_linear_table_ADDR                 			((90 + TABLE_offset_table_ADDR))
#define TABLE_sf_table_ADDR                     			((14 + TABLE_linear_table_ADDR))
#define TABLE_sbquant_table_ADDR                			((64 + TABLE_sf_table_ADDR))
#define TABLE_bitalloc_table_ADDR               			((160*2 + TABLE_sbquant_table_ADDR))
#define TABLE_qc_table_ADDR                     			((16 + TABLE_bitalloc_table_ADDR))
#define LOCAL_AX_ADDR                           			(LOCAL_Y_START_ADDR)

/*****************
 **  MP3_LOCAL  **
 *****************/

//CII_mad_synth_frame
#define CII_mad_synth_frame_Y_BEGIN_ADDR        			(0 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_mad_synth_frame_addr              			(0 + CII_mad_synth_frame_Y_BEGIN_ADDR) //64 short
#define LOCAL_output_addr_addr                  			(64 + CII_mad_synth_frame_Y_BEGIN_ADDR) //1 short
#define LOCAL_output_raw_addr_addr              			(65 + CII_mad_synth_frame_Y_BEGIN_ADDR) //1 short

//CII_mad_layer_II
#define CII_mad_layer_II_Y_BEGIN_ADDR           			(0 + MP3_LOCAL_Y_BEGIN_ADDR)
#define layer_II_allocation_addr                			(0 + CII_mad_layer_II_Y_BEGIN_ADDR) //64 short
#define layer_II_scfsi_addr                     			(64 + CII_mad_layer_II_Y_BEGIN_ADDR) //64 short
#define layer_II_scalefactor_addr               			(128 + CII_mad_layer_II_Y_BEGIN_ADDR) //192 short
#define TABLE_bitalloc_table_ADDR_ADDR          			(320 + CII_mad_layer_II_Y_BEGIN_ADDR) //2 short
#define layer_II_BACK_JUMP1_addr                			(322 + CII_mad_layer_II_Y_BEGIN_ADDR) //1 short
#define layer_II_BACK_JUMP_addr                 			(323 + CII_mad_layer_II_Y_BEGIN_ADDR) //1 short

//CII_dct32
#define CII_dct32_X_BEGIN_ADDR                  			(0 + MP3_LOCAL_X_BEGIN_ADDR)
#define LOCAL_X_DCT32_TX1_T_ADDR                			(0 + CII_dct32_X_BEGIN_ADDR) //2 short
#define LOCAL_X_DCT32_TX1_ADDR                  			(2 + CII_dct32_X_BEGIN_ADDR) //32 short
#define LOCAL_X_DCT32_TX2_T_ADDR                			(34 + CII_dct32_X_BEGIN_ADDR) //2 short
#define LOCAL_X_DCT32_TX2_ADDR                  			(36 + CII_dct32_X_BEGIN_ADDR) //32 short

//CII_dct32
#define CII_dct32_Y_BEGIN_ADDR                  			(66 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_Y_DCT32_TY2_T_ADDR                			(0 + CII_dct32_Y_BEGIN_ADDR) //2 short
#define LOCAL_Y_DCT32_TY2_ADDR                  			(2 + CII_dct32_Y_BEGIN_ADDR) //32 short
#define LOCAL_X_DCT32_IN_ADDR                   			(34 + CII_dct32_Y_BEGIN_ADDR) //34 short
#define LOCAL_Y_DCT32_TX1_T_ADDR                			(68 + CII_dct32_Y_BEGIN_ADDR) //2 short
#define LOCAL_Y_DCT32_TY1_ADDR                  			(70 + CII_dct32_Y_BEGIN_ADDR) //32 short
#define LOCAL_Y_PHASE_ADDR                      			(102 + CII_dct32_Y_BEGIN_ADDR) //2 short
#define CONST_DCT32_A24_ADDR_ADDR               			(104 + CII_dct32_Y_BEGIN_ADDR) //2 short
#define LOCAL_X_DCT32_A_ADDR                    			(STRUCT_SIDEINFO_ADDR)
#define LOCAL_DCT32_A24_ADDR                    			((46+LOCAL_X_DCT32_A_ADDR))

//CII_II_samples
#define CII_II_samples_Y_BEGIN_ADDR             			(324 + MP3_LOCAL_Y_BEGIN_ADDR)
#define layer_II_samples_addr                   			(0 + CII_II_samples_Y_BEGIN_ADDR) //6 short
#define layer_II_TEMP_ADDR                      			(6 + CII_II_samples_Y_BEGIN_ADDR) //2 short
#define TABLE_sf_table_ADDR_ADDR                			(8 + CII_II_samples_Y_BEGIN_ADDR) //1 short
#define layer_II_scalefactor_ADDR_ADDR          			(9 + CII_II_samples_Y_BEGIN_ADDR) //1 short
#define layer_II_SBSAMPLE_SFTABLE_ADDR          			(10 + CII_II_samples_Y_BEGIN_ADDR) //1 short
#define layer_II_SFTABLE_ADDR                   			(11 + CII_II_samples_Y_BEGIN_ADDR) //1 short
#define layer_II_bound_addr                     			(12 + CII_II_samples_Y_BEGIN_ADDR) //1 short
#define layer_II_sblimit_addr                   			(13 + CII_II_samples_Y_BEGIN_ADDR) //1 short

//CII_III_decode
#define CII_III_decode_Y_BEGIN_ADDR             			(0 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_III_decode_sfbwidth_ADDR          			(0 + CII_III_decode_Y_BEGIN_ADDR) //2 short

//CII_III_decode_inner_0
#define CII_III_decode_inner_0_Y_BEGIN_ADDR     			(2 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_III_decode_sblimit_ADDR           			(0 + CII_III_decode_inner_0_Y_BEGIN_ADDR) //2 short
#define LOCAL_III_decode_output_ADDR            			(2 + CII_III_decode_inner_0_Y_BEGIN_ADDR) //72 short

//CII_III_scalefactors_lsf
#define CII_III_scalefactors_lsf_Y_BEGIN_ADDR   			(2 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_III_scalefactors_lsf_slen_ADDR    			(0 + CII_III_scalefactors_lsf_Y_BEGIN_ADDR) //4 short

//CII_III_huffdecode
#define CII_III_huffdecode_Y_BEGIN_ADDR         			(2 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_HUFF_SFBOUND_ADDR                 			(0 + CII_III_huffdecode_Y_BEGIN_ADDR) //2 short
#define LOCAL_HUFF_REQCACHE_ADDR                			(2 + CII_III_huffdecode_Y_BEGIN_ADDR) //32 short
#define LOCAL_HUFF_TABLE_ADDR                   			(34 + CII_III_huffdecode_Y_BEGIN_ADDR) //1 short
#define LOCAL_HUFF_LINBITS_ADDR                 			(35 + CII_III_huffdecode_Y_BEGIN_ADDR) //1 short
#define LOCAL_HUFF_STARTBITS_ADDR               			(36 + CII_III_huffdecode_Y_BEGIN_ADDR) //1 short
#define LOCAL_HUFF_REQHITS_ADDR                 			(37 + CII_III_huffdecode_Y_BEGIN_ADDR) //1 short
#define LOCAL_HUFF_REGION_ADDR                  			(38 + CII_III_huffdecode_Y_BEGIN_ADDR) //1 short
#define LOCAL_HUFF_XRPTR_ADDR                   			(39 + CII_III_huffdecode_Y_BEGIN_ADDR) //1 short

//CII_III_stereo
#define CII_III_stereo_Y_BEGIN_ADDR             			(2 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_modes_ADDR                        			(0 + CII_III_stereo_Y_BEGIN_ADDR) //40 short
#define LOCAL_bound_ADDR                        			(40 + CII_III_stereo_Y_BEGIN_ADDR) //4 short

//CII_III_reorder
#define CII_III_reorder_Y_BEGIN_ADDR            			(2 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_sbw_ADDR                          			(0 + CII_III_reorder_Y_BEGIN_ADDR) //3 short
#define LOCAL_sw_ADDR                           			(3 + CII_III_reorder_Y_BEGIN_ADDR) //3 short
#define LOCAL_III_reorder_tmp_ADDR              			((STATIC_MAD_SBSAMPLE_ADDR+3*576*2))

//Coolsand_III_equalizer
#define Coolsand_III_equalizer_Y_BEGIN_ADDR     			(2 + MP3_LOCAL_Y_BEGIN_ADDR)
#define MP3_Tab_Equalizer_addr                  			(0 + Coolsand_III_equalizer_Y_BEGIN_ADDR) //144 short
#define MP3_Tab_Equalizer_SampleFrequeyBin_Map_addr			(MP3_Tab_Equalizer_addr+11*12)

//CII_III_exponents
#define CII_III_exponents_Y_BEGIN_ADDR          			(42 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_EXPONENTS_ADDR                    			(0 + CII_III_exponents_Y_BEGIN_ADDR) //39 short

//CII_imdct36
#define CII_imdct36_X_BEGIN_ADDR                			(0 + MP3_LOCAL_X_BEGIN_ADDR)
#define LOCAL_III_imdct_s_Y_ADDR                			(LOCAL_BX_ADDR)

//CII_imdct36
#define CII_imdct36_Y_BEGIN_ADDR                			(2 + MP3_LOCAL_Y_BEGIN_ADDR)
#define LOCAL_III_imdct36_X_16_ADDR             			((72+LOCAL_III_decode_output_ADDR))
#define LOCAL_III_imdct36_X_4_ADDR              			((18+LOCAL_III_imdct36_X_16_ADDR))
#define LOCAL_III_imdct36_t_16_ADDR             			((36+LOCAL_III_imdct36_X_16_ADDR))
#define LOCAL_III_imdct36_t_4_ADDR              			((44+LOCAL_III_imdct36_X_16_ADDR))

/******************
 **  AMR_GLOBAL  **
 ******************/

//GLOBAL_VARS
//VPP_SPEECH_DEC_IN_STRUCT
#define VPP_SPEECH_DEC_IN_STRUCT                			(0 + AMR_GLOBAL_X_BEGIN_ADDR)
#define INPUT_BITS2_ADDR                        			(0 + VPP_SPEECH_DEC_IN_STRUCT) //8 short
#define INPUT_ACTUAL_ADDR                       			(8 + VPP_SPEECH_DEC_IN_STRUCT) //48 short

//VPP_SPEECH_DEC_OUT_STRUCT
#define VPP_SPEECH_DEC_OUT_STRUCT               			(56 + AMR_GLOBAL_X_BEGIN_ADDR)
#define OUTPUT_SPEECH_BUFFER2_ADDR              			(0 + VPP_SPEECH_DEC_OUT_STRUCT) //160 short
#define OUTPUT_SPEECH_BUFFER1_ADDR              			(160 + VPP_SPEECH_DEC_OUT_STRUCT) //160 short

#define STRUCT_POST_FILTERSTATE_RES2_ADDR       			(376 + AMR_GLOBAL_X_BEGIN_ADDR) //40 short
#define STRUCT_POST_FILTERSTATE_MEM_SYN_PST_ADDR			(416 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_PREEMPHASISSTATE_ADDR            			(426 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_PREEMPHASISSTATE_MEM_PRE_ADDR    			(STRUCT_PREEMPHASISSTATE_ADDR)
#define STRUCT_AGCSTATE_ADDR                    			(427 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_AGCSTATE_PAST_GAIN_ADDR          			(STRUCT_AGCSTATE_ADDR)
#define STRUCT_POST_FILTERSTATE_SYNTH_BUF_ADDR  			(428 + AMR_GLOBAL_X_BEGIN_ADDR) //170 short
#define STRUCT_POST_PROCESSSTATE_ADDR           			(598 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_POST_PROCESSSTATE_Y2_HI_ADDR     			(599 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_POST_PROCESSSTATE_Y1_LO_ADDR     			(600 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_POST_PROCESSSTATE_Y1_HI_ADDR     			(601 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_POST_PROCESSSTATE_X0_ADDR        			(602 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_POST_PROCESSSTATE_X1_ADDR        			(603 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STURCT_SPEECH_DECODE_FRAMESTATE_PREV_MODE_ADDR			(604 + AMR_GLOBAL_X_BEGIN_ADDR) //8 short
#define TX_SP_FLAG_ADDR                         			(612 + AMR_GLOBAL_X_BEGIN_ADDR) //2 short
#define STRUCT_DECOD_AMRSTATE_OLD_EXC_ADDR      			(614 + AMR_GLOBAL_X_BEGIN_ADDR) //154 short
#define STRUCT_DECOD_AMRSTATE_EXC_ADDR          			(768 + AMR_GLOBAL_X_BEGIN_ADDR) //40 short
#define STRUCT_DECOD_AMRSTATE_LSP_OLD_ADDR      			(808 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_DECOD_AMRSTATE_MEM_SYN_ADDR      			(818 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_DECOD_AMRSTATE_SHARP_ADDR        			(828 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_OLD_T0_ADDR       			(829 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_PREV_BF_ADDR      			(830 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_PREV_PDF_ADDR     			(831 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_STATE_ADDR        			(832 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_T0_LAGBUFF_ADDR   			(833 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_EXCENERGYHIST_ADDR			(834 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_DECOD_AMRSTATE_INBACKGROUNDNOISE_ADDR			(844 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_VOICEDHANGOVER_ADDR			(845 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DECOD_AMRSTATE_LTPGAINHISTORY_ADDR			(846 + AMR_GLOBAL_X_BEGIN_ADDR) //9 short
#define STRUCT_DECOD_AMRSTATE_NODATASEED_ADDR   			(855 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_BGN_SCDSTATE_ADDR                			(856 + AMR_GLOBAL_X_BEGIN_ADDR) //60 short
#define STRUCT_BGN_SCDSTATE_FRAMEENERGYHIST_ADDR			(STRUCT_BGN_SCDSTATE_ADDR)
#define STRUCT_BGN_SCDSTATE_BGHANGOVER_ADDR     			(916 + AMR_GLOBAL_X_BEGIN_ADDR) //2 short
#define STRUCT_CB_GAIN_AVERAGESTATE_ADDR        			(918 + AMR_GLOBAL_X_BEGIN_ADDR) //8 short
#define STRUCT_CB_GAIN_AVERAGESTATE_CBGAINHISTORY_ADDR			(STRUCT_CB_GAIN_AVERAGESTATE_ADDR)
#define STRUCT_CB_GAIN_AVERAGESTATE_HANGCOUNT_ADDR			(926 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_CB_GAIN_AVERAGESTATE_HANGVAR_ADDR			(927 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_LSP_AVGSTATE_ADDR                			(928 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_LSP_AVGSTATE_LSP_MEANSAVE_ADDR   			(STRUCT_LSP_AVGSTATE_ADDR)
#define STRUCT_D_PLSFSTATE_ADDR                 			(938 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_D_PLSFSTATE_PAST_LSF_Q_ADDR      			(STRUCT_D_PLSFSTATE_ADDR)
#define STRUCT_D_PLSFSTATE_PAST_R_Q_ADDR        			(948 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_EC_GAIN_PITCHSTATE_ADDR          			(958 + AMR_GLOBAL_X_BEGIN_ADDR) //6 short
#define STRUCT_EC_GAIN_PITCHSTATE_PBUF_ADDR     			(STRUCT_EC_GAIN_PITCHSTATE_ADDR)
#define STRUCT_EC_GAIN_PITCHSTATE_PAST_GAIN_PIT_ADDR			(964 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_EC_GAIN_PITCHSTATE_PREV_GP_ADDR  			(965 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_EC_GAIN_CODESTATE_ADDR           			(966 + AMR_GLOBAL_X_BEGIN_ADDR) //6 short
#define STRUCT_EC_GAIN_CODESTATE_GBUF_ADDR      			(STRUCT_EC_GAIN_CODESTATE_ADDR)
#define STRUCT_EC_GAIN_CODESTATE_PAST_GAIN_CODE_ADDR			(972 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_EC_GAIN_CODESTATE_PREV_GC_ADDR   			(973 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_GC_PREDSTATE_ADDR                			(974 + AMR_GLOBAL_X_BEGIN_ADDR) //4 short
#define STRUCT_GC_PREDSTATE_PAST_QUA_EN_ADDR    			(STRUCT_GC_PREDSTATE_ADDR)
#define STRUCT_GC_PREDSTATE_PAST_QUA_EN_MR122_ADDR			(978 + AMR_GLOBAL_X_BEGIN_ADDR) //4 short
#define STRUCT_PH_DISPSTATE_ADDR                			(982 + AMR_GLOBAL_X_BEGIN_ADDR) //6 short
#define STRUCT_PH_DISPSTATE_GAINMEM_ADDR        			(STRUCT_PH_DISPSTATE_ADDR)
#define STRUCT_PH_DISPSTATE_PREVSTATE_ADDR      			(988 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_PH_DISPSTATE_PREVCBGAIN_ADDR     			(989 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_PH_DISPSTATE_LOCKFULL_ADDR       			(990 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_PH_DISPSTATE_ONSET_ADDR          			(991 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_ADDR                			(992 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_SINCE_LAST_SID_ADDR 			(STRUCT_DTX_DECSTATE_ADDR)
#define STRUCT_DTX_DECSTATE_TRUE_SID_PERIOD_INV_ADDR			(993 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
//STRUCT_AMR_DEC_INPUT_ADDR
#define STRUCT_AMR_DEC_INPUT_ADDR               			(994 + AMR_GLOBAL_X_BEGIN_ADDR)
#define STRUCT_AMR_DEC_INPUT_mode_ADDR          			(0 + STRUCT_AMR_DEC_INPUT_ADDR) //1 short
#define STRUCT_AMR_DEC_INPUT_reset_ADDR         			(1 + STRUCT_AMR_DEC_INPUT_ADDR) //1 short
#define STRUCT_AMR_DEC_INBUF_ADDR               			(2 + STRUCT_AMR_DEC_INPUT_ADDR) //2 short
#define STRUCT_AMR_DEC_OUTBUF_ADDR              			(4 + STRUCT_AMR_DEC_INPUT_ADDR) //2 short
#define AMR_RESERVE_blank1                      			(6 + STRUCT_AMR_DEC_INPUT_ADDR) //8 short

#define STRUCT_DTX_DECSTATE_LSP_OLD_ADDR        			(1008 + AMR_GLOBAL_X_BEGIN_ADDR) //10 short
#define STRUCT_DTX_DECSTATE_LSF_HIST_ADDR       			(1018 + AMR_GLOBAL_X_BEGIN_ADDR) //80 short
#define STRUCT_DTX_DECSTATE_LSF_HIST_PTR_ADDR   			(1098 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_LOG_PG_MEAN_ADDR    			(1099 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_LOG_EN_HIST_ADDR    			(1100 + AMR_GLOBAL_X_BEGIN_ADDR) //8 short
#define STRUCT_DTX_DECSTATE_LOG_EN_HIST_PTR_ADDR			(1108 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_LOG_EN_ADJUST_ADDR  			(1109 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_DTXHANGOVERCOUNT_ADDR			(1110 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_DECANAELAPSEDCOUNT_ADDR			(1111 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_SID_FRAME_ADDR      			(1112 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_VALID_DATA_ADDR     			(1113 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_DTXHANGOVERADDED_ADDR			(1114 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_DTXGLOBALSTATE_ADDR 			(1115 + AMR_GLOBAL_X_BEGIN_ADDR) //1 short
#define STRUCT_DTX_DECSTATE_DATA_UPDATED_ADDR   			(1116 + AMR_GLOBAL_X_BEGIN_ADDR) //2 short
#define AMR_RESERVE_blank2                      			(1118 + AMR_GLOBAL_X_BEGIN_ADDR) //2 short

//GLOBAL_VARS
#define GLOBAL_RELOAD_MODE_ADDR                 			(0 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_RESET_FLAG_ADDR                  			(1 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_RESET_ADDR                       			(2 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_DEC_RESET_DONE_ADDR              			(3 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_RELOAD_RESET_FLAG_ADDR           			(4 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_BFI_ADDR                         			(5 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_UFI_ADDR                         			(6 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SID_ADDR                         			(7 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_TAF_ADDR                         			(8 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_DEC_MODE_ADDR                    			(9 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_ENC_MODE_ADDR                    			(10 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_ENC_USED_MODE_ADDR               			(11 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DEC_RESET_FLAG_ADDR                     			(12 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DEC_MAIN_RESET_FLAG_OLD_ADDR            			(13 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DEC_AMR_FRAME_TYPE_ADDR                 			(14 + AMR_GLOBAL_Y_BEGIN_ADDR) //2 short
#define DEC_INPUT_ADDR_ADDR                     			(16 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DEC_OUTPUT_ADDR_ADDR                    			(17 + AMR_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DEC_INOUT_ADDR_BAK_ADDR                 			(18 + AMR_GLOBAL_Y_BEGIN_ADDR) //2 short
//GLOBAL_OUTPUT_STRUCT_ADDR
#define GLOBAL_OUTPUT_STRUCT_ADDR               			(20 + AMR_GLOBAL_Y_BEGIN_ADDR)
#define STRUCT_AMR_DEC_mode_ADDR                			(0 + GLOBAL_OUTPUT_STRUCT_ADDR) //1 short
#define STRUCT_AMR_DEC_CONSUME_ADDR             			(1 + GLOBAL_OUTPUT_STRUCT_ADDR) //1 short
#define STRUCT_AMR_DEC_length_ADDR              			(2 + GLOBAL_OUTPUT_STRUCT_ADDR) //1 short

#define AMRJPEG_CONST_BLANK                     			(23 + AMR_GLOBAL_Y_BEGIN_ADDR) //927 short
//Short_Constant
#define Short_Constant                          			(950 + AMR_GLOBAL_Y_BEGIN_ADDR)
#define CONST_AMR_0x1fff_ADDR                   			(0 + Short_Constant) //1 short
#define CONST_AMR_5462_ADDR                     			(1 + Short_Constant) //1 short
#define CONST_AMR_0x3fff_ADDR                   			(2 + Short_Constant) //1 short
#define CONST_AMR_0x0800_ADDR                   			(3 + Short_Constant) //1 short
#define CONST_AMR_21299_ADDR                    			(4 + Short_Constant) //1 short
#define CONST_AMR_5443_ADDR                     			(5 + Short_Constant) //1 short
#define CONST_AMR_112_ADDR                      			(6 + Short_Constant) //1 short
#define CONST_AMR_105_ADDR                      			(7 + Short_Constant) //1 short
#define CONST_AMR_197_ADDR                      			(8 + Short_Constant) //1 short
#define CONST_AMR_31128_ADDR                    			(9 + Short_Constant) //1 short
#define CONST_AMR_134_ADDR                      			(10 + Short_Constant) //1 short
#define CONST_AMR_0x177_ADDR                    			(11 + Short_Constant) //1 short
#define CONST_AMR_102_ADDR                      			(12 + Short_Constant) //1 short
#define CONST_AMR_3277_ADDR                     			(13 + Short_Constant) //1 short
#define CONST_AMR_0x87_ADDR                     			(14 + Short_Constant) //1 short
#define CONST_AMR_NEG11_ADDR                    			(15 + Short_Constant) //1 short
#define CONST_AMR_31821_ADDR                    			(16 + Short_Constant) //1 short
#define CONST_AMR_29491_ADDR                    			(17 + Short_Constant) //1 short
#define CONST_AMR_9830_ADDR                     			(18 + Short_Constant) //1 short
#define CONST_AMR_14746_ADDR                    			(19 + Short_Constant) //1 short
#define CONST_AMR_17578_ADDR                    			(20 + Short_Constant) //1 short
#define CONST_AMR_16384_ADDR                    			(21 + Short_Constant) //1 short
#define CONST_AMR_256_ADDR                      			(22 + Short_Constant) //1 short
#define CONST_AMR_160_ADDR                      			(23 + Short_Constant) //1 short
#define CONST_AMR_143_ADDR                      			(24 + Short_Constant) //1 short
#define CONST_AMR_26214_ADDR                    			(25 + Short_Constant) //1 short
#define CONST_AMR_4096_ADDR                     			(26 + Short_Constant) //1 short
#define CONST_AMR_64_ADDR                       			(27 + Short_Constant) //1 short
#define CONST_AMR_128_ADDR                      			(28 + Short_Constant) //1 short
#define CONST_AMR_205_ADDR                      			(29 + Short_Constant) //1 short
#define CONST_AMR_50_ADDR                       			(30 + Short_Constant) //1 short
#define CONST_AMR_44_ADDR                       			(31 + Short_Constant) //1 short
#define CONST_AMR_2_ADDR                        			(32 + Short_Constant) //1 short
#define CONST_AMR_3_ADDR                        			(33 + Short_Constant) //1 short
#define CONST_AMR_4_ADDR                        			(34 + Short_Constant) //1 short
#define CONST_AMR_6_ADDR                        			(35 + Short_Constant) //1 short
#define CONST_AMR_7_ADDR                        			(36 + Short_Constant) //1 short
#define CONST_AMR_8_ADDR                        			(37 + Short_Constant) //1 short
#define CONST_AMR_9_ADDR                        			(38 + Short_Constant) //1 short
#define CONST_AMR_10_ADDR                       			(39 + Short_Constant) //1 short
#define CONST_AMR_12_ADDR                       			(40 + Short_Constant) //1 short
#define CONST_AMR_16_ADDR                       			(41 + Short_Constant) //1 short
#define CONST_AMR_17_ADDR                       			(42 + Short_Constant) //1 short
#define CONST_AMR_18_ADDR                       			(43 + Short_Constant) //1 short
#define CONST_AMR_20_ADDR                       			(44 + Short_Constant) //1 short
#define CONST_AMR_22_ADDR                       			(45 + Short_Constant) //1 short
#define CONST_AMR_25_ADDR                       			(46 + Short_Constant) //1 short
#define CONST_AMR_30_ADDR                       			(47 + Short_Constant) //1 short
#define CONST_AMR_31_ADDR                       			(48 + Short_Constant) //1 short
#define CONST_AMR_32_ADDR                       			(49 + Short_Constant) //1 short
#define CONST_AMR_33_ADDR                       			(50 + Short_Constant) //1 short
#define CONST_AMR_40_ADDR                       			(51 + Short_Constant) //1 short
#define CONST_AMR_41_ADDR                       			(52 + Short_Constant) //1 short
#define CONST_AMR_15_ADDR                       			(53 + Short_Constant) //1 short
#define CONST_AMR_11_ADDR                       			(54 + Short_Constant) //1 short
#define CONST_AMR_3276_ADDR                     			(55 + Short_Constant) //1 short
#define CONST_AMR_0x2000_ADDR                   			(56 + Short_Constant) //1 short
#define CONST_AMR_0x2666_ADDR                   			(57 + Short_Constant) //1 short
#define CONST_AMR_neg2_ADDR                     			(58 + Short_Constant) //1 short
#define CONST_AMR_80_ADDR                       			(59 + Short_Constant) //1 short
#define CONST_AMR_512_ADDR                      			(60 + Short_Constant) //1 short
#define CONST_AMR_1024_ADDR                     			(61 + Short_Constant) //1 short
#define CONST_AMR_0x199a_ADDR                   			(62 + Short_Constant) //1 short
#define CONST_AMR_120_ADDR                      			(63 + Short_Constant) //1 short
#define CONST_AMR_124_ADDR                      			(64 + Short_Constant) //1 short
#define CONST_AMR_1311_ADDR                     			(65 + Short_Constant) //1 short
#define CONST_AMR_368_ADDR                      			(66 + Short_Constant) //1 short
#define CONST_AMR_463_ADDR                      			(67 + Short_Constant) //1 short
#define CONST_AMR_9000_ADDR                     			(68 + Short_Constant) //1 short
#define CONST_AMR_5325_ADDR                     			(69 + Short_Constant) //1 short

//Long_Constant
#define Long_Constant                           			(1020 + AMR_GLOBAL_Y_BEGIN_ADDR)
#define CONST_AMR_0_ADDR                        			(0 + Long_Constant) //2 short
#define CONST_AMR_1_ADDR                        			(2 + Long_Constant) //2 short
#define CONST_AMR_0x40000000_ADDR               			(4 + Long_Constant) //2 short
#define CONST_AMR_0x7FFFFFFF_ADDR               			(6 + Long_Constant) //2 short
#define CONST_AMR_0x70816958_ADDR               			(8 + Long_Constant) //2 short
#define CONST_AMR_0x00010001_ADDR               			(10 + Long_Constant) //2 short
#define CONST_AMR_0x00020002_ADDR               			(12 + Long_Constant) //2 short
#define CONST_AMR_0x00080008_ADDR               			(14 + Long_Constant) //2 short
#define CONST_AMR_0xFFFFFFFF_ADDR               			(16 + Long_Constant) //2 short
#define CONST_AMR_0x00004000L_ADDR              			(18 + Long_Constant) //2 short
#define CONST_AMR_0x00007FFF_ADDR               			(20 + Long_Constant) //2 short
#define CONST_AMR_0x00008000_ADDR               			(22 + Long_Constant) //2 short
#define CONST_AMR_0x0000FFFF_ADDR               			(24 + Long_Constant) //2 short
#define CONST_AMR_0x00000005L_ADDR              			(26 + Long_Constant) //2 short
#define CONST_AMR_0x40000000L_ADDR              			(CONST_AMR_0x40000000_ADDR)
#define CONST_AMR_0x7fffffff_ADDR               			(CONST_AMR_0x7FFFFFFF_ADDR)
#define CONST_AMR_WORD32_0_ADDR                 			(CONST_AMR_0_ADDR)
#define CONST_AMR_0x00000001_ADDR               			(CONST_AMR_1_ADDR)
#define CONST_AMR_0x7FFF_ADDR                   			(CONST_AMR_0x00007FFF_ADDR)
#define CONST_AMR_0xFFFF_ADDR                   			(CONST_AMR_0x0000FFFF_ADDR)
#define CONST_AMR_0x4000_ADDR                   			(CONST_AMR_0x00004000L_ADDR)
#define CONST_AMR_5_ADDR                        			(CONST_AMR_0x00000005L_ADDR)
#define CONST_AMR_0X80008_ADDR                  			(CONST_AMR_0x00080008_ADDR)
#define CONST_AMR_0x00008000L_ADDR              			(CONST_AMR_0x00008000_ADDR)
#define CONST_AMR_0x7fff_ADDR                   			(CONST_AMR_0x7FFF_ADDR)
#define CONST_AMR_0x400_ADDR                    			(CONST_AMR_1024_ADDR)
#define CONST_AMR_0x80008_ADDR                  			(CONST_AMR_0x00080008_ADDR)
#define CONST_AMR_6554_ADDR                     			(CONST_AMR_0x199a_ADDR)

//EFR_TABLE_START
#define EFR_TABLE_START                         			(1048 + AMR_GLOBAL_Y_BEGIN_ADDR)
#define TABLE_MEAN_LSF_ADDR                     			(0 + EFR_TABLE_START) //10 short
#define TABLE_SLOPE_ADDR                        			(10 + EFR_TABLE_START) //64 short
#define TABLE_LSP_LSF_ADDR                      			(74 + EFR_TABLE_START) //66 short
#define TABLE_LOG2_ADDR                         			(140 + EFR_TABLE_START) //34 short
#define TABLE_INV_SQRT_ADDR                     			(174 + EFR_TABLE_START) //50 short
#define TABLE_POW2_ADDR                         			(224 + EFR_TABLE_START) //34 short
#define STATIC_CONST_DHF_MASK_ADDR              			(258 + EFR_TABLE_START) //58 short
#define STATIC_CONST_QUA_GAIN_PITCH_ADDR        			(316 + EFR_TABLE_START) //16 short
#define STATIC_CONST_F_GAMMA4_ADDR              			(332 + EFR_TABLE_START) //10 short
#define STATIC_CONST_F_GAMMA3_ADDR              			(342 + EFR_TABLE_START) //10 short
#define STATIC_CONST_INTER_6_ADDR               			(352 + EFR_TABLE_START) //62 short
#define STATIC_CONST_DGRAY_ADDR                 			(414 + EFR_TABLE_START) //8 short
#define TABLE_DICO1_LSF_ADDR                    			(422 + EFR_TABLE_START) //512 short
#define TABLE_DICO2_LSF_ADDR                    			(934 + EFR_TABLE_START) //1024 short
#define TABLE_DICO3_LSF_ADDR                    			(1958 + EFR_TABLE_START) //1024 short
#define TABLE_DICO4_LSF_ADDR                    			(2982 + EFR_TABLE_START) //1024 short
#define TABLE_DICO5_LSF_ADDR                    			(4006 + EFR_TABLE_START) //256 short
#define STATIC_CONST_BITNO_ADDR                 			(4262 + EFR_TABLE_START) //30 short

//AMR_TABLE_START
#define AMR_TABLE_START                         			(5340 + AMR_GLOBAL_Y_BEGIN_ADDR)
#define STATIC_CONST_PRMNO_ADDR                 			(0 + AMR_TABLE_START) //10 short
#define STATIC_CONST_PRMNOFSF_ADDR              			(10 + AMR_TABLE_START) //8 short
#define STATIC_CONST_bitno_MR475_ADDR           			(18 + AMR_TABLE_START) //9 short
#define STATIC_CONST_bitno_MR515_ADDR           			(27 + AMR_TABLE_START) //10 short
#define STATIC_CONST_bitno_MR59_ADDR            			(37 + AMR_TABLE_START) //10 short
#define STATIC_CONST_bitno_MR67_ADDR            			(47 + AMR_TABLE_START) //10 short
#define STATIC_CONST_bitno_MR74_ADDR            			(57 + AMR_TABLE_START) //10 short
#define STATIC_CONST_bitno_MR795_ADDR           			(67 + AMR_TABLE_START) //12 short
#define STATIC_CONST_bitno_MR102_ADDR           			(79 + AMR_TABLE_START) //20 short
#define STATIC_CONST_bitno_MRDTX_ADDR           			(99 + AMR_TABLE_START) //3 short
#define STATIC_CONST_BITNO_AMR_ADDR             			(102 + AMR_TABLE_START) //10 short
#define STATIC_CONST_startPos_ADDR              			(112 + AMR_TABLE_START) //16 short
#define STATIC_CONST_dhf_MR475_ADDR             			(128 + AMR_TABLE_START) //18 short
#define STATIC_CONST_dhf_MR515_ADDR             			(146 + AMR_TABLE_START) //20 short
#define STATIC_CONST_dhf_MR59_ADDR              			(166 + AMR_TABLE_START) //20 short
#define STATIC_CONST_dhf_MR67_ADDR              			(186 + AMR_TABLE_START) //20 short
#define STATIC_CONST_dhf_MR74_ADDR              			(206 + AMR_TABLE_START) //20 short
#define STATIC_CONST_dhf_MR795_ADDR             			(226 + AMR_TABLE_START) //24 short
#define STATIC_CONST_dhf_MR102_ADDR             			(250 + AMR_TABLE_START) //40 short
#define STATIC_CONST_dhf_MR122_ADDR             			(290 + AMR_TABLE_START) //58 short
#define STATIC_CONST_dhf_amr_ADDR               			(348 + AMR_TABLE_START) //8 short
#define STATIC_CONST_qua_gain_code_ADDR         			(356 + AMR_TABLE_START) //96 short
#define STATIC_CONST_lsp_init_data_ADDR         			(452 + AMR_TABLE_START) //10 short
#define STATIC_past_rq_init_ADDR                			(462 + AMR_TABLE_START) //80 short
#define TABLE_SQRT_L_ADDR                       			(542 + AMR_TABLE_START) //50 short
#define STATIC_CONST_WIND_200_40_ADDR           			(592 + AMR_TABLE_START) //240 short
#define STATIC_trackTable_ADDR                  			(832 + AMR_TABLE_START) //20 short
#define STATIC_CONST_lsf_hist_mean_scale_ADDR   			(852 + AMR_TABLE_START) //10 short
#define STATIC_CONST_dtx_log_en_adjust_ADDR     			(862 + AMR_TABLE_START) //10 short
#define STATIC_CONST_pred_ADDR                  			(872 + AMR_TABLE_START) //4 short
#define STATIC_CONST_pred_MR122_ADDR            			(876 + AMR_TABLE_START) //4 short
#define STATIC_CONST_b_60Hz_ADDR                			(880 + AMR_TABLE_START) //4 short
#define STATIC_CONST_a_60Hz_ADDR                			(884 + AMR_TABLE_START) //4 short
#define STATIC_CONST_gamma3_ADDR                			(888 + AMR_TABLE_START) //10 short
#define AMR_RESET_VALUE_TABLE_ADDR              			(898 + AMR_TABLE_START) //48 short

#define GLOBAL_OUT244_ADDR                      			(6286 + AMR_GLOBAL_Y_BEGIN_ADDR) //244 short
#define CONST_TAB244_ADDR                       			(GLOBAL_OUT244_ADDR)
#define TEMP_MYRING_ADDR                        			(6530 + AMR_GLOBAL_Y_BEGIN_ADDR) //2 short
#define CONST_HEAD_RING                         			(TEMP_MYRING_ADDR)

/*****************
 **  AMR_LOCAL  **
 *****************/

//CII_AMR_Decode
#define CII_AMR_Decode_X_BEGIN_ADDR             			(0 + AMR_LOCAL_X_BEGIN_ADDR)
#define DEC_AMR_LSP_NEW_ADDRESS                 			(0 + CII_AMR_Decode_X_BEGIN_ADDR) //10 short
#define LOCAL_ENC_SO_ADDR                       			(DEC_AMR_LSP_NEW_ADDRESS)
#define DEC_AMR_LSP_MID_ADDRESS                 			(10 + CII_AMR_Decode_X_BEGIN_ADDR) //10 short
#define DEC_AMR_PREV_LSF_ADDRESS                			(20 + CII_AMR_Decode_X_BEGIN_ADDR) //10 short
#define DEC_AMR_LSF_I_ADDRESS                   			(30 + CII_AMR_Decode_X_BEGIN_ADDR) //50 short
#define DEC_AMR_EXCP_ADDRESS                    			(80 + CII_AMR_Decode_X_BEGIN_ADDR) //40 short
#define DEC_AMR_EXC_ENHANCED_ADDRESS            			(120 + CII_AMR_Decode_X_BEGIN_ADDR) //44 short
#define DEC_AMR_T0_ADDRESS                      			(164 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_T0_FRAC_ADDRESS                 			(165 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_INDEX_ADDRESS                   			(166 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_INDEX_MR475_ADDRESS             			(167 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_GAIN_PIT_ADDRESS                			(168 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_GAIN_CODE_ADDRESS               			(169 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_GAIN_CODE_MIX_ADDRESS           			(170 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_PIT_SHARP_ADDRESS               			(171 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_PIT_FLAG_ADDRESS                			(172 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_PIT_FAC_ADDRESS                 			(173 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_T0_MIN_ADDRESS                  			(174 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_T0_MAX_ADDRESS                  			(175 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_DELTA_FRC_LOW_ADDRESS           			(176 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_DELTA_FRC_RANGE_ADDRESS         			(177 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP_SHIFT_ADDRESS              			(178 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP_ADDRESS                    			(179 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_L_TEMP_ADDRESS                  			(180 + CII_AMR_Decode_X_BEGIN_ADDR) //2 short
#define DEC_AMR_FLAG4_ADDRESS                   			(182 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_CAREFULFLAG_ADDRESS             			(183 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_EXCENERGY_ADDRESS               			(184 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_SUBFRNR_ADDRESS                 			(185 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_EVENSUBFR_ADDRESS               			(186 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_NEWDTXSTATE_ADDRESS             			(187 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_BFI_ADDRESS                     			(188 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_PDFI_ADDRESS                    			(189 + CII_AMR_Decode_X_BEGIN_ADDR) //1 short
#define DEC_AMR_A_T_ADDRESS                     			(190 + CII_AMR_Decode_X_BEGIN_ADDR) //44 short
#define DEC_AMR_PARM_ADDRESS                    			(234 + CII_AMR_Decode_X_BEGIN_ADDR) //58 short
#define COD_AMR_CODE_ADDRESS                    			(292 + CII_AMR_Decode_X_BEGIN_ADDR) //40 short
#define DEC_AMR_CODE_ADDRESS                    			(COD_AMR_CODE_ADDRESS)

//CII_AMR_Decode
#define CII_AMR_Decode_Y_BEGIN_ADDR             			(0 + AMR_LOCAL_Y_BEGIN_ADDR)
#define AMR_DEC_BUFOUT_ADDR                     			(0 + CII_AMR_Decode_Y_BEGIN_ADDR) //160 short

//CII_Syn_filt
#define CII_Syn_filt_Y_BEGIN_ADDR               			(160 + AMR_LOCAL_Y_BEGIN_ADDR)
#define SYN_FILT_TMP_ADDR                       			(0 + CII_Syn_filt_Y_BEGIN_ADDR) //80 short

//CII_bits2prm_amr_efr
#define CII_bits2prm_amr_efr_X_BEGIN_ADDR       			(332 + AMR_LOCAL_X_BEGIN_ADDR)
#define AMR_DEC_BUFIN_ADDR                      			(0 + CII_bits2prm_amr_efr_X_BEGIN_ADDR) //246 short

//CII_amr_dec_122
#define CII_amr_dec_122_X_BEGIN_ADDR            			(332 + AMR_LOCAL_X_BEGIN_ADDR)
#define DEC_AMR_TEMP0_ADDRESS                   			(0 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP1_ADDRESS                   			(1 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP2_ADDRESS                   			(2 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP3_ADDRESS                   			(3 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP4_ADDRESS                   			(4 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP5_ADDRESS                   			(5 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP6_ADDRESS                   			(6 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP7_ADDRESS                   			(7 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP8_ADDRESS                   			(8 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP9_ADDRESS                   			(9 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP10_ADDRESS                  			(10 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP11_ADDRESS                  			(11 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP12_ADDRESS                  			(12 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP13_ADDRESS                  			(13 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP14_ADDRESS                  			(14 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP15_ADDRESS                  			(15 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP16_ADDRESS                  			(16 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP17_ADDRESS                  			(17 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP18_ADDRESS                  			(18 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP19_ADDRESS                  			(19 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP20_ADDRESS                  			(20 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP21_ADDRESS                  			(21 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short
#define DEC_AMR_TEMP22_ADDRESS                  			(22 + CII_amr_dec_122_X_BEGIN_ADDR) //1 short

//CII_Int_lpc_1and3_dec
#define CII_Int_lpc_1and3_dec_Y_BEGIN_ADDR      			(160 + AMR_LOCAL_Y_BEGIN_ADDR)
#define INT_LPC_LSP_ADDR                        			(0 + CII_Int_lpc_1and3_dec_Y_BEGIN_ADDR) //10 short
#define F1_ADDR                                 			(10 + CII_Int_lpc_1and3_dec_Y_BEGIN_ADDR) //12 short
#define F2_ADDR                                 			(22 + CII_Int_lpc_1and3_dec_Y_BEGIN_ADDR) //12 short

//CII_gmed_n
#define CII_gmed_n_Y_BEGIN_ADDR                 			(160 + AMR_LOCAL_Y_BEGIN_ADDR)
#define MR475_gain_quant_coeff_ADDRESS          			(0 + CII_gmed_n_Y_BEGIN_ADDR) //10 short
#define MR475_gain_quant_coeff_lo_ADDRESS       			(10 + CII_gmed_n_Y_BEGIN_ADDR) //10 short

/*******************
 **  JPEG_GLOBAL  **
 *******************/

//GLOBAL_VARS
#define JEPG_DEC_FOR_MP3_AMR_RAM_X_ALIGNED      			(0 + JPEG_GLOBAL_X_BEGIN_ADDR) //3244 short
//JPEG_GLOBAL_X_VAR
#define JPEG_GLOBAL_X_VAR                       			(3244 + JPEG_GLOBAL_X_BEGIN_ADDR)
#define GLOBAL_MCUBuffer_ADDR                   			(0 + JPEG_GLOBAL_X_VAR) //640 short
#define GLOBAL_MCU_lastcoef_ADDR                			(640 + JPEG_GLOBAL_X_VAR) //10 short
#define GLOBAL_ycoef_ADDR                       			(650 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_ucoef_ADDR                       			(651 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_vcoef_ADDR                       			(652 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_coef_ADDR                        			(653 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_image_control_ADDR               			(654 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_lpMCUBuffer_ADDR_ADDR            			(655 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_lastcoef_pt_ADDR                 			(656 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_temp_lpJpegBuf_ADDR              			(657 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_Y_data_ADDR                      			(658 + JPEG_GLOBAL_X_VAR) //256 short
#define GLOBAL_InnerY_ADDR                      			(914 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_InnerU_ADDR                      			(915 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_InnerV_ADDR                      			(916 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_Reserved1_ADDR                   			(917 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_DMA_Y_SIZE_ADDR                  			(918 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_DMA_UV_SIZE_ADDR                 			(919 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_DMA_Y_INC_ADDR                   			(920 + JPEG_GLOBAL_X_VAR) //2 short
#define GLOBAL_DMA_UV_INC_ADDR                  			(922 + JPEG_GLOBAL_X_VAR) //2 short
#define GLOBAL_rgb_buf_ADDR                     			(924 + JPEG_GLOBAL_X_VAR) //4800 short
#define GLOBAL_Output_U_ADDR                    			(GLOBAL_rgb_buf_ADDR + 3200)
#define GLOBAL_Output_V_ADDR                    			(GLOBAL_Output_U_ADDR + 800)
#define GLOBAL_temp_Y_data_ADDR                 			(5724 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_col_num_ADDR                     			(5725 + JPEG_GLOBAL_X_VAR) //1 short
#define TABLE_qt_table_ADDR                     			(5726 + JPEG_GLOBAL_X_VAR) //196 short
#define GLOBAL_IDCT_BUFFER_X_ADDR               			(5922 + JPEG_GLOBAL_X_VAR) //8 short
#define GLOBAL_row_even_ADDR                    			(5930 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_col_even_ADDR                    			(5931 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_block_id_ADDR                    			(5932 + JPEG_GLOBAL_X_VAR) //2 short
#define GLOBAL_JPEG_IMAGECONTROL_ADDR           			(5934 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_ImgWidth_ADDR                    			(5935 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_ImgHeight_ADDR                   			(5936 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_Temp_ImgHeight_ADDR              			(5937 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_temp_ImgWidth_ADDR               			(5938 + JPEG_GLOBAL_X_VAR) //1 short
#define GLOBAL_Temp_ImgWidth_ADDR               			(5939 + JPEG_GLOBAL_X_VAR) //1 short


//GLOBAL_VARS
#define JEPG_DEC_FOR_MP3_AMR_RAM_Y_ALIGNED      			(0 + JPEG_GLOBAL_Y_BEGIN_ADDR) //7456 short
//JPEG_Short_Constant
#define JPEG_Short_Constant                     			(7456 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define CONST_JPEG_DEC_56_ADDR                  			(0 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xdbff_ADDR              			(1 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xc0ff_ADDR              			(2 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xc4ff_ADDR              			(3 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xdaff_ADDR              			(4 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x00c4_ADDR              			(5 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_1448_ADDR                			(6 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_63_ADDR                  			(7 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_784_ADDR                 			(8 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xFF_ADDR                			(9 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xef_ADDR                			(10 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xF0_ADDR                			(11 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x3ff_ADDR               			(12 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x3df_ADDR               			(13 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x3e0_ADDR               			(14 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_464_ADDR                 			(15 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xfd7_ADDR               			(16 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x1cf_ADDR               			(17 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_4056_ADDR                			(18 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x1bf_ADDR               			(19 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_448_ADDR                 			(20 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_2676_ADDR                			(21 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0xFF00_ADDR              			(22 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x001f_ADDR              			(23 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x07ff_ADDR              			(24 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_152_ADDR                 			(25 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_176_ADDR                 			(26 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_320_ADDR                 			(27 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_reserve1_ADDR            			(28 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_367_ADDR                 			(29 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_159_ADDR                 			(30 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_220_ADDR                 			(31 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_411_ADDR                 			(32 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_29_ADDR                  			(33 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_reserve2_ADDR            			(34 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_565_ADDR                 			(35 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_3406_ADDR                			(36 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_2276_ADDR                			(37 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_2408_ADDR                			(38 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_799_ADDR                 			(39 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_4017_ADDR                			(40 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_1108_ADDR                			(41 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_3784_ADDR                			(42 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_1568_ADDR                			(43 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_181_ADDR                 			(44 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0x3f_ADDR                			(45 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_0_ADDR                   			(46 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_1_ADDR                   			(47 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_2_ADDR                   			(48 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_3_ADDR                   			(49 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_4_ADDR                   			(50 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_5_ADDR                   			(51 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_6_ADDR                   			(52 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_7_ADDR                   			(53 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_8_ADDR                   			(54 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_10_ADDR                  			(55 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_15_ADDR                  			(56 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_16_ADDR                  			(57 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_18_ADDR                  			(58 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_31_ADDR                  			(59 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_32_ADDR                  			(60 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_64_ADDR                  			(61 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_80_ADDR                  			(62 + JPEG_Short_Constant) //1 short
#define CONST_JPEG_DEC_128_ADDR                 			(63 + JPEG_Short_Constant) //1 short

//JPEG_Long_Constant
#define JPEG_Long_Constant                      			(7520 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define CONST_JPEG_DEC_0xFFFFFFFF_ADDR          			(0 + JPEG_Long_Constant) //2 short
#define CONST_JPEG_DEC_0x00000020_ADDR          			(2 + JPEG_Long_Constant) //2 short
#define CONST_JPEG_DEC_0x00000080_ADDR          			(4 + JPEG_Long_Constant) //2 short
#define CONST_JPEG_DEC_0x00000040_ADDR          			(6 + JPEG_Long_Constant) //2 short
#define CONST_JPEG_DEC_0x00002000_ADDR          			(8 + JPEG_Long_Constant) //2 short
#define CONST_JPEG_DEC_0x0000ffff_ADDR          			(10 + JPEG_Long_Constant) //2 short
#define CONST_JPEG_DEC_0x0000ff7f_ADDR          			(12 + JPEG_Long_Constant) //2 short
#define CONST_JPEG_DEC_0x0000ff80_ADDR          			(14 + JPEG_Long_Constant) //2 short

//TABLE_DClumtab
#define TABLE_DClumtab                          			(7536 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define TABLE_DClumtab0_ADDR                    			(0 + TABLE_DClumtab) //16 short
#define TABLE_DClumtab1_ADDR                    			(16 + TABLE_DClumtab) //16 short

//TABLE_DCchromtab
#define TABLE_DCchromtab                        			(7568 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define TABLE_DCchromtab0_ADDR                  			(0 + TABLE_DCchromtab) //32 short
#define TABLE_DCchromtab1_ADDR                  			(32 + TABLE_DCchromtab) //32 short

//TABLE_AClumtab
#define TABLE_AClumtab                          			(7632 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define TABLE_AClumtab0_ADDR                    			(0 + TABLE_AClumtab) //64 short
#define TABLE_AClumtab1_ADDR                    			(64 + TABLE_AClumtab) //96 short
#define TABLE_AClumtab2_ADDR                    			(160 + TABLE_AClumtab) //64 short
#define TABLE_AClumtab3_ADDR                    			(224 + TABLE_AClumtab) //256 short

//TABLE_ACchromtab
#define TABLE_ACchromtab                        			(8112 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define TABLE_ACchromtab0_ADDR                  			(0 + TABLE_ACchromtab) //64 short
#define TABLE_ACchromtab1_ADDR                  			(64 + TABLE_ACchromtab) //128 short
#define TABLE_ACchromtab2_ADDR                  			(192 + TABLE_ACchromtab) //64 short
#define TABLE_ACchromtab3_ADDR                  			(256 + TABLE_ACchromtab) //256 short

//TABLE_OtherConstant
#define TABLE_OtherConstant                     			(8624 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define TABLE_inverse_Zig_Zag_ADDR              			(0 + TABLE_OtherConstant) //64 short
#define TABLE_add_para_ADDR                     			(64 + TABLE_OtherConstant) //320 short
#define GLOBAL_IDCT_BUFFER_Y_ADDR               			(384 + TABLE_OtherConstant) //8 short

//JPEG_GLOBAL_Y_VAR
#define JPEG_GLOBAL_Y_VAR                       			(9016 + JPEG_GLOBAL_Y_BEGIN_ADDR)
#define GLOBAL_lpJpegBuf_ADDR                   			(0 + JPEG_GLOBAL_Y_VAR) //448 short
#define GLOBAL_output_buf_ADDR                  			(448 + JPEG_GLOBAL_Y_VAR) //640 short


/******************
 **  JPEG_LOCAL  **
 ******************/

/***********************
 **  H263zoom_GLOBAL  **
 ***********************/

//GLOBAL_VARS
#define H263_ZOOM_FOR_MP3_AMR_RAM_X_ALIGNED     			(0 + H263zoom_GLOBAL_X_BEGIN_ADDR) //3244 short
#define CONST_TabV2R_ADDR                       			(3244 + H263zoom_GLOBAL_X_BEGIN_ADDR) //256 short
#define CONST_TableV2G_ADDR                     			(3500 + H263zoom_GLOBAL_X_BEGIN_ADDR) //256 short
#define CONST_TabU2G_ADDR                       			(3756 + H263zoom_GLOBAL_X_BEGIN_ADDR) //256 short
#define CONST_TabU2B_ADDR                       			(4012 + H263zoom_GLOBAL_X_BEGIN_ADDR) //256 short
#define CONST_H263_1_ADDR                       			(CONST_TableV2G_ADDR+2)
#define CONST_H263_2_ADDR                       			(CONST_TableV2G_ADDR+3)
#define GLOBAL_TEMP_PTMP2_ADDR                  			(4268 + H263zoom_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_TEMP_PTMP3_ADDR                  			(4269 + H263zoom_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_TEMP_PTMP4_ADDR                  			(4270 + H263zoom_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_BUF_OUT_ADDR                     			(4271 + H263zoom_GLOBAL_X_BEGIN_ADDR) //1 short
#define GLOBAL_TEMP_BUF_ADDR                    			((GLOBAL_BUF_OUT_ADDR+1))
#define GLOBAL_OUTPUT_YUV2RGB_ADDR              			(4272 + H263zoom_GLOBAL_X_BEGIN_ADDR) //4000 short

//GLOBAL_VARS
#define H263_ZOOM_FOR_MP3_AMR_RAM_Y_ALIGNED     			(0 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //7456 short
#define GLOBAL_INLINE_NUM_1_ADDR                			(7456 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_INLINE_NUM_2_ADDR                			(7457 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define TEMP_ADDRY_ADDR                         			(7458 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define TEMP_ADDRU_ADDR                         			(7459 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define TEMP_ADDRV_ADDR                         			(7460 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_BUFIN_Y_ADDR                     			(7461 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_BUFIN_U_ADDR                     			(7462 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_BUFIN_V_ADDR                     			(7463 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SRCW_HALF_ADDR                   			(7464 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SRCW_QUA_ADDR                    			(7465 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_NUM1_ADDR                        			(7466 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_NUM3_ADDR                        			(7467 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_MASK_ADDR                        			(7468 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_DMAOUT_SIZE_ADDR                 			(7470 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_DEAD_OUT_LINE_ADDR               			(7471 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_DMAOUT_INC_ADDR                  			(7472 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_INLINE_COUNT_ADDR                			(7474 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_INLINE_COUNT_UV_ADDR             			(7475 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_DMAIN_SIZE_ADDR                  			(7476 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_DMAIN_SIZE_UV_ADDR               			(7478 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SCALEX_SIZE_ADDR                 			(7480 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SCALEY_SIZE_ADDR                 			(7482 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_P1_ADDR                          			(7484 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_P2_ADDR                          			(7485 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOABL_N_ADDR                           			(7486 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_RESERVED_ADDR                    			(7487 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_OUTLINE_ADDR                     			(7488 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_ROTATE_POINTER_ADDR              			(7489 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_BUF_IN_ADDR                      			(7490 + H263zoom_GLOBAL_Y_BEGIN_ADDR) //2400 short

/**********************
 **  H263zoom_LOCAL  **
 **********************/

//Coolsand_Set_RotatePara
#define Coolsand_Set_RotatePara_Y_BEGIN_ADDR    			(0 + H263zoom_LOCAL_Y_BEGIN_ADDR)
#define TMP_BUF_FOR_ROTATE                      			(0 + Coolsand_Set_RotatePara_Y_BEGIN_ADDR) //32 short
#define TMP_BUF_FOR_ROTATE2                     			(32 + Coolsand_Set_RotatePara_Y_BEGIN_ADDR) //32 short

/*********************
 **  SBCDEC_GLOBAL  **
 *********************/

//GLOBAL_VARS
#define SBCDEC_RAM_X_ALIGNED                    			(0 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //0 short
#define SBC_DECODE_CRC_TABLE_ADDR               			(0 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //128 short
#define SBC_DECODE_CONST_1                      			(128 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_2                      			(129 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_4                      			(130 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_8                      			(131 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_16                     			(132 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_0                      			(133 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_3                      			(134 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_32                     			(135 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_n16                    			(136 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_7                      			(137 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_32BIT_1                			(138 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DECODE_CONST_340                    			(140 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_320                    			(141 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_OFFSET4                			(142 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //16 short
#define SBC_DECODE_CONST_OFFSET8                			(158 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //32 short
#define SBC_44_1K_TO_48K_FIR_COEF               			(190 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //3200 short
#define SBC_DECODE_CONST_147                    			(3390 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_160                    			(3391 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_0x00007fff             			(3392 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DECODE_CONST_0xffff8000             			(3394 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_EQ_DBgain                       			(3396 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //44 short
#define SBC_DEC_EQ_DBreci                       			(3440 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //44 short
#define SBC_DEC_EQ_ALPHA                        			(3484 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //40 short
#define SBC_DEC_EQ_NUM1                         			(3524 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //40 short
#define SBC_DEC_EQ_CONST_0                      			(3564 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_1                      			(3565 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_2                      			(3566 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_3                      			(3567 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_4                      			(3568 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_6                      			(3569 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_7                      			(3570 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_8                      			(3571 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_9                      			(3572 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_10                     			(3573 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_12                     			(3574 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_16                     			(3575 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_24                     			(3576 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_30                     			(3577 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_48                     			(3578 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_VOID                   			(3579 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_EQ_CONST_MAX                    			(3580 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_EQ_CONST_MIN                    			(3582 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_EQ_GAIN                         			(3584 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //80 short
#define SBC_DEC_dequant_long_scaled             			(3664 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //34 short
#define SBC_DEQUANT_LONG_SCALED_OFFSET1         			(3698 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEQUANT_LONG_SCALED_OFFSET2         			(3700 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_AAN_C4_FIX                      			(3702 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_AAN_C6_FIX                      			(3704 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_AAN_Q0_FIX                      			(3706 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_AAN_Q1_FIX                      			(3708 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DEC_DCTII_4_K06_FIX                 			(3710 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_DCTII_4_K08_FIX                 			(3711 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_DCTII_4_K09_FIX                 			(3712 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DEC_DCTII_4_K10_FIX                 			(3713 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_0x7fff                 			(3714 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_128                    			(3715 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short
#define SBC_DECODE_CONST_0x00008000             			(3716 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DECODE_CONST_0x00004000             			(3718 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DECODE_CONST_0x00001000             			(3720 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //2 short
#define SBC_DECODE_CONST_sub8filter_cef         			(3722 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //74 short
#define SBC_DECODE_CONST_sub4filter_cef         			(3796 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //26 short
#define SBCDEC_CONST_X_END                      			((SBC_DECODE_CONST_sub4filter_cef+26))
#define FOR_SBCDEC_DRC_Previous_Frame_ADDR      			(3822 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1120 short
#define SBCDEC_NO_USED_X_ADDR                   			(4942 + SBCDEC_GLOBAL_X_BEGIN_ADDR) //1 short

//GLOBAL_VARS
#define SBCDEC_INPUTSTREAM_LENGTH_ADDR          			(0 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_INPUTSTREAM_BUF_ADDR             			(2 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_OUTPUTSTREAM_LENGTH_ADDR         			(4 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_OUTPUTSTREAM_BUF_ADDR            			(6 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_FRAME_FREQUENCY                 			(8 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_BLOCK_MODE                			(9 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_BLOCKS                    			(10 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_CHANNEL_MODE              			(11 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_CHANNELS                  			(12 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_ALLOC_MODE                			(13 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_SUBBAND_MODE              			(14 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_SUBBANDS                  			(15 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_BITPOOL                   			(16 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_JOINT                     			(17 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_CRC                       			(18 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_CONSUMED_LENGTH           			(19 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_SAMPLE_LENGTH             			(20 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_INIT                      			(21 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_DATA_LEFT_LENGTH          			(22 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_DATA_CURRENT_ADDR         			(23 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_COUNT                     			(24 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_TEMP_1                    			(25 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_CURRENT_DATA              			(26 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_FRAME_CRC_POS                   			(28 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_CONSUMED                  			(29 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_SCALE_FACTOR              			(30 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //16 short
#define SBC_DEC_FRAME_BITS                      			(46 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //16 short
#define SBC_DEC_FRAME_LEVELS                    			(62 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //16 short
#define SBC_DEC_TEMP_BITNEED                    			(78 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //16 short
#define SBC_DEC_FRAME_SB_SAMPLE                 			(94 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //512 short
#define SBC_DEC_FRAME_PCM_SAMPLE                			(606 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //256 short
#define SBC_DEC_FRAME_SB_SAMPLE_F               			(862 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //512 short
#define SBC_DEC_STATE_V                         			(1374 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //438 short
#define SBC_DEC_STATE_OFFSET                    			(1812 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_STATE_Filter                    			(1814 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //256 short
#define SBC_DEC_Filter_L00                      			(2070 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_Filter_L01                      			(2072 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_Filter_L02                      			(2074 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_Filter_L03                      			(2076 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_Filter_L04                      			(2078 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_Filter_L05                      			(2080 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_Filter_L06                      			(2082 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_Filter_L07                      			(2084 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_FRAME_CRC_HEADER                			(2086 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //22 short
#define SBC_DEC_FRAME_TEMP1                     			(2108 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DEC_FRAME_TEMP2                     			(2109 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_DECODE_INPUTSTREAM_ADDR             			(2110 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1024 short
#define sbc_mixing_audio_flag                   			(3134 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define sbc_support_kalaok_flag                 			(3136 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DECODE_FREQUENCY_CALIB              			(3138 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //4 short
#define GLOBAL_DIGITAL_GAIN_SBCDEC_ADDR         			(3142 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_FILTER_w01                      			(3144 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //40 short
#define SBC_DEC_FILTER_y01                      			(3184 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //28 short
#define SBC_DEC_FILTER_w2                       			(3212 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //4 short
#define SBC_DEC_FILTER_y2                       			(3216 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //4 short
#define sbc_detect_pval_addr                    			(3220 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define sbc_detect_nval_addr                    			(3222 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_PCM_OUT_ADDR_begain             			(3224 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_PCM_OUT_ADDR_end                			(3226 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_DEC_FILTER_gain                     			(3228 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_48K_SET_DMA_OUT_LENGTH_FLAG         			(3230 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_48K_DATA_DMA_OUT_LENGTH             			(3232 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_44_1K_DATA_LENGTH                   			(3234 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define sbc_detect_mute_addr                    			(3236 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBC_44_1K_TO_48K_ENABLE_FLAG            			(3238 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_44_1K_TO_48K_InterPhase             			(3239 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_44_1K_TO_48K_buffer                 			(3240 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //40 short
#define SBC_44_1K_TO_48K_PCM_SAMPLE             			(3280 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //280 short
#define SBC_44_1K_TO_48K_count                  			(3560 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_44_1K_TO_48K_REMAIN_LENGTH          			(3561 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBC_44_1K_TO_48K_REMAIN_buffer          			(3562 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //560 short
#define GLOBAL_SBCDEC_IIR_BAND_NUM0_USED_ADDR_ADDR			(4122 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SBCDEC_IIR_BAND_DEN0_USED_ADDR_ADDR			(4123 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_IIR_INPUT_ADDR_ADDR              			(4124 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_IIR_INPUT_RESERVED               			(4125 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SBCDEC_IIR_BAND_NUM0_ADDR        			(4126 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND_NUM1_ADDR        			(4128 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND_NUM2_ADDR        			(4130 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND_DEN0_ADDR        			(4132 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND_DEN1_ADDR        			(4134 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND_DEN2_ADDR        			(4136 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND_XN_1_USED_ADDR_ADDR			(4138 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SBCDEC_IIR_BAND_XN_2_USED_ADDR_ADDR			(4139 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SBCDEC_IIR_BAND_YN_1_USED_ADDR_ADDR			(4140 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SBCDEC_IIR_BAND_YN_2_USED_ADDR_ADDR			(4141 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_SBCDEC_IIR_BAND0_XN_1_ADDR       			(4142 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND0_XN_2_ADDR       			(4144 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND0_YN_1_ADDR       			(4146 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND0_YN_2_ADDR       			(4148 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1_XN_1_ADDR       			(4150 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1_XN_2_ADDR       			(4152 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1_YN_1_ADDR       			(4154 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1_YN_2_ADDR       			(4156 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2_XN_1_ADDR       			(4158 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2_XN_2_ADDR       			(4160 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2_YN_1_ADDR       			(4162 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2_YN_2_ADDR       			(4164 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3_XN_1_ADDR       			(4166 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3_XN_2_ADDR       			(4168 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3_YN_1_ADDR       			(4170 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3_YN_2_ADDR       			(4172 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4_XN_1_ADDR       			(4174 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4_XN_2_ADDR       			(4176 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4_YN_1_ADDR       			(4178 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4_YN_2_ADDR       			(4180 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5_XN_1_ADDR       			(4182 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5_XN_2_ADDR       			(4184 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5_YN_1_ADDR       			(4186 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5_YN_2_ADDR       			(4188 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6_XN_1_ADDR       			(4190 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6_XN_2_ADDR       			(4192 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6_YN_1_ADDR       			(4194 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6_YN_2_ADDR       			(4196 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7_XN_1_ADDR       			(4198 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7_XN_2_ADDR       			(4200 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7_YN_1_ADDR       			(4202 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7_YN_2_ADDR       			(4204 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8_XN_1_ADDR       			(4206 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8_XN_2_ADDR       			(4208 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8_YN_1_ADDR       			(4210 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8_YN_2_ADDR       			(4212 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9_XN_1_ADDR       			(4214 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9_XN_2_ADDR       			(4216 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9_YN_1_ADDR       			(4218 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9_YN_2_ADDR       			(4220 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND0R_XN_1_ADDR      			(4222 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND0R_XN_2_ADDR      			(4224 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND0R_YN_1_ADDR      			(4226 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND0R_YN_2_ADDR      			(4228 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1R_XN_1_ADDR      			(4230 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1R_XN_2_ADDR      			(4232 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1R_YN_1_ADDR      			(4234 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND1R_YN_2_ADDR      			(4236 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2R_XN_1_ADDR      			(4238 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2R_XN_2_ADDR      			(4240 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2R_YN_1_ADDR      			(4242 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND2R_YN_2_ADDR      			(4244 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3R_XN_1_ADDR      			(4246 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3R_XN_2_ADDR      			(4248 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3R_YN_1_ADDR      			(4250 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND3R_YN_2_ADDR      			(4252 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4R_XN_1_ADDR      			(4254 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4R_XN_2_ADDR      			(4256 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4R_YN_1_ADDR      			(4258 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND4R_YN_2_ADDR      			(4260 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5R_XN_1_ADDR      			(4262 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5R_XN_2_ADDR      			(4264 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5R_YN_1_ADDR      			(4266 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND5R_YN_2_ADDR      			(4268 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6R_XN_1_ADDR      			(4270 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6R_XN_2_ADDR      			(4272 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6R_YN_1_ADDR      			(4274 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND6R_YN_2_ADDR      			(4276 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7R_XN_1_ADDR      			(4278 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7R_XN_2_ADDR      			(4280 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7R_YN_1_ADDR      			(4282 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND7R_YN_2_ADDR      			(4284 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8R_XN_1_ADDR      			(4286 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8R_XN_2_ADDR      			(4288 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8R_YN_1_ADDR      			(4290 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND8R_YN_2_ADDR      			(4292 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9R_XN_1_ADDR      			(4294 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9R_XN_2_ADDR      			(4296 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9R_YN_1_ADDR      			(4298 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_SBCDEC_IIR_BAND9R_YN_2_ADDR      			(4300 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_IIR_CLIP_MAX_SHORT               			(4302 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_IIR_CLIP_MIN_SHORT               			(4304 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_IIR_PCM_ROUND                    			(4306 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_IIR_ENABLE_FLAG_ADDR             			(4308 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_IIR_ENABLE_RESERVED              			(4309 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
//m_SBCDEC_IIRProcPara_ADDR
#define m_SBCDEC_IIRProcPara_ADDR               			(4310 + SBCDEC_GLOBAL_Y_BEGIN_ADDR)
#define m_SBCDEC_IIRProcPara_band0_coeffs       			(0 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band1_coeffs       			(12 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band2_coeffs       			(24 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band3_coeffs       			(36 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band4_coeffs       			(48 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band5_coeffs       			(60 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band6_coeffs       			(72 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band7_coeffs       			(84 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band8_coeffs       			(96 + m_SBCDEC_IIRProcPara_ADDR) //12 short
#define m_SBCDEC_IIRProcPara_band9_coeffs       			(108 + m_SBCDEC_IIRProcPara_ADDR) //12 short

#define SBCDEC_BAND_INFO_ENABLE_FLAG_ADDR       			(4430 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_BAND_INFO_RESERVED               			(4431 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
//m_SBCDEC_BAND_INFO_ADDR
#define m_SBCDEC_BAND_INFO_ADDR                 			(4432 + SBCDEC_GLOBAL_Y_BEGIN_ADDR)
#define SBCDEC_BAND1_INFO                       			(0 + m_SBCDEC_BAND_INFO_ADDR) //2 short
#define SBCDEC_BAND2_INFO                       			(2 + m_SBCDEC_BAND_INFO_ADDR) //2 short
#define SBCDEC_BAND3_INFO                       			(4 + m_SBCDEC_BAND_INFO_ADDR) //2 short
#define SBCDEC_BAND4_INFO                       			(6 + m_SBCDEC_BAND_INFO_ADDR) //2 short
#define SBCDEC_BAND5_INFO                       			(8 + m_SBCDEC_BAND_INFO_ADDR) //2 short
#define SBCDEC_BAND6_INFO                       			(10 + m_SBCDEC_BAND_INFO_ADDR) //2 short
#define SBCDEC_BAND7_INFO                       			(12 + m_SBCDEC_BAND_INFO_ADDR) //2 short
#define SBCDEC_BAND8_INFO                       			(14 + m_SBCDEC_BAND_INFO_ADDR) //2 short

#define SBCDEC_OUTPUT_PCM_BUFFER1               			(4448 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2240 short
#define SBCDEC_OUTPUT_PCM_BUFFER1_END           			((SBCDEC_OUTPUT_PCM_BUFFER1+2240))
#define FOR_SBCDEC_DRC_RAM_Y_ALIGEND            			(6688 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //768 short
#define FOR_SBC_EQ_MODEL_ADDR                   			(7456 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1478 short
//SBCDEC_GLOBAL_STRUCT_DRC_PARAM
#define SBCDEC_GLOBAL_STRUCT_DRC_PARAM          			(8934 + SBCDEC_GLOBAL_Y_BEGIN_ADDR)
#define SBCDEC_DRC_G_Y_alc_enable               			(0 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_thres              			(1 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_width              			(2 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_R                  			(3 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_R1                 			(4 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_R2                 			(5 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_limit              			(6 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_M                  			(7 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_alpha1             			(8 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_alpha2             			(9 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_noise_gate         			(10 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_alpha_max          			(11 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_Thr_dB             			(12 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_mm_gain            			(13 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_param_channel            			(14 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define SBCDEC_DRC_G_Y_reserved                 			(15 + SBCDEC_GLOBAL_STRUCT_DRC_PARAM) //1 short

#define SBCDEC_DRC_G_Y_alg_gain                 			(8950 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_DRC_G_Y_crosszero_window         			(8951 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_DRC_G_Y_peak_last1               			(8952 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_DRC_G_Y_peak_last2               			(8954 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define SBCDEC_DRC_G_Y_history_peak1            			(8956 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_DRC_G_Y_history_peak2            			(8957 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_DRC_G_Y_crosszero_offset1        			(8958 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_DRC_G_Y_crosszero_offset2        			(8959 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_DRC_G_Y_gain_smooth1             			(8960 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_DRC_G_Y_gain_smooth2             			(8961 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define SBCDEC_NO_USED_Y_ADDR                   			(8962 + SBCDEC_GLOBAL_Y_BEGIN_ADDR) //1 short

/********************
 **  SBCDEC_LOCAL  **
 ********************/

/*********************
 **  SBCENC_GLOBAL  **
 *********************/

//GLOBAL_VARS
#define SBCENC_FOR_MP3_AMR_RAM_X_ALIGNED        			(0 + SBCENC_GLOBAL_X_BEGIN_ADDR) //5548 short
#define SMALL_FILTER_IMP_ADDR                   			(5548 + SBCENC_GLOBAL_X_BEGIN_ADDR) //1536 short
#define Const_anamatrix8                        			(7084 + SBCENC_GLOBAL_X_BEGIN_ADDR) //8 short
#define Const_crc_table                         			(7092 + SBCENC_GLOBAL_X_BEGIN_ADDR) //128 short
#define Const_sbc_offset8                       			(7220 + SBCENC_GLOBAL_X_BEGIN_ADDR) //32 short
#define Const_sbc_proto_8                       			(7252 + SBCENC_GLOBAL_X_BEGIN_ADDR) //70 short
#define Const_in_index                          			(7322 + SBCENC_GLOBAL_X_BEGIN_ADDR) //74 short
#define Const_t_index                           			(7396 + SBCENC_GLOBAL_X_BEGIN_ADDR) //8 short
#define CONST_SBC_32bit_1_ADDR                  			((SMALL_FILTER_IMP_ADDR+1418))
#define CONST_SBC_1_ADDR                        			((Const_in_index+9))
#define CONST_SBC_2_ADDR                        			((Const_t_index+4))
#define CONST_SBC_16_ADDR                       			((Const_in_index+2))
#define CONST_NEG_16_ADDR                       			((SMALL_FILTER_IMP_ADDR+1129))

//GLOBAL_VARS
#define SBCENC_FOR_MP3_AMR_RAM_Y_ALIGNED        			(0 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //7456 short
#define STRUCT_sbc_frame_channels               			(Output_nbChannel_addr)
#define SMALL_FILTER_IMPD_ADDR                  			(7456 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1536 short
#define CONST_Pmask_ADDR                        			(8992 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define CONST_D_Npc_ADDR                        			(8994 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define CONST_SBC_reseverd2_ADDR                			(8996 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //4 short
#define GLOBAL_OUTPUT_ADDR_ADDR                 			(9000 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define CONST_SBC_reseverd_ADDR                 			(9001 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define STRUCT_sbc_INPUT_ADDR                   			(9002 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define STRUCT_sbc_INPUT_END_ADDR               			(9003 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Tmp_crc_pos_addr                        			(9004 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define Tmp_produced_addr                       			(9005 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_sbc_encoder_state_position       			(9006 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_sbc_encoder_state_position1      			((GLOBAL_sbc_encoder_state_position+1))
#define GLOBAL_sbc_encoder_state_X              			(9008 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //640 short
#define GLOBAL_sbc_encoder_state_X_ch1          			((GLOBAL_sbc_encoder_state_X+320))
#define RESAMPLE_TOTAL_LEN_ADDR                 			(9648 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define RESAMPLE_USED_LEN_ADDR                  			(9649 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define RESAMPLE_VPTR_ADDR                      			(9650 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define RESAMPLE_FLAG_ADDR                      			(9651 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define RESAMPLE_INC_ADDR                       			(9652 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define RESAMPLE_OFS_ADDR                       			(9654 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define RESAMPLE_SRC_TMP_ADDR                   			(9656 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //264 short
#define RESAMPLE_X1_ADDR                        			(RESAMPLE_SRC_TMP_ADDR)
#define RESAMPLE_X2_ADDR                        			((RESAMPLE_SRC_TMP_ADDR+128+4))
#define RESAMPLE_Nx_ADDR                        			(9920 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define RESAMPLE_BuffSize_ADDR                  			(9921 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define LOCAL_SrcUp_dtb_ADDR                    			(9922 + SBCENC_GLOBAL_Y_BEGIN_ADDR) //2 short

/********************
 **  SBCENC_LOCAL  **
 ********************/

//CS_sbc_encode
#define CS_sbc_encode_X_BEGIN_ADDR              			(0 + SBCENC_LOCAL_X_BEGIN_ADDR)
#define SBC_sb_sample_f                         			(0 + CS_sbc_encode_X_BEGIN_ADDR) //512 short
#define SBC_sb_sample_f_ch1                     			((SBC_sb_sample_f+16))
#define Out_put_addr                            			(512 + CS_sbc_encode_X_BEGIN_ADDR) //1000 short
#define Out_put_addr_1                          			((Out_put_addr+1))

//CS_ResampleAudio_SBCEnc
#define CS_ResampleAudio_SBCEnc_X_BEGIN_ADDR    			(1512 + SBCENC_LOCAL_X_BEGIN_ADDR)
#define LOCAL_SrcUp_endTime_ADDR                			(0 + CS_ResampleAudio_SBCEnc_X_BEGIN_ADDR) //2 short

//CS_sbc_analyze_audio
#define CS_sbc_analyze_audio_X_BEGIN_ADDR       			(1514 + SBCENC_LOCAL_X_BEGIN_ADDR)
#define SBC_pcm_sample                          			(0 + CS_sbc_analyze_audio_X_BEGIN_ADDR) //256 short
#define SBC_pcm_sample_ch1                      			((SBC_pcm_sample+128))

//CS_SBC_PackFrame
#define CS_SBC_PackFrame_X_BEGIN_ADDR           			(1514 + SBCENC_LOCAL_X_BEGIN_ADDR)
#define SBC_scale_factor                        			(0 + CS_SBC_PackFrame_X_BEGIN_ADDR) //16 short
#define SBC_scale_factor_ch1                    			((SBC_scale_factor+1))
#define Tmp_Bits                                			(16 + CS_SBC_PackFrame_X_BEGIN_ADDR) //16 short
#define Tmp_Bits_ch1                            			((Tmp_Bits + 1))

//CS_SBC_PackFrame
#define CS_SBC_PackFrame_Y_BEGIN_ADDR           			(0 + SBCENC_LOCAL_Y_BEGIN_ADDR)
#define sb_sample_j                             			(0 + CS_SBC_PackFrame_Y_BEGIN_ADDR) //64 short
#define sb_sample_j_ch1                         			((sb_sample_j + 2))
#define Tmp_Scale_factor                        			(64 + CS_SBC_PackFrame_Y_BEGIN_ADDR) //32 short
#define Tmp_Scale_factor_ch1                    			((Tmp_Scale_factor+2))
#define Tmp_Levels                              			(96 + CS_SBC_PackFrame_Y_BEGIN_ADDR) //32 short
#define Tmp_Levels_ch1                          			((Tmp_Levels + 2))
#define Tmp_crc_header_addr                     			(128 + CS_SBC_PackFrame_Y_BEGIN_ADDR) //11 short
#define STRUCT_sbc_frame_joint                  			(139 + CS_SBC_PackFrame_Y_BEGIN_ADDR) //1 short

//CS_sbc_analyze_eight
#define CS_sbc_analyze_eight_X_BEGIN_ADDR       			(1770 + SBCENC_LOCAL_X_BEGIN_ADDR)
#define Tmp_T_addr                              			(0 + CS_sbc_analyze_eight_X_BEGIN_ADDR) //16 short

//CS_sbc_analyze_eight
#define CS_sbc_analyze_eight_Y_BEGIN_ADDR       			(0 + SBCENC_LOCAL_Y_BEGIN_ADDR)
#define Tmp_S_addr                              			(0 + CS_sbc_analyze_eight_Y_BEGIN_ADDR) //16 short

//CS_SBC_Calculate_Bits
#define CS_SBC_Calculate_Bits_X_BEGIN_ADDR      			(1546 + SBCENC_LOCAL_X_BEGIN_ADDR)
#define Tmp_bitneed_addr                        			(0 + CS_SBC_Calculate_Bits_X_BEGIN_ADDR) //16 short
#define Tmp_bitneed_addr_ch1                    			((Tmp_bitneed_addr+1))

/******************
 **  DRC_GLOBAL  **
 ******************/

//GLOBAL_VARS
#define DRC_FOR_DECODER_RAM_X_ALIGNED           			(0 + DRC_GLOBAL_X_BEGIN_ADDR) //5548 short
#define DRC_G_X_drc_input_buf_addr              			(5548 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_drc_previous_frame_addr         			(5549 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_in_pcm_len                      			(5550 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_frame_len                       			(5551 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_CrossZeroSize         			(5552 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_gain_amp              			(5553 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_output_ptr            			(5554 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_historydata_ptr       			(5555 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_frame                 			(5556 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_shift_num             			(5557 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_crosszeropeak         			(5558 + DRC_GLOBAL_X_BEGIN_ADDR) //2 short
#define DRC_G_X_music_drc_crosszeroindex_i      			(5560 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_size                  			(5561 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_historydata_cur_ptr   			(5562 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_music_drc_pow10_fxp_Q           			(5563 + DRC_GLOBAL_X_BEGIN_ADDR) //1 short
#define DRC_G_X_drc_historydata_buf             			(5564 + DRC_GLOBAL_X_BEGIN_ADDR) //2304 short
#define DRC_G_X_music_drc_CrossZeroPeakBuffer   			(7868 + DRC_GLOBAL_X_BEGIN_ADDR) //1152 short
#define DRC_G_X_music_drc_instant_gain          			(9020 + DRC_GLOBAL_X_BEGIN_ADDR) //1152 short

//GLOBAL_VARS
#define EQ_FOR_DECODER_RAM_Y_ALIGNED            			(0 + DRC_GLOBAL_Y_BEGIN_ADDR) //7456 short
#define GLOBAL_IIR_BAND_NUM0_USED_ADDR_ADDR     			(7456 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_IIR_BAND_DEN0_USED_ADDR_ADDR     			(7457 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define IIR_INPUT_ADDR_ADDR                     			(7458 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define IIR_INPUT_RESERVED                      			(7459 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_IIR_BAND_NUM0_ADDR               			(7460 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND_NUM1_ADDR               			(7462 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND_NUM2_ADDR               			(7464 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND_DEN0_ADDR               			(7466 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND_DEN1_ADDR               			(7468 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND_DEN2_ADDR               			(7470 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND_XN_1_USED_ADDR_ADDR     			(7472 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_IIR_BAND_XN_2_USED_ADDR_ADDR     			(7473 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_IIR_BAND_YN_1_USED_ADDR_ADDR     			(7474 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_IIR_BAND_YN_2_USED_ADDR_ADDR     			(7475 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define GLOBAL_IIR_BAND0_XN_1_ADDR              			(7476 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND0_XN_2_ADDR              			(7478 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND0_YN_1_ADDR              			(7480 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND0_YN_2_ADDR              			(7482 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1_XN_1_ADDR              			(7484 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1_XN_2_ADDR              			(7486 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1_YN_1_ADDR              			(7488 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1_YN_2_ADDR              			(7490 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2_XN_1_ADDR              			(7492 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2_XN_2_ADDR              			(7494 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2_YN_1_ADDR              			(7496 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2_YN_2_ADDR              			(7498 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3_XN_1_ADDR              			(7500 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3_XN_2_ADDR              			(7502 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3_YN_1_ADDR              			(7504 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3_YN_2_ADDR              			(7506 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4_XN_1_ADDR              			(7508 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4_XN_2_ADDR              			(7510 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4_YN_1_ADDR              			(7512 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4_YN_2_ADDR              			(7514 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5_XN_1_ADDR              			(7516 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5_XN_2_ADDR              			(7518 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5_YN_1_ADDR              			(7520 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5_YN_2_ADDR              			(7522 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6_XN_1_ADDR              			(7524 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6_XN_2_ADDR              			(7526 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6_YN_1_ADDR              			(7528 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6_YN_2_ADDR              			(7530 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7_XN_1_ADDR              			(7532 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7_XN_2_ADDR              			(7534 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7_YN_1_ADDR              			(7536 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7_YN_2_ADDR              			(7538 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8_XN_1_ADDR              			(7540 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8_XN_2_ADDR              			(7542 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8_YN_1_ADDR              			(7544 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8_YN_2_ADDR              			(7546 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9_XN_1_ADDR              			(7548 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9_XN_2_ADDR              			(7550 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9_YN_1_ADDR              			(7552 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9_YN_2_ADDR              			(7554 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND0R_XN_1_ADDR             			(7556 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND0R_XN_2_ADDR             			(7558 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND0R_YN_1_ADDR             			(7560 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND0R_YN_2_ADDR             			(7562 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1R_XN_1_ADDR             			(7564 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1R_XN_2_ADDR             			(7566 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1R_YN_1_ADDR             			(7568 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND1R_YN_2_ADDR             			(7570 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2R_XN_1_ADDR             			(7572 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2R_XN_2_ADDR             			(7574 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2R_YN_1_ADDR             			(7576 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND2R_YN_2_ADDR             			(7578 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3R_XN_1_ADDR             			(7580 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3R_XN_2_ADDR             			(7582 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3R_YN_1_ADDR             			(7584 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND3R_YN_2_ADDR             			(7586 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4R_XN_1_ADDR             			(7588 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4R_XN_2_ADDR             			(7590 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4R_YN_1_ADDR             			(7592 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND4R_YN_2_ADDR             			(7594 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5R_XN_1_ADDR             			(7596 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5R_XN_2_ADDR             			(7598 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5R_YN_1_ADDR             			(7600 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND5R_YN_2_ADDR             			(7602 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6R_XN_1_ADDR             			(7604 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6R_XN_2_ADDR             			(7606 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6R_YN_1_ADDR             			(7608 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND6R_YN_2_ADDR             			(7610 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7R_XN_1_ADDR             			(7612 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7R_XN_2_ADDR             			(7614 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7R_YN_1_ADDR             			(7616 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND7R_YN_2_ADDR             			(7618 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8R_XN_1_ADDR             			(7620 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8R_XN_2_ADDR             			(7622 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8R_YN_1_ADDR             			(7624 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND8R_YN_2_ADDR             			(7626 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9R_XN_1_ADDR             			(7628 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9R_XN_2_ADDR             			(7630 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9R_YN_1_ADDR             			(7632 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define GLOBAL_IIR_BAND9R_YN_2_ADDR             			(7634 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define IIR_CLIP_MAX_SHORT                      			(7636 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define IIR_CLIP_MIN_SHORT                      			(7638 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define IIR_PCM_ROUND                           			(7640 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define IIR_ENABLE_FLAG_ADDR                    			(7642 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define IIR_ENABLE_RESERVED                     			(7643 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
//m_IIRProcPara_ADDR
#define m_IIRProcPara_ADDR                      			(7644 + DRC_GLOBAL_Y_BEGIN_ADDR)
#define m_IIRProcPara_band0_coeffs              			(0 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band1_coeffs              			(12 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band2_coeffs              			(24 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band3_coeffs              			(36 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band4_coeffs              			(48 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band5_coeffs              			(60 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band6_coeffs              			(72 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band7_coeffs              			(84 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band8_coeffs              			(96 + m_IIRProcPara_ADDR) //12 short
#define m_IIRProcPara_band9_coeffs              			(108 + m_IIRProcPara_ADDR) //12 short

#define IIR_PCM_BAND_ACTIVE_NUM                 			(7764 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define IIR_PCM_BUF_ADDR_ADDR                   			(7765 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define IIR_PCM_CH                              			(7766 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define IIR_PCM_LEN                             			(7767 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_FOR_DECODER_RAM_Y_ALIGNED           			(7768 + DRC_GLOBAL_Y_BEGIN_ADDR) //1166 short
//DRC_GLOBAL_STRUCT_DRC_PARAM
#define DRC_GLOBAL_STRUCT_DRC_PARAM             			(8934 + DRC_GLOBAL_Y_BEGIN_ADDR)
#define DRC_G_Y_alc_enable                      			(0 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_thres                     			(1 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_width                     			(2 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_R                         			(3 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_R1                        			(4 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_R2                        			(5 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_limit                     			(6 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_M                         			(7 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_alpha1                    			(8 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_alpha2                    			(9 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_noise_gate                			(10 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_alpha_max                 			(11 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_Thr_dB                    			(12 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_mm_gain                   			(13 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_param_channel                   			(14 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short
#define DRC_G_Y_reserved                        			(15 + DRC_GLOBAL_STRUCT_DRC_PARAM) //1 short

#define DRC_G_Y_alg_gain                        			(8950 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_G_Y_crosszero_window                			(8951 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_G_Y_peak_last1                      			(8952 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define DRC_G_Y_peak_last2                      			(8954 + DRC_GLOBAL_Y_BEGIN_ADDR) //2 short
#define DRC_G_Y_history_peak1                   			(8956 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_G_Y_history_peak2                   			(8957 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_G_Y_crosszero_offset1               			(8958 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_G_Y_crosszero_offset2               			(8959 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_G_Y_gain_smooth1                    			(8960 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
#define DRC_G_Y_gain_smooth2                    			(8961 + DRC_GLOBAL_Y_BEGIN_ADDR) //1 short
//DRC_GLOBAL_CONSTY_STRUCT
#define DRC_GLOBAL_CONSTY_STRUCT                			(8962 + DRC_GLOBAL_Y_BEGIN_ADDR)
#define DRC_G_Y_music_drc_log_table             			(0 + DRC_GLOBAL_CONSTY_STRUCT) //256 short
#define DRC_G_Y_pow10_fxp_const_table           			(256 + DRC_GLOBAL_CONSTY_STRUCT) //258 short
#define DRC_G_Y_pow10_fxp_const_tens_table      			(514 + DRC_GLOBAL_CONSTY_STRUCT) //10 short
#define DRC_G_Y_pow10_fxp_const_Q_table         			(524 + DRC_GLOBAL_CONSTY_STRUCT) //4 short
#define DRC_G_Y_music_drc_const_Q_20            			(528 + DRC_GLOBAL_CONSTY_STRUCT) //1 short
#define DRC_G_Y_music_drc_const_Q_09375         			(529 + DRC_GLOBAL_CONSTY_STRUCT) //1 short
#define DRC_G_Y_music_drc_const_Q_005           			(530 + DRC_GLOBAL_CONSTY_STRUCT) //1 short
#define DRC_G_Y_music_drc_const_4096            			(531 + DRC_GLOBAL_CONSTY_STRUCT) //1 short
#define DRC_G_Y_music_drc_const_1536            			(532 + DRC_GLOBAL_CONSTY_STRUCT) //1 short
#define DRC_G_Y_music_drc_const_3072            			(533 + DRC_GLOBAL_CONSTY_STRUCT) //1 short
#define DRC_G_Y_music_drc_const_32767           			(534 + DRC_GLOBAL_CONSTY_STRUCT) //2 short
#define DRC_G_Y_music_drc_const_32768           			(536 + DRC_GLOBAL_CONSTY_STRUCT) //2 short
#define DRC_G_Y_music_drc_const_0x40000000      			(538 + DRC_GLOBAL_CONSTY_STRUCT) //2 short
#define DRC_G_Y_music_drc_const_0x80000000      			(540 + DRC_GLOBAL_CONSTY_STRUCT) //2 short
#define DRC_G_Y_music_drc_const_0xc0000000      			(542 + DRC_GLOBAL_CONSTY_STRUCT) //2 short

#define DRC_G_Y_music_drc_CrossZeroIndex        			(9506 + DRC_GLOBAL_Y_BEGIN_ADDR) //10 short

/*****************
 **  DRC_LOCAL  **
 *****************/

/********************
 **  COMMON_LOCAL  **
 ********************/

//RAM_X: size 0x2800, used 0x27f0	RAM_Y: size 0x2800, used 0x27c4

#endif
