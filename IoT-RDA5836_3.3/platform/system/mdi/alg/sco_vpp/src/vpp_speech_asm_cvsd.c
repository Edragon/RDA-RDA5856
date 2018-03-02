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
#include "vppp_speech_common.h"
#include "vpp_speech_asm_cvsd.h"


void CVSD_LowPass_Filter1(void)
{ 
	VoC_push16(RA,DEFAULT);
	VoC_lw16i_short(FORMATX,0,DEFAULT);
	VoC_lw16i_short(FORMAT32,0,IN_PARALLEL);

	VoC_loop_i(0,2)
		VoC_lw16inc_p(REG4,REG0,DEFAULT);//input data;
		VoC_lw16inc_p(REG5,REG0,DEFAULT);//input data;
		VoC_lw32_d(ACC0,CVSD_LOWPASS_FILTER_ROUND);
		VoC_lw32_d(ACC1,CVSD_LOWPASS_FILTER_ROUND);
		VoC_bez16_d(CVSD_DEC_LOWPASS_filter1,CVSD_LOWPASS_DEC_ENC_FLAG);
			VoC_lw16i(REG6,1);
			VoC_sub16_rr(REG0,REG0,REG6,DEFAULT);
			VoC_movreg16(REG5,REG4,IN_PARALLEL);

CVSD_DEC_LOWPASS_filter1:
	    //x[n]
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB);//x(n)*a0;
		VoC_lw32_p(REG67,REG3,DEFAULT);//x(n-1);
		VoC_NOP();
	    VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n)*b0;

		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-1)=x(n);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+1);;//x(n-1)*a1;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-1)=x(n);
		VoC_NOP();
	    VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-1)*b1;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-2)=x(n-1);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+2);//x(n-2)*a2;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-2)=x(n-1);
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-2)*b2;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-3)=x(n-2);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+3);//x(n-3)*a3;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-3)=x(n-2);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-3)*b3;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-4)=x(n-3);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+4);//x(n-4)*a4;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-4)=x(n-3);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-4)*b4;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-5)=x(n-4);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+5);//x(n-5)*a5;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-5)=x(n-4);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-5)*b5;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-6)=x(n-5);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+6);//x(n-6)*a6;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-6)=x(n-5);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-6)*b6;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-7)=x(n-6);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+7);//x(n-7)*a7;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-7)=x(n-6);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-7)*b7;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-8)=x(n-7);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+8);//x(n-8)*a8;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-8)=x(n-7);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-4)*b4;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-9)=x(n-8);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+9);//x(n-9)*a9;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-9)=x(n-8);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-9)*b9;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-10)=x(n-9);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+10);//x(n-10)*a10;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-10)=x(n-9);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-10)*b10;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-11)=x(n-10);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+11);//x(n-11)*a11;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-11)=x(n-10);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-11)*b11;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-12)=x(n-11);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+12);//x(n-12)*a12;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-12)=x(n-11);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-12)*b12;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-13)=x(n-12);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+13);//x(n-13)*a13;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-13)=x(n-12);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-13)*b13;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-14)=x(n-13);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+14);//x(n-14)*a14;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-14)=x(n-13);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-14)*b14;

	    VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-15)=x(n-14);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+15);//x(n-15)*a15;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-15)=x(n-14);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-15)*b15;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-16)=x(n-15);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+16);//x(n-16)*a16;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-16)=x(n-15);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-16)*b16;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-17)=x(n-16);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+17);//x(n-17)*a17;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-17)=x(n-16);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-17)*b17;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-18)=x(n-17);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+18);//x(n-18)*a18;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-18)=x(n-17);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-18)*b18;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-19)=x(n-18);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+19);//x(n-19)*a19;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-19)=x(n-18);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-19)*b19;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-20)=x(n-19);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER1_Coef_TAB+20);//x(n-20)*a20;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-20)=x(n-19);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-20)*b20;

		
	    VoC_shr32_ri(ACC0,12,DEFAULT);
		VoC_lw32z(REG45,DEFAULT);
		VoC_bnltz32_r(LOWPASS_LIMIT_POSTIVE_VALUE100,ACC0);
			VoC_sub32_rr(ACC0,REG45,ACC0,DEFAULT);
			VoC_lw16i(REG5,1);
LOWPASS_LIMIT_POSTIVE_VALUE100:  
		VoC_lw16i(REG7,0x7fff);
		VoC_shru32_ri(REG67,16,DEFAULT);
		VoC_bgt32_rr(LOWPASS_LIMIT_VALUE100,ACC0,REG67);
			VoC_movreg32(REG67,ACC0,DEFAULT);
LOWPASS_LIMIT_VALUE100:
		VoC_bez16_r(LOWPASS_LIMIT_POSTIVE_VALUE110,REG5);
			VoC_lw16i(REG5,0);
			VoC_sub16_rr(REG6,REG5,REG6,DEFAULT);

LOWPASS_LIMIT_POSTIVE_VALUE110:	
		VoC_mult16_rd(REG6,REG6,CVSD_LOWPASS_CONST_FILTER_GAIN_Coef_TAB);
		VoC_lw16i(REG4,21);
		VoC_NOP();
		VoC_sw16inc_p(REG6,REG1,DEFAULT);
	    VoC_add16_rr(REG2,REG2,REG4,IN_PARALLEL);
		
	    VoC_shr32_ri(ACC1,12,DEFAULT);
		VoC_lw32z(REG45,DEFAULT);
		VoC_bnltz32_r(LOWPASS_LIMIT_POSTIVE_VALUE101,ACC1);
			VoC_sub32_rr(ACC1,REG45,ACC1,DEFAULT);
			VoC_lw16i(REG5,1);
LOWPASS_LIMIT_POSTIVE_VALUE101:  
			VoC_lw16i(REG7,0x7fff);
		VoC_shru32_ri(REG67,16,DEFAULT);
		VoC_bgt32_rr(LOWPASS_LIMIT_VALUE101,ACC1,REG67);
			VoC_movreg32(REG67,ACC1,DEFAULT);
LOWPASS_LIMIT_VALUE101:
		VoC_bez16_r(LOWPASS_LIMIT_POSTIVE_VALUE111,REG5);
			VoC_lw16i(REG5,0);
			VoC_sub16_rr(REG6,REG5,REG6,DEFAULT);

LOWPASS_LIMIT_POSTIVE_VALUE111:	
		VoC_mult16_rd(REG6,REG6,CVSD_LOWPASS_CONST_FILTER_GAIN_Coef_TAB);
		VoC_lw16i(REG4,40);
		VoC_NOP();
		VoC_sw16inc_p(REG6,REG1,DEFAULT);
	VoC_sub16_rr(REG3,REG3,REG4,IN_PARALLEL);

	VoC_endloop0

	VoC_pop16(RA,DEFAULT);
    VoC_NOP();
	VoC_return;
		
}


void CVSD_LowPass_Filter2(void)
{ 
	VoC_push16(RA,DEFAULT);
	VoC_lw16i_short(FORMATX,0,DEFAULT);
	VoC_lw16i_short(FORMAT32,0,IN_PARALLEL);

VoC_loop_i(0,4)
		VoC_lw16inc_p(REG4,REG0,DEFAULT);//input data;
		VoC_lw16inc_p(REG5,REG0,DEFAULT);//input data;
		VoC_lw32_d(ACC0,CVSD_LOWPASS_FILTER_ROUND);
		VoC_lw32_d(ACC1,CVSD_LOWPASS_FILTER_ROUND);
		VoC_bez16_d(CVSD_DEC_LOWPASS_filter2,CVSD_LOWPASS_DEC_ENC_FLAG);
		VoC_lw16i(REG6,1);
		VoC_sub16_rr(REG0,REG0,REG6,DEFAULT);
		VoC_movreg16(REG5,REG4,IN_PARALLEL);
	
		//x[n]
CVSD_DEC_LOWPASS_filter2:
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER2_Coef_TAB);//x(n)*a0;
		VoC_lw32_p(REG67,REG3,DEFAULT);//x(n-1);
		VoC_NOP();
	    VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n)*b0;

		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-1)=x(n);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER2_Coef_TAB+1);;//x(n-1)*a1;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-1)=x(n);
		VoC_NOP();
	    VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-1)*b1;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-2)=x(n-1);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER2_Coef_TAB+2);//x(n-2)*a2;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-2)=x(n-1);
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-2)*b2;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-3)=x(n-2);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER2_Coef_TAB+3);//x(n-3)*a3;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-3)=x(n-2);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-3)*b3;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-4)=x(n-3);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER2_Coef_TAB+4);//x(n-4)*a4;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-4)=x(n-3);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-4)*b4;

		VoC_lw32_p(REG67,REG3,DEFAULT);
		VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-5)=x(n-4);
		VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER2_Coef_TAB+5);//x(n-5)*a5;
		VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-5)=x(n-4);
		VoC_NOP();
		VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-5)*b5;

		VoC_lw32_p(REG45,REG3,DEFAULT);
		VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-6)=x(n-5);
		VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER2_Coef_TAB+6);//x(n-6)*a6;
		VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-6)=x(n-5);	
		VoC_NOP();
		VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-6)*b6;

        VoC_shr32_ri(ACC0,12,DEFAULT);
		VoC_lw32z(REG45,DEFAULT);
		VoC_bnltz32_r(LOWPASS_LIMIT_POSTIVE_VALUE200,ACC0);
			VoC_sub32_rr(ACC0,REG45,ACC0,DEFAULT);
			VoC_lw16i(REG5,1);
LOWPASS_LIMIT_POSTIVE_VALUE200:  
		VoC_lw16i(REG7,0x7fff);
		VoC_shru32_ri(REG67,16,DEFAULT);
		VoC_bgt32_rr(LOWPASS_LIMIT_VALUE200,ACC0,REG67);
			VoC_movreg32(REG67,ACC0,DEFAULT);
LOWPASS_LIMIT_VALUE200:
		VoC_bez16_r(LOWPASS_LIMIT_POSTIVE_VALUE210,REG5);
			VoC_lw16i(REG5,0);
			VoC_sub16_rr(REG6,REG5,REG6,DEFAULT);
LOWPASS_LIMIT_POSTIVE_VALUE210:	
			VoC_mult16_rd(REG6,REG6,CVSD_LOWPASS_CONST_FILTER_GAIN_Coef_TAB+1);
			VoC_lw16i(REG4,7);
			VoC_NOP();
			VoC_sw16inc_p(REG6,REG1,DEFAULT);
            VoC_add16_rr(REG2,REG2,REG4,IN_PARALLEL);

        VoC_shr32_ri(ACC1,12,DEFAULT);
		VoC_lw32z(REG45,DEFAULT);
		VoC_bnltz32_r(LOWPASS_LIMIT_POSTIVE_VALUE201,ACC1);
			VoC_sub32_rr(ACC1,REG45,ACC1,DEFAULT);
			VoC_lw16i(REG5,1);
LOWPASS_LIMIT_POSTIVE_VALUE201:  
		VoC_lw16i(REG7,0x7fff);
		VoC_shru32_ri(REG67,16,DEFAULT);
		VoC_bgt32_rr(LOWPASS_LIMIT_VALUE201,ACC1,REG67);
			VoC_movreg32(REG67,ACC1,DEFAULT);
LOWPASS_LIMIT_VALUE201:
		VoC_bez16_r(LOWPASS_LIMIT_POSTIVE_VALUE211,REG5);
			VoC_lw16i(REG5,0);
			VoC_sub16_rr(REG6,REG5,REG6,DEFAULT);

LOWPASS_LIMIT_POSTIVE_VALUE211:		
		VoC_mult16_rd(REG6,REG6,CVSD_LOWPASS_CONST_FILTER_GAIN_Coef_TAB+1);
		VoC_lw16i(REG4,12);
		VoC_NOP();
	    VoC_sw16inc_p(REG6,REG1,DEFAULT);
        VoC_sub16_rr(REG3,REG3,REG4,IN_PARALLEL);
VoC_endloop0				
	
		VoC_pop16(RA,DEFAULT);
	    VoC_NOP();
		VoC_return;
		
}


void CVSD_LowPass_Filter3(void)
{ 
	VoC_push16(RA,DEFAULT);
	VoC_lw16i_short(FORMATX,0,DEFAULT);
	VoC_lw16i_short(FORMAT32,0,IN_PARALLEL);
	
	VoC_loop_i(0,8)
		    VoC_lw16inc_p(REG4,REG0,DEFAULT);//input data;
			VoC_lw16inc_p(REG5,REG0,DEFAULT);//input data;
			VoC_lw32_d(ACC0,CVSD_LOWPASS_FILTER_ROUND);
			VoC_lw32_d(ACC1,CVSD_LOWPASS_FILTER_ROUND);
			VoC_bez16_d(CVSD_DEC_LOWPASS_filter3,CVSD_LOWPASS_DEC_ENC_FLAG);
			VoC_lw16i(REG6,1);
			VoC_sub16_rr(REG0,REG0,REG6,DEFAULT);
			VoC_movreg16(REG5,REG4,IN_PARALLEL);
		
			//x[n]
CVSD_DEC_LOWPASS_filter3:
			VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER3_Coef_TAB);//x(n)*a0;
			VoC_lw32_p(REG67,REG3,DEFAULT);//x(n-1);
			VoC_NOP();
		    VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n)*b0;

			VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-1)=x(n);
			VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER3_Coef_TAB+1);;//x(n-1)*a1;
			VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-1)=x(n);
			VoC_NOP();
		    VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-1)*b1;

			VoC_lw32_p(REG45,REG3,DEFAULT);
			VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-2)=x(n-1);
			VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER3_Coef_TAB+2);//x(n-2)*a2;
			VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-2)=x(n-1);
			VoC_NOP();
			VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-2)*b2;

			VoC_lw32_p(REG67,REG3,DEFAULT);
			VoC_sw16inc_p(REG4,REG3,DEFAULT);//x(n-3)=x(n-2);
			VoC_mac32_rd(REG6,CVSD_LOWPASS_CONST_FILTER3_Coef_TAB+3);//x(n-3)*a3;
			VoC_sw16inc_p(REG5,REG3,DEFAULT);//x(n-3)=x(n-2);
			VoC_NOP();
			VoC_macXinc_rp(REG7,REG2,IN_PARALLEL);//x(n-3)*b3;

			VoC_lw32_p(REG45,REG3,DEFAULT);
			VoC_sw16inc_p(REG6,REG3,DEFAULT);//x(n-4)=x(n-3);
			VoC_mac32_rd(REG4,CVSD_LOWPASS_CONST_FILTER3_Coef_TAB+4);//x(n-4)*a4;
			VoC_sw16inc_p(REG7,REG3,DEFAULT);//x(n-4)=x(n-3);	
			VoC_NOP();
			VoC_macXinc_rp(REG5,REG2,IN_PARALLEL);//x(n-4)*b4;

	        VoC_shr32_ri(ACC0,12,DEFAULT);
			VoC_lw32z(REG45,DEFAULT);
			VoC_bnltz32_r(LOWPASS_LIMIT_POSTIVE_VALUE300,ACC0);
				VoC_sub32_rr(ACC0,REG45,ACC0,DEFAULT);
				VoC_lw16i(REG5,1);
LOWPASS_LIMIT_POSTIVE_VALUE300:  
			VoC_lw16i(REG7,0x7fff);
			VoC_shru32_ri(REG67,16,DEFAULT);
			VoC_bgt32_rr(LOWPASS_LIMIT_VALUE300,ACC0,REG67);
				VoC_movreg32(REG67,ACC0,DEFAULT);
LOWPASS_LIMIT_VALUE300:
			VoC_bez16_r(LOWPASS_LIMIT_POSTIVE_VALUE310,REG5);
				VoC_lw16i(REG5,0);
				VoC_sub16_rr(REG6,REG5,REG6,DEFAULT);
LOWPASS_LIMIT_POSTIVE_VALUE310:	
			VoC_mult16_rd(REG6,REG6,CVSD_LOWPASS_CONST_FILTER_GAIN_Coef_TAB+2);
			VoC_lw16i(REG4,5);
			VoC_NOP();
			VoC_sw16inc_p(REG6,REG1,DEFAULT);
            VoC_add16_rr(REG2,REG2,REG4,IN_PARALLEL);

        	VoC_shr32_ri(ACC1,12,DEFAULT);
			VoC_lw32z(REG45,DEFAULT);
			VoC_bnltz32_r(LOWPASS_LIMIT_POSTIVE_VALUE301,ACC1);
				VoC_sub32_rr(ACC1,REG45,ACC1,DEFAULT);
				VoC_lw16i(REG5,1);
LOWPASS_LIMIT_POSTIVE_VALUE301:  
			VoC_lw16i(REG7,0x7fff);
			VoC_shru32_ri(REG67,16,DEFAULT);
			VoC_bgt32_rr(LOWPASS_LIMIT_VALUE301,ACC1,REG67);
				VoC_movreg32(REG67,ACC1,DEFAULT);
LOWPASS_LIMIT_VALUE301:
			VoC_bez16_r(LOWPASS_LIMIT_POSTIVE_VALUE311,REG5);
				VoC_lw16i(REG5,0);
				VoC_sub16_rr(REG6,REG5,REG6,DEFAULT);

LOWPASS_LIMIT_POSTIVE_VALUE311:		
			VoC_mult16_rd(REG6,REG6,CVSD_LOWPASS_CONST_FILTER_GAIN_Coef_TAB+2);
			VoC_lw16i(REG4,8);
			VoC_NOP();
		    VoC_sw16inc_p(REG6,REG1,DEFAULT);
	        VoC_sub16_rr(REG3,REG3,REG4,IN_PARALLEL);
	VoC_endloop0				
	
		VoC_pop16(RA,DEFAULT);
	    VoC_NOP();
		VoC_return;
		
}

void CVSD_encode(void)
{
	VoC_push16(RA,DEFAULT);

	VoC_lw16i_short(REG2,0,DEFAULT);//for REG7
	VoC_lw16i_short(REG3,1,IN_PARALLEL);//for REG7
	VoC_lw16_d(REG7,CVSD_ENC_JUDGE_alpha_buf);
	VoC_loop_i(0,16)
	    VoC_lw16i(REG4,0);//for extend
		VoC_lw16inc_p(REG5,REG0,DEFAULT); //x[0]
		VoC_shru16_ri(REG7,-1,DEFAULT);
		VoC_shru16_ri(REG2,1,IN_PARALLEL);

		VoC_shr32_ri(REG45,6,DEFAULT);
		VoC_lw16i(REG6,1);
		VoC_sub32_rd(REG45,REG45,CVSD_enc_xest_l);//x(k)-`x(k-1);
		VoC_bnltz32_r(csdv_b0_e_1,REG45);
			VoC_or16_ri(REG7,1);
			VoC_or16_ri(REG2,0x8000);
			VoC_lw16i(REG6,0);
	csdv_b0_e_1:
		VoC_and16_rd(REG7,CVSD_LOWPASS_CONST_0x000f);
		VoC_lw32_d(REG45,CVSD_enc_delta);
		VoC_sw16_d(REG7,CVSD_ENC_JUDGE_alpha_buf);
		VoC_bez16_r(cvsd_alpha_eq_1,REG7);
		VoC_be16_rd(cvsd_alpha_eq_1,REG7,CVSD_LOWPASS_CONST_0x000f);

		VoC_shr32_ri(REG45,10,DEFAULT);
		VoC_sub32_dr(REG45,CVSD_enc_delta,REG45);
		VoC_lw32_d(ACC0,CVSD_delta_min);
		VoC_sw32_d(REG45,CVSD_enc_delta);
		VoC_bgt32_rr(cvsd_alpha_judge_end,REG45,ACC0);
		VoC_sw32_d(ACC0,CVSD_enc_delta);
		VoC_jump(cvsd_alpha_judge_end);
cvsd_alpha_eq_1:
	
	    VoC_add32_rd(REG45,REG45,CVSD_delta_min);
		VoC_lw32_d(ACC0,CVSD_delta_max);
		VoC_sw32_d(REG45,CVSD_enc_delta);
		VoC_bgt32_rr(cvsd_alpha_judge_end,ACC0,REG45);
			VoC_sw32_d(ACC0,CVSD_enc_delta);
			
cvsd_alpha_judge_end:

	VoC_bnez16_d(cvsd_not_first_frame_1,CVSD_enc_first_frame_flag);
		VoC_lw32z(REG45,DEFAULT);
		VoC_sw16_d(REG3,CVSD_enc_first_frame_flag);
		VoC_sw32_d(REG45,CVSD_enc_xest_l);
		VoC_jump(cvsd_xest_judge_end);
cvsd_not_first_frame_1:

		VoC_lw32_d(REG45,CVSD_enc_xest_l);//`x(k-1);
		VoC_lw32_d(ACC0,CVSD_enc_delta);//deta;
	    VoC_bez16_r(csdv_encode_b0_e_0,REG6);
        VoC_add32_rr(REG45,REG45,ACC0,DEFAULT);//add deta;
        VoC_jump(csdv_encode_b0_e_end);
csdv_encode_b0_e_0:
        VoC_sub32_rr(REG45,REG45,ACC0,DEFAULT);//add deta;
csdv_encode_b0_e_end:

        VoC_bgt32_rd(csdv_encode_y_e_min,REG45,CVSD_y_min);
        VoC_lw32_d(REG45,CVSD_y_min);
csdv_encode_y_e_min:
        VoC_blt32_rd(csdv_encode_y_e_max,REG45,CVSD_y_max);
        VoC_lw32_d(REG45,CVSD_y_max);
csdv_encode_y_e_max:
        VoC_movreg32(ACC0,REG45,DEFAULT);
        VoC_shr32_ri(ACC0,5,DEFAULT);
		VoC_sub32_rr(REG45,REG45,ACC0,DEFAULT);
        VoC_NOP();
		VoC_sw32_d(REG45,CVSD_enc_xest_l);
cvsd_xest_judge_end:
		VoC_NOP();
	VoC_endloop0
		
	VoC_sw16inc_p(REG2,REG1,DEFAULT);

	VoC_pop16(RA,DEFAULT);
	VoC_NOP();
	VoC_return;
}

void CVSD_decode(void)
{
	VoC_push16(RA,DEFAULT);
	VoC_lw16inc_p(REG2,REG0,DEFAULT);//DEC_data;
	VoC_lw16_d(REG7,CVSD_DEC_JUDGE_alpha_buf);
	VoC_loop_i(0,16)
        VoC_movreg16(REG6,REG2,DEFAULT);
		VoC_and16_ri(REG6,1);
		VoC_shru16_ri(REG2,1,DEFAULT);
		VoC_shru16_ri(REG7,-1,IN_PARALLEL);
	    VoC_lw16i(REG4,0);//for extend
        
        VoC_or16_rr(REG7,REG6,DEFAULT);
		VoC_and16_rd(REG7,CVSD_LOWPASS_CONST_0x000f);
		VoC_lw32_d(REG45,CVSD_dec_delta);
		VoC_sw16_d(REG7,CVSD_DEC_JUDGE_alpha_buf);
		VoC_bez16_r(cvsd_dec_alpha_eq_1,REG7);
		VoC_be16_rd(cvsd_dec_alpha_eq_1,REG7,CVSD_LOWPASS_CONST_0x000f);

		VoC_shr32_ri(REG45,10,DEFAULT);
		VoC_sub32_dr(REG45,CVSD_dec_delta,REG45);
		VoC_lw32_d(ACC0,CVSD_delta_min);
		VoC_sw32_d(REG45,CVSD_dec_delta);
		VoC_bgt32_rr(cvsd_dec_alpha_judge_end,REG45,ACC0);
		VoC_sw32_d(ACC0,CVSD_dec_delta);
		VoC_jump(cvsd_dec_alpha_judge_end);
cvsd_dec_alpha_eq_1:
	
	    VoC_add32_rd(REG45,REG45,CVSD_delta_min);
		VoC_lw32_d(ACC0,CVSD_delta_max);
		VoC_sw32_d(REG45,CVSD_dec_delta);
		VoC_bgt32_rr(cvsd_dec_alpha_judge_end,ACC0,REG45);
			VoC_sw32_d(ACC0,CVSD_dec_delta);
			
cvsd_dec_alpha_judge_end:

		VoC_lw32_d(REG45,CVSD_dec_xest_l);//`x(k-1);
		VoC_lw32_d(ACC0,CVSD_dec_delta);//deta;
	    VoC_bnez16_r(csdv_decode_b0_e_0,REG6);
        VoC_add32_rr(REG45,REG45,ACC0,DEFAULT);//add deta;
        VoC_jump(csdv_decode_b0_e_end);
csdv_decode_b0_e_0:
        VoC_sub32_rr(REG45,REG45,ACC0,DEFAULT);//add deta;
csdv_decode_b0_e_end:
        VoC_bgt32_rd(csdv_decode_y_e_min,REG45,CVSD_y_min);
        VoC_lw32_d(REG45,CVSD_y_min);
csdv_decode_y_e_min:
        VoC_blt32_rd(csdv_decode_y_e_max,REG45,CVSD_y_max);
        VoC_lw32_d(REG45,CVSD_y_max);
csdv_decode_y_e_max:
        VoC_movreg32(ACC0,REG45,DEFAULT);
        VoC_shr32_ri(ACC0,5,DEFAULT);
		VoC_sub32_rr(REG45,REG45,ACC0,DEFAULT);
        VoC_NOP();
		VoC_sw32_d(REG45,CVSD_dec_xest_l);
		VoC_bgtz32_r(csdv_decode_x_g_round,REG45);
		VoC_sub32_rd(REG45,REG45,CVSD_DEC_OUTPUT_ROUND);
		VoC_jump(csdv_decode_x_g_round_end);
csdv_decode_x_g_round:
		VoC_add32_rd(REG45,REG45,CVSD_DEC_OUTPUT_ROUND);
csdv_decode_x_g_round_end:
		VoC_shr32_ri(REG45,10,DEFAULT);
cvsd_dec_xest_judge_end:
	    VoC_NOP();
	    VoC_sw16inc_p(REG4,REG1,DEFAULT);

	VoC_endloop0

	VoC_pop16(RA,DEFAULT);
	VoC_NOP();
	VoC_return;
}

