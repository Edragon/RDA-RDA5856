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
#ifndef MAP_ADDR_H
#define MAP_ADDR_H

/* This file defines all the variables as memory addresses.
   It's created by voc_map automatically.*/

/********************
 **  SECTION_SIZE  **
 ********************/

#define COMMON_GLOBAL_X_SIZE                              		122
#define COMMON_GLOBAL_Y_SIZE                              		202
#define COMMON_LOCAL_X_SIZE                               		0
#define COMMON_LOCAL_Y_SIZE                               		0

/**************************
 **  SECTION_BEGIN_ADDR  **
 **************************/
 
#define RAM_X_BEGIN_ADDR 0x0
#define RAM_Y_BEGIN_ADDR 0x4000

#define COMMON_GLOBAL_X_BEGIN_ADDR                        		(0 + RAM_X_BEGIN_ADDR)
#define COMMON_GLOBAL_Y_BEGIN_ADDR                        		(0 + RAM_Y_BEGIN_ADDR)
#define COMMON_LOCAL_X_BEGIN_ADDR                         		(122 + RAM_X_BEGIN_ADDR)
#define COMMON_LOCAL_Y_BEGIN_ADDR                         		(202 + RAM_Y_BEGIN_ADDR)

/*********************
 **  COMMON_GLOBAL  **
 *********************/

//GLOBAL_VARS
#define STATIC_MOTION_A                         			(0 + COMMON_GLOBAL_X_BEGIN_ADDR) //12 short
#define STATIC_TEMP1                            			(12 + COMMON_GLOBAL_X_BEGIN_ADDR) //12 short
#define STATIC_TEMP2                            			(24 + COMMON_GLOBAL_X_BEGIN_ADDR) //12 short
#define STATIC_MOUNT_1                          			(36 + COMMON_GLOBAL_X_BEGIN_ADDR) //12 short
#define STATIC_MOUNT_2                          			(48 + COMMON_GLOBAL_X_BEGIN_ADDR) //12 short
#define STATIC_ADD_SUB_MOD_T                    			(60 + COMMON_GLOBAL_X_BEGIN_ADDR) //12 short
#define STATIC_MUL_MODE_C_1                     			(72 + COMMON_GLOBAL_X_BEGIN_ADDR) //12 short
#define STATIC_STEP_NUM                         			(84 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define STATIC_STEP_NUM_2                       			(85 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_1                           			(86 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_2                           			(87 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_3                           			(88 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_4                           			(89 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_5                           			(90 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_6                           			(91 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_8                           			(92 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_16                          			(93 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_32                          			(94 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_64                          			(95 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_128                         			(96 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_SHORT_ff                          			(97 + COMMON_GLOBAL_X_BEGIN_ADDR) //1 short
#define CONST_32BIT_0                           			(98 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_1                           			(100 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_2                           			(102 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_4                           			(104 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_8                           			(106 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_16                          			(108 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_32                          			(110 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_64                          			(112 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_128                         			(114 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_256                         			(116 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_FFFFFFFE                    			(118 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define CONST_32BIT_FFFFFFFF                    			(120 + COMMON_GLOBAL_X_BEGIN_ADDR) //2 short
#define VOC_P192_MUL_MOD_Q1                     			(122 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define VOC_P192_MUL_MOD_Q1_SUP                 			(138 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short
#define VOC_P192_MUL_MOD_LOCAL_A1               			(154 + COMMON_GLOBAL_X_BEGIN_ADDR) //16 short

//GLOBAL_VARS
#define P192_FUNC_IN_GX                         			(0 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_FUNC_IN_GY                         			(12 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_FUNC_IN_K                          			(24 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_FUNC_OUT_PX                        			(36 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_FUNC_OUT_PY                        			(48 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_TX1                                			(60 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_TY1                                			(72 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_TX3                                			(84 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define P192_TY3                                			(96 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define VOC_P192_PNT_ADD_PNB_LOCAL_T3           			(108 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define VOC_P192_INV_MODE_LOCAL_TEMP            			(120 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define VOC_P192_MUL_MODE_LOCAL_M               			(132 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define VOC_P192_MUL_MODE_LOCAL_S               			(144 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define VOC_P192_MUL_MODE_LOCAL_TEMP            			(156 + COMMON_GLOBAL_Y_BEGIN_ADDR) //12 short
#define VOC_P192_MUL_TEMP1                      			(168 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define VOC_P192_MUL_TEMP2                      			(170 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define VOC_P192_MUL_TEMP3                      			(172 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define VOC_P192_MUL_TEMP4                      			(174 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define P192_STEPS                              			(176 + COMMON_GLOBAL_Y_BEGIN_ADDR) //2 short
#define VOC_P192_MUL_MOD_PARA1                  			(178 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_MUL_MOD_PARA2                  			(179 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_MUL_MOD_PARA3                  			(180 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_SUB_MOD_PARA1                  			(181 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_SUB_MOD_PARA2                  			(182 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_SUB_MOD_PARA3                  			(183 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_INV_MOD_PARA1                  			(184 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_INV_MOD_PARA2                  			(185 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_INV_MOD_PARA3                  			(186 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD_MOD_PARA1                  			(187 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD_MOD_PARA2                  			(188 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD_MOD_PARA3                  			(189 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD_PARA1                      			(190 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD_PARA2                      			(191 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD_PARA3                      			(192 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_SUB_PARA1                      			(193 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_SUB_PARA2                      			(194 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_SUB_PARA3                      			(195 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_MUL_PARA1                      			(196 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_MUL_PARA2                      			(197 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_MUL_PARA3                      			(198 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD64_PARA1                    			(199 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD64_PARA2                    			(200 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short
#define VOC_P192_ADD64_PARA3                    			(201 + COMMON_GLOBAL_Y_BEGIN_ADDR) //1 short

/********************
 **  COMMON_LOCAL  **
 ********************/

//RAM_X: size 0x2800, used 0x007a	RAM_Y: size 0x2800, used 0x00ca

#endif
