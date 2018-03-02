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
#include "vppp_speech_common.h"


// VoC_directive: PARSER_OFF
#include <stdio.h>
extern FILE *TestFileHandle;

// VoC_directive: PARSER_ON




#if 0


// voc_enum NonLinearTransform

//	voc_alias NonLinearTransform_Spk_Beta_order				  0x0000, x
  //  voc_alias NonLinearTransform_Spk_T_HI				      0x0000, x
//	voc_alias NonLinearTransform_Spk_T_LO				      0xc000, x
//	voc_alias NonLinearTransform_Spk_Scale					  0x3333, x  
	//voc_alias NonLinearTransform_Spk_CosPhaseBitNum		      0x0006, x
// voc_enum_end




     voc_struct  VPP_SPEECH_DEC_IN_STRUCT __attribute__((export)),x
	 voc_short INPUT_BITS2_ADDR[26],x
     voc_struct_end

     voc_struct  VPP_SPEECH_ENC_OUT_STRUCT __attribute__((export)),x
     voc_short OUTPUT_BITS2_ADDR,22,x
     voc_struct_end

     voc_struct VPP_SPEECH_ENC_IN_STRUCT __attribute__((export)),x
     voc_short INPUT_SPEECH_BUFFER2_ADDR,160,x
     voc_short INPUT_SPEECH_BUFFER1_ADDR,160,x
     voc_struct_end

     voc_struct   VPP_SPEECH_DEC_OUT_STRUCT   __attribute__((export)),x
     voc_short OUTPUT_SPEECH_BUFFER2_ADDR,160,x
     voc_short OUTPUT_SPEECH_BUFFER1_ADDR,160,x
	 voc_struct_end

#endif

#if 0
	voc_short GLOBAL_SDF_SAMPLS_ADDR,64,x







#endif

	#if 0

     voc_struct VPP_SPEECH_INIT_STRUCT          __attribute__((export)) , y
     voc_short GLOBAL_LOADED_VOCODER_ADDR			   ,y
     voc_short GLOBAL_LOADED_AMR_EFR_ADDR,				 y
     voc_short GLOBAL_LOADED_AMR_ENC_MODE_ADDR,          y
     voc_short GLOBAL_LOADED_AMR_DEC_MODE_ADDR,          y
     voc_short GLOBAL_VOCODER_SOURCE_CODE_ADDR_ADDR,6,   y
     voc_short GLOBAL_AMR_EFR_SOURCE_CODE_ADDR_ADDR,4,		  y
     voc_short GLOBAL_AMR_ENC_MODE_SOURCE_CODE_ADDR_ADDR,16 ,y
     voc_short GLOBAL_AMR_DEC_MODE_SOURCE_CODE_ADDR_ADDR,16, y
     voc_short GLOBAL_CONSTANTS_ADDR_ADDR,8, y

     voc_short GLOBAL_RESET_ADDR							,y
     voc_short GLOBAL_IRQGEN_ADDR							,y
     voc_struct_end



     voc_struct VPP_SPEECH_AUDIO_CFG_STRUCT  __attribute__((export)),y


     voc_short GLOBAL_ECHO_ES_ON,y
     voc_short GLOBAL_ECHO_ES_VAD,y
	 voc_short GLOBAL_ECHO_ES_DTD,y

     voc_short GLOBAL_ECHO_EC_REL,y
	 voc_short GLOBAL_ECHO_EC_MU,y
     voc_short GLOBAL_ECHO_EC_MIN,y

	 voc_short GLOBAL_ENC_MICGAIN_ADDR,y
     voc_short GLOBAL_DEC_SPKGAIN_ADDR,y
     voc_short GLOBAL_SDF_ADDR,2,y
     voc_short GLOBAL_MDF_ADDR,2,y
     voc_short GLOBAL_IF1_flag_ADDR     , 1 ,    y
     voc_struct_end


     voc_short GLOBAL_DTX_ADDR              ,y

     voc_short GLOBAL_RELOAD_MODE_ADDR,                  y
     voc_short GLOBAL_AMR_EFR_RESET_ADDR,                  y

     voc_short GLOBAL_OLD_MICGAIN_ADDR,y
     voc_short GLOBAL_OLD_SPKGAIN_ADDR,y
     voc_short GLOBAL_SDF_COEFFS_ADDR, 64,               y

     voc_short GLOBAL_BFI_ADDR,                          y
     voc_short GLOBAL_UFI_ADDR,                          y

     voc_short GLOBAL_SID_ADDR,                          y
     voc_short GLOBAL_TAF_ADDR,                          y
     voc_short GLOBAL_DEC_MODE_ADDR,                     y
 
     voc_short GLOBAL_ENC_MODE_ADDR,                     y
     voc_short GLOBAL_ENC_USED_MODE_ADDR,                y

     voc_short GLOBAL_EFR_FLAG_ADDR,                    y
     voc_short DEC_RESET_FLAG_ADDR,                      y
     voc_short DEC_MAIN_RESET_FLAG_OLD_ADDR,             y

#endif

#if 0
	voc_short GLOBAL_MDF_SAMPLS_ADDR,64,x

#endif



#if 0

 

 voc_struct AMR_EFR_RAMX_CONSTANTS_ADDR     ,x



	voc_short  CONST_2_ADDR          ,x
	voc_short  CONST_3_ADDR          ,x
	voc_short  CONST_4_ADDR          ,x
	voc_short  CONST_6_ADDR          ,x
	voc_short  CONST_7_ADDR          ,x
	voc_short  CONST_8_ADDR          ,x
	voc_short  CONST_9_ADDR          ,x
	voc_short  CONST_10_ADDR         ,x
	voc_short  CONST_11_ADDR         ,x
	voc_short  CONST_12_ADDR         ,x
	voc_short  CONST_13_ADDR         ,x
	voc_short  CONST_15_ADDR         ,x
	voc_short  CONST_16_ADDR         ,x
	voc_short  CONST_20_ADDR         ,x
	voc_short  CONST_30_ADDR         ,x
	voc_short  CONST_32_ADDR         ,x
	voc_short  CONST_40_ADDR         ,x
	voc_short  CONST_41_ADDR         ,x
	voc_short  CONST_64_ADDR         ,x
	voc_short  CONST_80_ADDR         ,x
	voc_short  CONST_120_ADDR        ,x
	voc_short  CONST_143_ADDR        ,x
	voc_short  CONST_160_ADDR        ,x
	voc_short  CONST_197_ADDR        ,x
	voc_short  CONST_5325_ADDR       ,x
	voc_short  CONST_0x1fff_ADDR     ,x
	voc_short  CONST_5_ADDR          ,x
	voc_short  CONST_15565_ADDR      ,x
	voc_short  CONST_0x1555_ADDR     ,x
	voc_short  CONST_0x2000_ADDR     ,x
	voc_short  CONST_neg2_ADDR       ,x
	voc_short  CONST_0x199a_ADDR     ,x
	voc_short  CONST_0x5a82_ADDR     ,x
	voc_short  CONST_0x00ff_ADDR     ,x


	voc_short  CONST_6390_ADDR        ,x                  		    	
	voc_short  CONST_21955_ADDR       ,x                   		    	
	voc_short  CONST_3641_ADDR        ,x                  		    	
	voc_short  CONST_0xf508_ADDR      ,x                    		
	voc_short  CONST_1260_ADDR        ,x                  		    	
	voc_short  CONST_720_ADDR         ,x                 		        
	voc_short  CONST_0x3fff_ADDR      ,x                    		
	voc_short  CONST_0x7f80_ADDR      ,x                    		
	voc_short  CONST_0x7c00_ADDR      ,x                    		
	voc_short  CONST_1000_ADDR        ,x                  		        
	voc_short  CONST_32750_ADDR       ,x                   		        
	voc_short  CONST_17578_ADDR       ,x                   		        
	voc_short  CONST_8521_ADDR        ,x                  		        
	voc_short  CONST_0xa80_ADDR       ,x                   		        
	voc_short  CONST_5443_ADDR        ,x                  		        
	voc_short  CONST_1310_ADDR        ,x                  		        

	voc_short CONST_28160_ADDR        ,x  
	voc_short CONST_1843_ADDR         ,x              		      
	voc_short CONST_3427_ADDR         ,x              		       
	voc_short CONST_6242_ADDR         ,x              		       
	voc_short CONST_31821_ADDR        ,x 
	voc_short CONST_3276_ADDR         ,x 
	voc_short CONST_368_ADDR		  ,x	
	voc_short CONST_21298_ADDR        ,x
	voc_short CONST_24660_ADDR        ,x
	voc_short CONST_5462_ADDR         ,x
	voc_short CONST_13106_ADDR        ,x
	voc_short CONST_1500_ADDR        ,x
 // 	voc_short  ReservedX16_ADDR     ,4,x

	voc_short  CONST_0_ADDR              ,2,x
	voc_short  CONST_1_ADDR              ,2,x
	voc_short  CONST_0x40000000_ADDR     ,2,x
	voc_short  CONST_0x7FFFFFFF_ADDR     ,2,x
	voc_short  CONST_0x80000000_ADDR     ,2,x
	voc_short  CONST_0x8000FFFF_ADDR     ,2,x
	voc_short  CONST_D_1_4_ADDR          ,2,x
	voc_short  CONST_0x00010001_ADDR     ,2,x
	voc_short  CONST_0x00004000L_ADDR    ,2,x
	voc_short  CONST_0x00007FFF_ADDR     ,2,x
	voc_short  CONST_0x00008000_ADDR     ,2,x
	voc_short  CONST_0x0000FFFF_ADDR     ,2,x


	voc_short CONST_0x70816958_ADDR    ,2,x                          
	voc_short CONST_0x69cb0014_ADDR    ,2,x                          
	voc_short CONST_15000_ADDR         ,2,x                          
	voc_short CONST_0x53c00_ADDR       ,2,x                          
	voc_short CONST_0x00040000L_ADDR   ,2,x               		
                                         

	voc_short CONST_D_0xc800_ADDR     ,2,x              		
	voc_short CONST_D_0xf6b3_ADDR     ,2,x             			
	voc_short CONST_D_150_ADDR        ,2,x            		    
	voc_short CONST_13849L_ADDR      ,2,x 
	voc_short CONST_0x10000000_ADDR    ,2,x 


	voc_short CONST_0x40000028_ADDR    ,2,x                       
	voc_short CONST_0x55550001_ADDR    ,2,x                        
	voc_short CONST_0x7fff0007_ADDR    ,2,x                        
    voc_short CONST_D_3500_ADDR       ,2,x 

	voc_short CONST_0x04000000_ADDR  ,2,x 
	voc_short CONST_0x80017fff_ADDR  ,2,x 
	voc_short  CONST_0x2000e000_ADDR ,2,x 
	voc_short	CONST_783741L_ADDR   ,2,x 
	voc_short CONST_0x100000_ADDR   ,2,x

	voc_short CONST_72_40_ADDR		,2,x                            
	voc_short CONST_40_36_ADDR		,2,x
                           
	voc_short CONST_D_1_8_ADDR      ,2,x                             
	voc_short CONST_D_1_16_ADDR     ,2,x                   	
	voc_short CONST_D_1_32_ADDR     ,2,x                  	
	voc_short CONST_D_1_64_ADDR     ,2,x                   	

	voc_short CONST_1_8_0x4000_ADDR  ,2,x                        
	voc_short CONST_1_16_1_4_ADDR    ,2,x                   		
	voc_short CONST_1_32_1_4_ADDR    ,2,x                   		
	voc_short CONST_1_64_1_8_ADDR    ,2,x                   		

    voc_short  CONST_0x00000100_ADDR ,2,x 
	voc_short  CONST_40_64_ADDR      ,2,x 

	voc_short CONST_0x7fff1fff_ADDR     ,2,x                          
	voc_short CONST_0x8000e000_ADDR     ,2,x                          

	voc_short CONST_ALPHA_DOWN2_ALPHA_UP2_ADDR   ,2,x  

	voc_short CONST_600_32000_ADDR     ,2,x  
	voc_short CONST_800_30500_ADDR     ,2,x 
	voc_short CONST_0xc3da1e13_ADDR   ,2,x



/*
    voc_short TABLE_DTX_psrCNNewFactor_ADDR        ,   12   ,x
	voc_short TABLE_DTX_ppLr_gsTable_ADDR          ,   256  ,x
	voc_short TABLE_ERR_psrR0RepeatThreshold_ADDR  ,   32   ,x
	voc_short TABLE_ERR_psrConceal_ADDR            ,   16   ,x
	voc_short TABLE_DE_A_SST_psrSST_ADDR           ,   12   ,x
	voc_short TABLE_ERR_psrLevelMaxIncrease_ADDR   ,   16   ,x
	voc_short TABLE_HOMING_n_ADDR                  ,   18   ,x
	voc_short TABLE_HOMING_dhf_mask_ADDR           ,   18   ,x
	voc_short TABLE_DE_SP_psrSPFDenomWidenCf_ADDR  ,   10   ,x
	voc_short TABLE_SP_FRM_pswPCoefE_ADDR          ,   4    ,x
	voc_short TABLE_SP_FRM_psrSubMultFactor_ADDR   ,   6    ,x
	voc_short TABLE_SP_ROM_pppsrGsp0_ADDR          ,   640  ,x
	voc_short TABLE_SP_ROM_pppsrUvCodeVec_ADDR     ,   560  ,x
	voc_short TABLE_SP_ROM_pppsrVcdCodeVec_ADDR    ,   360  ,x
	voc_short TABLE_SP_ROM_psrR0DecTbl_ADDR           	,64    ,x
	voc_short TABLE_SP_ROM_psrHPFCoefs_ADDR           	,10    ,x
	voc_short TABLE_SP_ROM_psrNWCoefs_ADDR            	,20    ,x
	voc_short TABLE_SP_ROM_pL_rFlatSstCoefs_ADDR      	,20    ,x
	voc_short TABLE_SP_ROM_psrOldCont_ADDR            	,4     ,x
	voc_short TABLE_SP_ROM_psrNewCont_ADDR            	,4     ,x
	voc_short TABLE_SFRM_pswUpdateIndexV_pswBitIndexV_ADDR	,256   ,x
	voc_short TABLE_SFRM_pswUpdateIndexUn_pswBitIndexUn_ADDR,64    ,x
	voc_short TABLE_SFRM_pswModNextBitV_ADDR          	,10    ,x
	voc_short TABLE_SFRM_pswModNextBitUV_ADDR         	,8     ,x
	voc_short TABLE_HOMING_n1_ADDR                    	,18    ,x

*/
 //  voc_struct_end

//	voc_short ReservedX_ADDR,  (628+818+118-6),x


     voc_short TABLE_SQRT_L_ADDR                    ,50   ,x
     voc_short STATIC_CONST_PRMNO_ADDR		        ,10   ,x
     voc_short STATIC_CONST_CDOWN_ADDR	            ,8    ,x
     voc_short STATIC_CONST_PDOWN_ADDR	            ,8    ,x

     voc_short TABLE_POW2_ADDR			           ,34    ,x
     voc_short TABLE_INV_SQRT_ADDR		           ,50    ,x
     voc_short TABLE_LSP_LSF_ADDR		           ,66    ,x
     voc_short  TABLE_SLOPE_ADDR		           ,64    ,x

     voc_short STATIC_CONST_dtx_log_en_adjust_ADDR      ,10  ,x
     voc_short STATIC_CONST_lsf_hist_mean_scale_ADDR	,10  ,x
     voc_short STATIC_CONST_DGRAY_ADDR	                ,8   ,x
     voc_short STATIC_CONST_GRAY_ADDR	                ,8   ,x
     voc_short TABLE_LOG2_ADDR			                ,34  ,x
     voc_short STATIC_CONST_qua_gain_code_ADDR          ,96  ,x
     voc_short STATIC_CONST_QUA_GAIN_PITCH_ADDR         ,16  ,x



     voc_short TABLE_GRID_ADDR			               ,62 ,x
	 voc_short STATIC_CONST_PRMNOFSF_ADDR	            ,8   ,x
	 voc_short STATIC_CONST_BITNO_ADDR	             ,30      ,x

   voc_short  STATIC_CONST_bitno_MR475_ADDR	       ,9    ,x
   voc_short  STATIC_CONST_bitno_MR515_ADDR	       ,10   ,x
   voc_short  STATIC_CONST_bitno_MR59_ADDR	       ,10   ,x
   voc_short  STATIC_CONST_bitno_MR67_ADDR	       ,10   ,x
   voc_short  STATIC_CONST_bitno_MR74_ADDR	      ,10    ,x
   voc_short  STATIC_CONST_bitno_MR795_ADDR	      ,12    ,x
   voc_short  STATIC_CONST_bitno_MR102_ADDR	      ,20    ,x
   voc_short  STATIC_CONST_bitno_MRDTX_ADDR	       ,3    ,x

    voc_short STATIC_CONST_ph_imp_mid_ADDR             ,40    ,x
    voc_short STATIC_CONST_ph_imp_low_MR795_ADDR       ,40    ,x
    voc_short STATIC_trackTable_ADDR	               ,20    ,x


    voc_short STATIC_CONST_table_gain_highrates_ADDR   ,512   ,x

    voc_short STATIC_CONST_table_gain_MR475_ADDR	   ,1024  ,x

	voc_short STATIC_mr515_3_lsf_compressed_ADDR	    ,416   ,x
    voc_short TABLE_DICO1_LSF_3_compressed_ADDR	                ,624 ,x
	
    voc_short TABLE_DICO3_LSF_3_compressed_ADDR	                ,1664 ,x
    voc_short TABLE_DICO1_LSF_compressed_ADDR	                   ,416   ,x


	voc_short ReservedX4_ADDR,    (2+720+8-32-32-38-480-4-15*6-16-4),x

	//voc_short GLOBAL_NON_CLIP_TEMP_BUFFER[FRAMESIZE*2],x

	voc_short GLOBAL_MIC_DIGITAL_OLD_GAIN_ADDR,x
	voc_short GLOBAL_RECEIVER_DIGITAL_OLD_GAIN_ADDR,x

	voc_short GLOBAL_AEC_DIGITAL_OLD_GAIN_ADDR,x
	voc_short GLOBAL_DIGITAL_OLD_GAIN_ADDR,x


	voc_short COS_TABLE_ADDR[129],x	
	voc_short TAN_TABLE_ADDR[33+(FRAMESIZE*2-129-33)],x

	voc_alias GLOBAL_NON_CLIP_TEMP_BUFFER COS_TABLE_ADDR,x
	
	voc_short GLOBAL_NON_CLIP_MIC_HISTORYDATA[FRAMESIZE],x

	voc_short GLOBAL_NON_CLIP_CROSSZEROCOUNTER,x
	voc_short GLOBAL_NON_CLIP_CROSSZEPEAK,x

	voc_short GLOBAL_DIGITAL_GAIN_ADDR,x
	voc_short GLOBAL_DIGITAL_MAXVALUE_ADDR,x

	voc_short GLOBAL_NOTCH_FILTER_XN12_YN12_ADDR[15*6],x

	voc_short GLOBAL_NOTCH_FILTER_COEF_ADDR __attribute__((export)),x
	voc_short GLOBAL_NOTCH_FILTER_COEF_ADDR0[15],x
	

    voc_struct_end




 #endif






#if 0
     voc_short GLOBAL_MDF_COEFFS_ADDR,64,         y

// ECHO
     voc_short GLOBAL_ECHO_SUPPRESS_EXP_COUNTER,1,      y
     voc_short GLOBAL_ECHO_CANCEL_EXP_MU,1,      y

     voc_short GLOBAL_ECHO_CANCEL_DEC_PWR,2,      y


	voc_short GLOBAL_HIGH_PASS_FILTER_XN_1_ADDR,y
	voc_short GLOBAL_HIGH_PASS_FILTER_XN_2_ADDR,y
	voc_short GLOBAL_HIGH_PASS_FILTER_YN_1_ADDR,y
	voc_short GLOBAL_HIGH_PASS_FILTER_YN_2_ADDR,y

	voc_short GLOBAL_COMFORTALBE_NOISE_ADDR,160,y

	voc_short GLOBAL_ECHO_ON_INLOUDSPEAKER_FLAG_ADDR,y
	voc_short GLOBAL_VAD_COUNTER,y

	voc_short GLOBAL_HIGH_PASS_FILTER2_XN_1_ADDR,y
	voc_short GLOBAL_HIGH_PASS_FILTER2_XN_2_ADDR,y
	voc_short GLOBAL_HIGH_PASS_FILTER2_YN_1_ADDR,y
	voc_short GLOBAL_HIGH_PASS_FILTER2_YN_2_ADDR,y

	voc_short GLOBAL_MIC_DIGITAL_GAIN_ADDR __attribute__((export)),y
	voc_short GLOBAL_MIC_DIGITAL_MAXVALUE_ADDR __attribute__((export)),y
	voc_short GLOBAL_RECEIVER_DIGITAL_GAIN_ADDR __attribute__((export)),y
	voc_short GLOBAL_RECEIVER_DIGITAL_MAXVALUE_ADDR __attribute__((export)),y
		
	voc_word GLOBAL_OUTPUTENERGY_ADDR __attribute__((export)),y
	voc_word GLOBAL_INPUTENERGY_ADDR __attribute__((export)),y
	voc_short GLOBAL_COMFORTALBE_NOISE_BAK_ADDR,160,y
	

	voc_word GLOBAL_SPEECH_RAMX_BACKUP_ADDR __attribute__((export)),y	
	voc_word GLOBAL_SPEECH_CODE_BACKUP_ADDR __attribute__((export)),y
	
	voc_word GLOBAL_AEC_RAMX_BACKUP_ADDR __attribute__((export)),y
	voc_word GLOBAL_AEC_CODE_ADDR __attribute__((export)),y

	voc_word GLOBAL_AEC_CONSTX_ADDR __attribute__((export)),y	

	voc_word GLOBAL_RX_ADDR __attribute__((export)),y
	voc_word GLOBAL_TX_ADDR __attribute__((export)),y


	voc_word UpLimit_Spk_ADDR,y
	voc_word DnLimit_Spk_ADDR,y

	voc_word Minus_UpLimit_Spk_ADDR,y
	voc_word Minus_DnLimit_Spk_ADDR,y


voc_struct m_AECProcPara_ADDR __attribute__((export)),y

	voc_short AEC_ENABLE_FLAG_ADDR 
	voc_short AGC_ENABLE_FLAG_ADDR 

	voc_short m_AECProcPara_ProcessMode
	voc_short m_AECProcPara_InitRegularFactor
	voc_short m_AECProcPara_AFUpdateFactor
	voc_short m_AECProcPara_AFCtrlSpeedUpFactor;
	voc_short m_AECProcPara_AFFilterAmp;
	
	voc_short m_AECProcPara_EchoEstMethod
	voc_short m_AECProcPara_NoiseUpdateFactor
	voc_short m_AECProcPara_SNRUpdateFactor
	voc_short m_AECProcPara_SNRLimit
	voc_short m_AECProcPara_NoiseOverEstFactor
	voc_short m_AECProcPara_NoiseGainLimit
	voc_short m_AECProcPara_EchoUpdateFactor
	voc_short m_AECProcPara_SERUpdateFactor
	voc_short m_AECProcPara_SERLimit
	voc_short m_AECProcPara_EchoOverEstFactor
	voc_short m_AECProcPara_EchoGainLimit
	voc_short m_AECProcPara_CNGFactor
	voc_short m_AECProcPara_AmpGain
	voc_word m_AECProcPara_NoiseMin
	
	voc_word m_AECProcPara_PFPrevEchoEstLimit;

	voc_short m_AECProcPara_PFDTEchoOverEstFactor;

	voc_short m_AECProcPara_pPFEchoGainLimitAmp[9];



	voc_short m_AECProcPara_NoiseGainLimitStep
	voc_short m_AECProcPara_AmpThr_ADDR

	voc_short m_AECProcPara_PFCLDataSmoothFactor
	voc_short m_AECProcPara_PFCLNoiseUpdateFactor
	voc_short m_AECProcPara_PFCLThrSigDet
	voc_short m_AECProcPara_PFCLThrDT

	voc_short m_AECProcPara_DataSmoothFactor;
	voc_short m_AECProcPara_PFCLChanlGain;

	voc_short m_AECProcPara_PFCLThrNT;
	voc_short m_AECProcPara_PFCLThrDTDiff;
	voc_short m_AECProcPara_PFCLDTHoldTime;
	voc_short m_AECProcPara_PFCLDTStartTime;

	
	voc_short m_AECProcPara_PFCLDTDUpdateFactor;
	voc_short m_AECProcPara_PFCLDTDThreshold;
	voc_short m_AECProcPara_PFCLDTD2ndThreshold;

	voc_short m_AECProcPara_StrongEchoFlag;

	voc_short m_AECProcPara__PFCLDTDThrRatio;
//	voc_short m_AECProcPara__reserve;
	



voc_struct_end

	voc_short GLOBAL_SPEECH_RECORD_FLAG __attribute__((export)),y


	voc_short GLABAL_AEC_RESET_ADDR,y
	voc_short GLABAL_MORPH_RESET_ADDR,y

	voc_short MORPHVOICE_ENABLE_FLAG_ADDR __attribute__((export)),y
	voc_short PITCH_SHIFT_ADDR __attribute__((export)),y
	

	voc_word GLOBAL_BLUETOOTH_RX_ADDR __attribute__((export)),y
	voc_alias GLOBAL_MORPH_CODE_ADDR ,y
	voc_word GLOBAL_BLUETOOTH_TX_ADDR __attribute__((export)),y
	voc_alias GLOBAL_MORPH_CONSTY_ADDR ,y
	voc_word GLOBAL_SPEECH_RAMY_BACKUP_ADDR __attribute__((export)),y	

	voc_short ingSumPhase_ADDR[129],y
	voc_short gRover_ADDR,y




//	voc_short GLOBAL_RESERVE_ADDR[381-5-160-2-10-4-8-4-18-4-4-5-1-4-6-130-2-4-6+10-4-2-4-2-1-2]				,y

    // voc_short GLOBAL_ECHO_CANCEL_SAMPLS_ADDR,296,y
    // voc_short GLOBAL_ECHO_CANCEL_COEFFS_ADDR,256,y



	 voc_short GLOBAL_VOCODER_RESET_ADDR,      y

     voc_short GLOBAL_OUTPUT_Vad_ADDR,            y
     voc_short GLOBAL_OUTPUT_SP_ADDR,             y
     voc_short DEC_AMR_FRAME_TYPE_ADDR,           y

     voc_short ENC_INPUT_ADDR_ADDR          ,y
	 voc_short DEC_OUTPUT_ADDR_ADDR         ,y
     voc_short ENC_INPUT_ADDR_BAK_ADDR      ,y
     voc_short DEC_OUTPUT_ADDR_BAK_ADDR     ,y



	#endif
/*

        #if 0

      voc_struct HR_RAMY_CONSTANTS_ADDR     ,y

	voc_short TABLE_SP_ROM_psrQuant1_ADDR             ,3072    ,    y
	voc_short TABLE_SP_ROM_psrQuant2_ADDR             ,768     ,    y
	voc_short TABLE_SP_ROM_psrQuant3_ADDR             ,512     ,    y
	voc_short TABLE_SP_ROM_psrPreQ1_ADDR              ,96      ,    y
	voc_short TABLE_SP_ROM_psrPreQ2_ADDR              ,48      ,    y
	voc_short TABLE_SP_ROM_psrPreQ3_ADDR              ,32      ,    y
	voc_short TABLE_SP_ROM_psrQuantSz_ADDR            ,4       ,    y
	voc_short TABLE_SP_ROM_psrPreQSz_ADDR             ,4       ,    y
	voc_short TABLE_SP_ROM_psrSQuant_ADDR             ,256     ,    y
	voc_short TABLE_SP_ROM_psvqIndex_ADDR             ,12      ,    y
	voc_short TABLE_SP_ROM_ppsrSqrtP0_ADDR            ,96      ,    y
	voc_short TABLE_SP_ROM_ppsrPVecIntFilt_ADDR       ,60      ,    y
	voc_short TABLE_SP_ROM_psrLagTbl_ADDR             ,256     ,    y
	voc_short TABLE_SP_ROM_ppsrCGIntFilt_ADDR         ,36      ,    y
    voc_short hr_reset_value_ADDR                    ,    10   ,   y

        voc_struct_end
        voc_short Reserved2_ADDR,           6144-5730-10+484-5,    y
    #endif
  */




      #if 0

    voc_struct AMR_EFR_RAMY_CONSTANTS_ADDR     ,y

     voc_short STATIC_CONST_F_GAMMA1_ADDR                 ,10      ,    y
     voc_short STATIC_CONST_F_GAMMA2_ADDR                 ,10      ,    y
     voc_short STATIC_CONST_F_GAMMA3_ADDR	          ,10      ,    y
     voc_short STATIC_CONST_F_GAMMA4_ADDR	          ,10      ,    y
     voc_short STATIC_CONST_gamma3_ADDR                   ,10      ,    y
     voc_short STATIC_CONST_gamma1_ADDR                   ,10      ,    y
     voc_short STATIC_CONST_WIND_200_40_compressd_ADDR              ,122     ,    y

     voc_short STATIC_CONST_INTER_6_ADDR	           ,62      ,   y
     voc_short STATIC_CONST_INTER_6_25_ADDR               ,26      ,    y

     voc_short TABLE_LAG_L_ADDR			         ,10  	   ,    y
     voc_short TABLE_LAG_H_ADDR			         ,10       ,    y
     voc_short STATIC_CONST_pred_ADDR                     ,4       ,    y
     voc_short STATIC_pred_fac_ADDR	                  ,10      ,    y
     voc_short STATIC_past_rq_init_ADDR                   ,80      ,    y
     voc_short STATIC_MEAN_LSF_3_ADDR	                  ,10      ,    y

	 voc_short STATIC_CONST_lsp_init_data_ADDR          ,10    ,y
     voc_short TABLE_MEAN_LSF_ADDR		              ,10  	      ,y


    voc_short  TABLE_WINDOW_160_80_compressd_ADDR	              ,122       ,y

    voc_short  TABLE_WINDOW_232_8_compressd_ADDR		          ,126       ,y

    voc_short STATIC_CONST_pred_MR122_ADDR	          ,4          ,y

    voc_short STATIC_CONST_INTERP_FACTOR_ADDR           ,24  	  ,y
    voc_short STATIC_CONST_SID_CODEWORD_BIT_IDX_ADDR     ,96      ,y
	voc_short STATIC_CONST_DHF_MASK_ADDR                 ,58      ,y
    voc_short STATIC_CONST_dhf_MR122_ADDR              ,58   ,y


     voc_short STATIC_CONST_dhf_MR475_ADDR              ,18  ,y
     voc_short STATIC_CONST_dhf_MR515_ADDR              ,20  ,y
     voc_short STATIC_CONST_dhf_MR59_ADDR	          ,20    ,y
     voc_short STATIC_CONST_dhf_MR67_ADDR	          ,20    ,y
     voc_short STATIC_CONST_dhf_MR74_ADDR	          ,20    ,y
     voc_short STATIC_CONST_dhf_MR795_ADDR              ,24  ,y
     voc_short STATIC_CONST_dhf_MR102_ADDR              ,40  ,y

     voc_short  STATIC_CONST_corrweight_ADDR            ,128   ,y
    voc_short  STATIC_CONST_startPos_ADDR	          ,16     ,y
 	voc_short STATIC_CONST_ph_imp_low_ADDR             ,40    ,y

    voc_short STATIC_CONST_table_gain_lowrates_ADDR	    ,256   ,y
    voc_short TABLE_DICO2_LSF_3_compressed_ADDR	        ,1248    ,y
    voc_short TABLE_DICO2_LSF_compressed_ADDR	        ,832    	  ,y
    voc_short TABLE_DICO3_LSF_compressed_ADDR	        ,832   	  ,y
    voc_short TABLE_DICO4_LSF_compressed_ADDR	        ,832    	  ,y
    voc_short TABLE_DICO5_LSF_compressed_ADDR	        ,208        ,y
	voc_short STATIC_mr795_1_lsf_compressed_ADDR	    ,1248   ,y

	voc_short TABLE_MR475_ADDR                          ,96      ,y
	voc_short TABLE_MR515_ADDR                          ,104     ,y
	voc_short TABLE_MR59_ADDR                           ,118     ,y
	voc_short TABLE_MR67_ADDR                           ,134     ,y
	voc_short TABLE_MR74_ADDR                           ,148     ,y
	voc_short TABLE_MR795_ADDR                          ,160     ,y
	voc_short TABLE_MR102_ADDR                          ,204     ,y
	voc_short TABLE_MR122_ADDR                          ,244     ,y

    voc_short Frame_header_ADDR                          ,8     ,y
    voc_short Bits_no_ADDR                              ,8     ,y

    voc_short Reserved2_ADDR,           (300-8-8-32-160-14),    y

	//voc_short GLOBAL_OUTPUT_BACKUP_ADDR,160,y


	voc_short GLOBAL_NON_CLIP_SPK_HISTORYDATA[FRAMESIZE],y	

	
	voc_short HIGH_PASS_FILTER_ENABLE_FLAG_ADDR __attribute__((export)),y
	voc_short NOTCH_FILTER_ENABLE_FLAG_ADDR __attribute__((export)),y

	voc_short NOISE_SUPPRESSER_ENABLE_FLAG_ADDR __attribute__((export)),y
	voc_short NOISE_SUPPRESSER_WITHOUT_SPEECH_ENABLE_FLAG_ADDR __attribute__((export)),y

	voc_word GLOBAL_AGC_acc_error_l,y

	voc_word GLOBAL_AGC_MAXVAL                          ,y
	voc_word GLOBAL_AGC_Coeff_l							,y

	voc_short GLOBAL_MIC_ENERGY_COUNTER_ADDR,y
	voc_short GLOBAL_NOISE_DIGITAL_GAIN_ADDR,y

	voc_short GLOBAL_AEC_SPK_DIGITAL_GAIN_ADDR __attribute__((export)),y
	voc_short GLOBAL_AEC_SPK_DIGITAL_MAXVALUE_ADDR __attribute__((export)),y

	
	
	voc_struct_end

	voc_short MIC_FILT_ADDR,x


    #endif


#if 0

  voc_alias CONST_0x7FFF_ADDR                                 	CONST_0x00007FFF_ADDR   ,x
  voc_alias CONST_0xFFFF_ADDR                                 	CONST_0x0000FFFF_ADDR   ,x
  voc_alias CONST_0x8000_ADDR                                 	CONST_0x00008000_ADDR   ,x
  voc_alias CONST_0x4000_ADDR                                 	CONST_0x00004000L_ADDR  ,x
  voc_alias CONST_0x5999_ADDR                                 	(6+  STATIC_CONST_CDOWN_ADDR),x
  voc_alias CONST_0x4CCC_ADDR                                 	(148+  STATIC_CONST_table_gain_lowrates_ADDR),y

#endif

void CII_BT_VOCODER_OPEN(void)
{
	// Clear all pending status
    VoC_cfg_lg(CFG_WAKEUP_ALL,CFG_WAKEUP_STATUS);
	VoC_cfg(CFG_CTRL_IRQ_TO_XCPU|CFG_CTRL_CONTINUE);
	VoC_NOP();
}

void CII_BT_VOCODER(void)
{


CII_VOCODER_EXIT:

	// Stall the execution
	VoC_cfg(CFG_CTRL_STALL);

	// VoC_directive: PARSER_OFF
	VoC_cfg_stop;
	// VoC_directive: PARSER_ON

	VoC_NOP();
	VoC_NOP();

	// Here the execution is woken up
  	VoC_lw16i(SP16,SP16_ADDR);
    VoC_lw16i(SP32,SP32_ADDR);

	// The wakeup mask is set on ifc0/sof0 or on ifc1/sof1 only.
	// so that we collect the 0/1 events together.
	// For this reason we get the wakeup status
	// (and no the cause = maksed status) in REG7
	VoC_lw16_d(REG7,CFG_WAKEUP_STATUS);
	VoC_lw16i_short(REG6,16,DEFAULT);

	// clear the wakeup status
	VoC_sw16_d(REG7,CFG_WAKEUP_STATUS);

	// if the events are SW, shift by 4
	VoC_bgt16_rr(is_hw_event,REG6,REG7);
		VoC_shr16_ri(REG7,4,DEFAULT);
is_hw_event:

	VoC_bez16_r(CII_VOCODER_EXIT,REG7)

	VoC_lw32_d(ACC0,GLOBAL_ENC_MICGAIN_ADDR);
	VoC_lw16i_short(REG2,-1,DEFAULT);

	VoC_lw16_d(REG3,GLOBAL_RESET_ADDR);
	//if (reset_flag != 0)
	VoC_bez16_r(CII_MAIN_VOCODER,REG3)

		VoC_lw16i_short(REG0,0,DEFAULT);
		VoC_lw16i_short(BITCACHE,0,DEFAULT);
		VoC_sw16_d(REG0,CFG_DMA_WRAP);
		VoC_lw16i(STATUS,STATUS_CLR);	
		
		// ramp mode (00 or 11)
        // mic gain mode (00 or 11)
		VoC_sw32_d(ACC0,GLOBAL_OLD_MICGAIN_ADDR);

		VoC_sw16_d(REG2,GLOBAL_RELOAD_MODE_ADDR);
		VoC_sw16_d(REG2,GLOBAL_VOCODER_RESET_ADDR);
		VoC_sw16_d(REG2,GLOBAL_AMR_EFR_RESET_ADDR);

        VoC_lw16i(REG2, INPUT_SPEECH_BUFFER2_ADDR);
        VoC_lw16i(REG3, OUTPUT_SPEECH_BUFFER2_ADDR);

        VoC_lw16i(REG4, INPUT_SPEECH_BUFFER1_ADDR);
        VoC_lw16i(REG5, OUTPUT_SPEECH_BUFFER1_ADDR);

        VoC_sw32_d(REG23,ENC_INPUT_ADDR_ADDR);
        VoC_sw32_d(REG45,ENC_INPUT_ADDR_BAK_ADDR);

        // clear GLOBAL_SDF_SAMPLS_ADDR and GLOBAL_MDF_SAMPLS_ADDR buffers
        VoC_lw16i_set_inc(REG2, GLOBAL_SDF_SAMPLS_ADDR,2);
        VoC_lw32z(ACC0,DEFAULT);
        VoC_loop_i(0,64)
            VoC_sw32inc_p(ACC0,REG2,DEFAULT);
        VoC_endloop0

        VoC_sw32_d(ACC0,CONST_0_ADDR);
        VoC_lw16i(ACC0_HI,0x4000);
		VoC_lw16i_short(REG6,15,DEFAULT);
		VoC_sw32_d(ACC0,CONST_0x40000000_ADDR);
		VoC_sw16_d(REG6,CONST_15_ADDR);

		VoC_lw32z(ACC0,DEFAULT);

		VoC_sw32_d(ACC0,GLOBAL_HIGH_PASS_FILTER_XN_1_ADDR);
		//VoC_sw16_d(ACC0_HI,GLOBAL_HIGH_PASS_FILTER_XN_2_ADDR);
		VoC_sw32_d(ACC0,GLOBAL_HIGH_PASS_FILTER_YN_1_ADDR);
		//VoC_sw16_d(ACC0_HI,GLOBAL_HIGH_PASS_FILTER_YN_2_ADDR);

		VoC_sw32_d(ACC0,GLOBAL_HIGH_PASS_FILTER2_XN_1_ADDR);
		//VoC_sw16_d(ACC0_HI,GLOBAL_HIGH_PASS_FILTER2_XN_2_ADDR);
		VoC_sw32_d(ACC0,GLOBAL_HIGH_PASS_FILTER2_YN_1_ADDR);
		//VoC_sw16_d(ACC0_HI,GLOBAL_HIGH_PASS_FILTER2_YN_2_ADDR);

		VoC_sw16_d(ACC0_HI,GLOBAL_ECHO_ON_INLOUDSPEAKER_FLAG_ADDR);

		VoC_sw16_d(ACC0_HI,GLOBAL_MIC_ENERGY_COUNTER_ADDR);
		VoC_sw16_d(ACC0_HI,GLOBAL_VAD_COUNTER);


		VoC_sw32_d(ACC0,GLOBAL_AGC_acc_error_l);

		VoC_lw16i(REG4,0xffff);
		VoC_lw16i_short(REG5,1,DEFAULT);
		VoC_lw16i(REG2,30000);
		VoC_lw16i_short(REG3,0,DEFAULT);

		VoC_sw32_d(REG45,GLOBAL_AGC_Coeff_l);
		VoC_sw32_d(REG23,GLOBAL_AGC_MAXVAL);

			
		VoC_lw16i_short(WRAP0,0,DEFAULT);
		VoC_lw16i_short(WRAP1,0,IN_PARALLEL);

		VoC_lw16i_short(WRAP2,0,DEFAULT);
		VoC_lw16i_short(WRAP3,0,IN_PARALLEL);

		VoC_lw16i_set_inc(REG3,GLOBAL_COMFORTALBE_NOISE_ADDR,2);
		VoC_lw16i_set_inc(REG2,GLOBAL_NON_CLIP_SPK_HISTORYDATA,2);
		VoC_lw16i_set_inc(REG1,GLOBAL_NON_CLIP_MIC_HISTORYDATA,2);
		VoC_lw16d_set_inc(REG0,DEC_OUTPUT_ADDR_BAK_ADDR,2);
		
		VoC_lw32z(ACC0,DEFAULT);

		VoC_loop_i(0,80)
            VoC_sw32inc_p(ACC0,REG3,DEFAULT);
			VoC_sw32inc_p(ACC0,REG2,DEFAULT);
			VoC_sw32inc_p(ACC0,REG1,DEFAULT);
			VoC_sw32inc_p(ACC0,REG0,DEFAULT);
        VoC_endloop0


		VoC_sw32_d(ACC0,GLOBAL_MIC_DIGITAL_OLD_GAIN_ADDR);

		VoC_sw32_d(ACC0,GLOBAL_AEC_DIGITAL_OLD_GAIN_ADDR);
			

		VoC_lw16i_set_inc(REG2,GLOBAL_NOTCH_FILTER_XN12_YN12_ADDR,2);

		VoC_loop_i(0,45)
            VoC_sw32inc_p(ACC0,REG2,DEFAULT);
        VoC_endloop0

		VoC_lw16i_short(REG5,1,DEFAULT);
		VoC_sw16_d(ACC0_HI,GLOBAL_ECHO_SUPPRESS_EXP_COUNTER);
		VoC_sw16_d(REG5,GLABAL_AEC_RESET_ADDR);
		VoC_sw16_d(REG5,GLABAL_MORPH_RESET_ADDR);


CII_MAIN_VOCODER:

	// SAVE PARAMETERS
    VoC_movreg16(REG6,REG7,DEFAULT);//dma_status
	VoC_and16_ri(REG6,1);

	VoC_bez16_r(CII_DECODER_RUN,REG6)

		//store  Mode value, Not push to RAM_X to avoid stack crash in MR74 and MR795
		VoC_push32(REG67,DEFAULT);

        //load_mic_data
		VoC_lw16_d(REG2,ENC_INPUT_ADDR_ADDR);

		VoC_lw16i(REG0,INPUT_SPEECH_BUFFER2_ADDR);

		VoC_lw32_d(REG45,GLOBAL_TX_ADDR);

		VoC_be16_rr(CII_MAIN_VOCODER_L0,REG0,REG2)

			VoC_lw16i(ACC0_HI,320);
			VoC_shr32_ri(ACC0,16,DEFAULT);
			VoC_add32_rr(REG45,REG45,ACC0,DEFAULT);

	CII_MAIN_VOCODER_L0:			
			VoC_shr16_ri(REG2,1,DEFAULT);
			VoC_lw16i(REG3,80);
			VoC_jal(Coolsand_BT_AecRunDma);	

	//High pass filter
	VoC_bez16_d(Coolsand_BT_Encoder_HighPass,HIGH_PASS_FILTER_ENABLE_FLAG_ADDR)
		VoC_jal(Coolsand_BT_HighPassFilter);
Coolsand_BT_Encoder_HighPass:
	
		VoC_lw16_d(REG3,GLOBAL_MIC_DIGITAL_GAIN_ADDR);
		VoC_lw16_d(REG4,GLOBAL_MIC_DIGITAL_MAXVALUE_ADDR);
		VoC_lw16_d(REG5,GLOBAL_MIC_DIGITAL_GAIN_ADDR);
		
		VoC_lw16d_set_inc(REG0,ENC_INPUT_ADDR_ADDR,2);
		VoC_lw16i_set_inc(REG1,GLOBAL_NON_CLIP_TEMP_BUFFER,2);
		VoC_lw16i_set_inc(REG2,GLOBAL_NON_CLIP_MIC_HISTORYDATA,2);	
		VoC_sw16_d(REG3,GLOBAL_DIGITAL_GAIN_ADDR);
		VoC_sw16_d(REG4,GLOBAL_DIGITAL_MAXVALUE_ADDR);
		VoC_sw16_d(REG5,GLOBAL_NOISE_DIGITAL_GAIN_ADDR);

		VoC_lw16i(REG3,GLOBAL_MIC_DIGITAL_OLD_GAIN_ADDR);
		
		
		VoC_jal(Coolsand_BT_NonClipDigitalGain);

	//AEC
	VoC_bez16_d(Coolsand_BT_Encoder_Aec,AEC_ENABLE_FLAG_ADDR)
		VoC_jal(Coolsand_BT_Aec);
		VoC_jump(CII_ENCODER_L1);
Coolsand_BT_Encoder_Aec:

	VoC_bez16_d(Coolsand_BT_Encoder_NoiseSuppress,NOISE_SUPPRESSER_ENABLE_FLAG_ADDR)
		VoC_jal(Coolsand_BT_NoiseSuppresser);
Coolsand_BT_Encoder_NoiseSuppress:

		VoC_jal(CII_BT_ECHO_MAIN);

CII_ENCODER_L1:

	VoC_bez16_d(Coolsand_BT_Encoder_NotchFilter,NOTCH_FILTER_ENABLE_FLAG_ADDR)
		VoC_jal(Coolsand_BT_NotchFilter);
Coolsand_BT_Encoder_NotchFilter:



	//store aec_mic data
		VoC_lw16_d(REG2,ENC_INPUT_ADDR_ADDR);

		VoC_lw16i(REG0,INPUT_SPEECH_BUFFER2_ADDR);

		VoC_lw32_d(REG45,GLOBAL_BLUETOOTH_RX_ADDR);

		VoC_be16_rr(CII_MAIN_VOCODER_L2,REG0,REG2)
			VoC_lw16i(ACC0_HI,320);
			VoC_shr32_ri(ACC0,16,DEFAULT);
			VoC_add32_rr(REG45,REG45,ACC0,DEFAULT);
		CII_MAIN_VOCODER_L2:

		VoC_shr16_ri(REG2,1,DEFAULT);
		VoC_lw16i(REG3,80);
		VoC_jal(Coolsand_BT_AecRunDma);

CII_DECODER_RUN:

	// VoC_directive: PARSER_OFF
			VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,1);
			VoC_NOP();
			VoC_NOP();
			fwrite(&RAM_X[((REGS[3].reg==INPUT_SPEECH_BUFFER1_ADDR) ? INPUT_SPEECH_BUFFER1_ADDR : INPUT_SPEECH_BUFFER2_ADDR)/2],1,320,TestFileHandle);
			fflush(TestFileHandle);
	// VoC_directive: PARSER_ON
	
		//load_speaker_data  test_nonclip;
		VoC_lw16_d(REG2,DEC_OUTPUT_ADDR_ADDR);
	
		VoC_lw16i(REG0,OUTPUT_SPEECH_BUFFER2_ADDR);
	
		VoC_lw32_d(REG45,GLOBAL_BLUETOOTH_TX_ADDR);
	
		VoC_be16_rr(CII_MAIN_VOCODER_L4,REG0,REG2)
	
			VoC_lw16i(ACC0_HI,320);
			VoC_shr32_ri(ACC0,16,DEFAULT);
			VoC_add32_rr(REG45,REG45,ACC0,DEFAULT);
	
	CII_MAIN_VOCODER_L4:			
			VoC_shr16_ri(REG2,1,DEFAULT);
			VoC_lw16i(REG3,80);
			VoC_jal(Coolsand_BT_AecRunDma);

	VoC_lw16i_short(REG5,4,DEFAULT);
	VoC_lw16_d(REG3,GLOBAL_RECEIVER_DIGITAL_GAIN_ADDR);
	VoC_lw16_d(REG4,GLOBAL_RECEIVER_DIGITAL_MAXVALUE_ADDR);
	VoC_lw16d_set_inc(REG0,DEC_OUTPUT_ADDR_ADDR,2);
	VoC_lw16i_set_inc(REG1,GLOBAL_NON_CLIP_TEMP_BUFFER,2);
	VoC_lw16i_set_inc(REG2,GLOBAL_NON_CLIP_SPK_HISTORYDATA,2);		
	VoC_sw16_d(REG3,GLOBAL_DIGITAL_GAIN_ADDR);
	VoC_sw16_d(REG4,GLOBAL_DIGITAL_MAXVALUE_ADDR);
	VoC_sw16_d(REG5,GLOBAL_NOISE_DIGITAL_GAIN_ADDR);
	
	VoC_lw16i(REG3,GLOBAL_RECEIVER_DIGITAL_OLD_GAIN_ADDR);
	
	VoC_jal(Coolsand_BT_NonClipDigitalGain);


	VoC_lw16_d(REG2,DEC_OUTPUT_ADDR_ADDR);

	VoC_lw16i(REG0,OUTPUT_SPEECH_BUFFER2_ADDR);

	VoC_lw32_d(REG45,GLOBAL_RX_ADDR);

	VoC_be16_rr(CII_MAIN_VOCODER_L1,REG0,REG2)
		VoC_lw16i(ACC0_HI,320);
		VoC_shr32_ri(ACC0,16,DEFAULT);
		VoC_add32_rr(REG45,REG45,ACC0,DEFAULT);
CII_MAIN_VOCODER_L1:

	VoC_shr16_ri(REG2,1,DEFAULT);
    VoC_lw16i(REG3,80);
	VoC_jal(Coolsand_BT_AecRunDma);
		

	// generate an interrupt if the flag is enabled
    VoC_lw16i_short(REG7,2,DEFAULT);
	VoC_and16_rd(REG7,GLOBAL_IRQGEN_ADDR);
	VoC_bez16_r(CII_DECODER_no_irq,REG7)
	VoC_cfg(CFG_CTRL_IRQ_TO_XCPU|CFG_CTRL_CONTINUE);	
CII_DECODER_no_irq:

	VoC_lw16i_short(REG1,0xFFFF,DEFAULT);

	VoC_lw32z(ACC0,DEFAULT);

    VoC_be16_rd(CII_VOCODER_not_update_reset,REG1,(INPUT_BITS2_ADDR+1)) //mode = 0xFFFF :

		// clear the vocoder reset only if the mode is valid
		VoC_sw16_d(ACC0_LO,GLOBAL_VOCODER_RESET_ADDR);

CII_VOCODER_not_update_reset:
	
	// clear the general reset
	VoC_sw16_d(ACC0_LO,GLOBAL_RESET_ADDR);

	VoC_lw32_d(REG45,ENC_INPUT_ADDR_ADDR);
    VoC_lw32_d(REG23,ENC_INPUT_ADDR_BAK_ADDR);
    VoC_sw32_d(REG45,ENC_INPUT_ADDR_BAK_ADDR);
	VoC_sw32_d(REG23,ENC_INPUT_ADDR_ADDR);

	VoC_jump(CII_VOCODER_EXIT);

}




// ****************************
// func  : CII_BT_ECHO_MAIN
//
//
// output: REG6 <- echo_vad
//         REG7 <- func_id = 2
//         ACC0 <- enc_corr_pwr
//         ACC1 <- enc_pwr
//
// used  : all
// ****************************


#define echo_format32       6  // format for the pwr computation

void CII_BT_ECHO_MAIN(void)
{

	VoC_push16(RA,DEFAULT);
	VoC_lw16i_short(REG6,0,IN_PARALLEL);

	// at this point the counter = 0, 1 or 2
	VoC_bngtz16_d(no_mic_muting_begin,GLOBAL_ECHO_SUPPRESS_EXP_COUNTER)

			// echo vad state counter
		VoC_lw16d_set_inc(REG3,GLOBAL_ECHO_SUPPRESS_EXP_COUNTER,-1);
		VoC_lw16i_short(REG6,1,DEFAULT);
		// at this point the counter = 1 or 2
		// decrement counter (INC3 = -1)
		VoC_inc_p(REG3,DEFAULT);
		// echo vad state counter
		VoC_sw16_d(REG3,GLOBAL_ECHO_SUPPRESS_EXP_COUNTER);
no_mic_muting_begin:

    // no mute if es_on = 0, return
//	VoC_lw16_d(REG1,GLOBAL_ECHO_EC_MU);
//	VoC_bez16_r(CII_ECHO_MAIN_L3 ,REG1)

		VoC_lw16d_set_inc(REG0,DEC_OUTPUT_ADDR_ADDR,2);

		// compute en_in PWR
		VoC_lw32z(ACC0,DEFAULT);
		VoC_lw16i_short(FORMAT32,echo_format32,DEFAULT);

		VoC_loop_i(0,80)
			VoC_bimac32inc_pp(REG0,REG0);
		VoC_endloop0;

		
		VoC_lw16d_set_inc(REG0,ENC_INPUT_ADDR_ADDR,2);

		VoC_lw32z(ACC0,DEFAULT);
		VoC_lw16i_short(FORMAT32,echo_format32,IN_PARALLEL);

		VoC_sw32_d(ACC0,GLOBAL_OUTPUTENERGY_ADDR);

		VoC_loop_i(0,80)
			VoC_bimac32inc_pp(REG0,REG0);
		VoC_endloop0;

		VoC_NOP();

		VoC_movreg16(REG1,ACC0_HI,DEFAULT);
		VoC_lw16i_short(FORMAT32,-16,IN_PARALLEL);

		VoC_multf32_rd(ACC0,REG1,GLOBAL_ECHO_EC_REL);

		VoC_sw32_d(ACC0,GLOBAL_INPUTENERGY_ADDR);

		VoC_blt32_rd(CII_ECHO_MAIN_L1,ACC0,GLOBAL_OUTPUTENERGY_ADDR)

			VoC_lw16i_short(REG6,0,DEFAULT);

	CII_ECHO_MAIN_L1:

		VoC_lw16i(ACC0_LO,0x0000);
		VoC_lw16i(ACC0_HI,0x0030);

		VoC_lw32_d(ACC1,GLOBAL_INPUTENERGY_ADDR);

		VoC_bez16_d(CII_ECHO_MAIN_L2,NOISE_SUPPRESSER_WITHOUT_SPEECH_ENABLE_FLAG_ADDR)

		VoC_bgt32_rr(CII_ECHO_MAIN_L2,ACC1,ACC0)

			VoC_lw16i_short(REG1,1,DEFAULT);
			VoC_add16_rd(REG1,REG1,GLOBAL_MIC_ENERGY_COUNTER_ADDR);
			VoC_lw16i_short(REG2,10,DEFAULT);
			VoC_sw16_d(REG1,GLOBAL_MIC_ENERGY_COUNTER_ADDR);

			VoC_lw16d_set_inc(REG0,ENC_INPUT_ADDR_ADDR,2);

			VoC_bgt16_rr(CII_ECHO_MAIN_L3,REG2,REG1)

				VoC_lw16i_short(REG1,26,DEFAULT);
				VoC_lw16i_short(FORMAT16,4-16,IN_PARALLEL);
				
				VoC_sub16_rd(REG1,REG1,GLOBAL_MIC_ENERGY_COUNTER_ADDR);

				VoC_lw16i_short(REG5,4,DEFAULT);

				VoC_bgt16_rr(CII_ECHO_MAIN_L4,REG1,REG5);
					VoC_lw16i_short(REG1,4,DEFAULT);
CII_ECHO_MAIN_L4:

				VoC_loop_i(0,80)
					VoC_lw32_p(REG23,REG0,DEFAULT);

					VoC_multf16_rr(REG2,REG2,REG1,DEFAULT);
					VoC_multf16_rr(REG3,REG3,REG1,IN_PARALLEL);
					
					VoC_NOP();
					VoC_NOP();
					VoC_sw32inc_p(REG23,REG0,DEFAULT);
					
				VoC_endloop0;

				VoC_jump(CII_ECHO_MAIN_L3);

	CII_ECHO_MAIN_L2:

			VoC_sw16_d(ACC0_LO,GLOBAL_MIC_ENERGY_COUNTER_ADDR);

	CII_ECHO_MAIN_L3:


    // no mute if es_on = 0, return	
	VoC_bnez16_d(LOUDSPEAKER_no_mic_muting_end ,GLOBAL_ECHO_ES_ON)

	VoC_lw16_d(REG1,GLOBAL_ECHO_EC_MU);
	VoC_bez16_r(NORMAL_no_mic_muting_end ,REG1)

LOUDSPEAKER_no_mic_muting_end:

	VoC_bez16_r(NORMAL_no_mic_muting_lastframe,REG6)
	
		VoC_bnez16_d(NORMAL_no_mic_muting_nofirstframe,GLOBAL_ECHO_ON_INLOUDSPEAKER_FLAG_ADDR)
			VoC_lw16i_short(REG2,1,DEFAULT);
			VoC_lw16i_set_inc(REG2,GLOBAL_COMFORTALBE_NOISE_ADDR,1);
			VoC_sw16_d(REG2,GLOBAL_ECHO_ON_INLOUDSPEAKER_FLAG_ADDR);
			VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,1);
			VoC_lw16d_set_inc(REG1,ENC_INPUT_ADDR_ADDR,1);

			VoC_lw16i_short(FORMAT16,7-16,DEFAULT);
			VoC_lw16i_short(REG0,0,DEFAULT);
			VoC_lw16i_short(REG5,127,IN_PARALLEL);
			VoC_lw16i_short(INC0,1,DEFAULT);

			VoC_loop_i(0,128)
				VoC_multf16inc_rp(REG4,REG0,REG2,DEFAULT);
				VoC_sub16_rr(REG7,REG5,REG0,IN_PARALLEL);

				VoC_multf16inc_rp(REG6,REG7,REG3,DEFAULT);
				VoC_inc_p(REG0,DEFAULT);

				VoC_add16_rr(REG4,REG4,REG6,DEFAULT);
				VoC_NOP();
				VoC_sw16inc_p(REG4,REG1,DEFAULT);				
			VoC_endloop0;

			VoC_loop_i(0,32)
				VoC_multf16inc_rp(REG4,REG0,REG2,DEFAULT);
				VoC_sub16_rr(REG7,REG5,REG0,IN_PARALLEL);

				VoC_multf16inc_rp(REG6,REG7,REG3,DEFAULT);
				VoC_NOP();

				VoC_add16_rr(REG4,REG4,REG6,DEFAULT);
				VoC_NOP();
				VoC_sw16inc_p(REG4,REG1,DEFAULT);				
			VoC_endloop0;

		VoC_jump(NORMAL_no_mic_muting_end);

NORMAL_no_mic_muting_nofirstframe: 

		VoC_lw16i_short(REG2,1,DEFAULT);
		VoC_lw16i_set_inc(REG2,GLOBAL_COMFORTALBE_NOISE_ADDR,2);
		VoC_sw16_d(REG2,GLOBAL_ECHO_ON_INLOUDSPEAKER_FLAG_ADDR);
		VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,2);
		VoC_lw32inc_p(ACC0,REG2,DEFAULT);
		VoC_loop_i(0,80)
			VoC_lw32inc_p(ACC0,REG2,DEFAULT);
			VoC_sw32inc_p(ACC0,REG3,DEFAULT);				
		VoC_endloop0;
		VoC_jump(NORMAL_no_mic_muting_end);


NORMAL_no_mic_muting_lastframe:

	VoC_bez16_d(NORMAL_no_mic_muting_end,GLOBAL_ECHO_ON_INLOUDSPEAKER_FLAG_ADDR)
		VoC_lw16i_short(REG2,0,DEFAULT);
		VoC_lw16i_set_inc(REG2,GLOBAL_COMFORTALBE_NOISE_ADDR,1);
		VoC_sw16_d(REG2,GLOBAL_ECHO_ON_INLOUDSPEAKER_FLAG_ADDR);
		VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,1);
		VoC_lw16d_set_inc(REG1,ENC_INPUT_ADDR_ADDR,1);

		VoC_lw16i_short(FORMAT16,7-16,DEFAULT);
		VoC_lw16i_short(REG0,0,DEFAULT);
		VoC_lw16i_short(REG5,127,IN_PARALLEL);
		VoC_lw16i_short(INC0,1,DEFAULT);

		VoC_loop_i(0,128)
			VoC_multf16inc_rp(REG4,REG0,REG3,DEFAULT);
			VoC_sub16_rr(REG7,REG5,REG0,IN_PARALLEL);

			VoC_multf16inc_rp(REG6,REG7,REG2,DEFAULT);
			VoC_inc_p(REG0,DEFAULT);

			VoC_add16_rr(REG4,REG4,REG6,DEFAULT);
			VoC_NOP();
			VoC_sw16inc_p(REG4,REG1,DEFAULT);				
		VoC_endloop0;
NORMAL_no_mic_muting_end:

    VoC_pop16(RA,DEFAULT);
	// format32
    VoC_lw16i_short(FORMAT32,-1,DEFAULT);
	// fct_id = echo flag = 2
//	VoC_lw16i_short(REG7,2,DEFAULT);
	VoC_return;

}


// **********************************
// func: CII_BT_ECHO_SUPPRESSION
//
// input :

//
// output: ACC0_LO <- echo_vad_flag
//         ACC1    <- enc_pwr
//
// used  : REG0123457
//
// unused: REG6
// ***********************************


void CII_BT_ECHO_SUPPRESSION(void)
{

	// push RA
	VoC_push16(RA,DEFAULT);
	VoC_lw16i_short(FORMAT32,-1,IN_PARALLEL);

	// compute dec_out PWR
	VoC_lw32z(ACC0,DEFAULT);
	VoC_lw16i_short(INC0,2,IN_PARALLEL);

	VoC_loop_i(0,80)
		VoC_bimac32inc_pp(REG0,REG0);
	VoC_endloop0;

	/**************************/
	// input in ACC0
	// output in REG1  only used ACC0, REG1
	/**************************/

	VoC_lw16i_short(REG1,31,DEFAULT);
	VoC_bez32_r(dec_pwr_null,ACC0)

	// get 32 - log(dec_pwr) in REG1
	VoC_jal(CII_BT_NORM_L_ACC0);

dec_pwr_null:

	// echo vad state counter
	VoC_lw16d_set_inc(REG3,GLOBAL_ECHO_SUPPRESS_EXP_COUNTER,-1);

	// no mute if es_on = 0, return
	VoC_bez16_d(LOUDSPEAKER_MODE_no_echo_VAD_counter_reset,GLOBAL_ECHO_ES_ON)

	// no mute if 32 - log(dec_pwr) > echo_vad_thr  (no voice activity on decoder)
	VoC_bgt16_rd(LOUDSPEAKER_MODE_no_echo_VAD_counter_reset,REG1,GLOBAL_ECHO_ES_VAD)

		// reset counter
		VoC_lw16i_short(REG3,4,DEFAULT);
	
	VoC_jump(NORMAL_MODE_no_echo_VAD_counter_reset);

LOUDSPEAKER_MODE_no_echo_VAD_counter_reset:


	VoC_bez16_d(NORMAL_MODE_no_echo_VAD_counter_reset,GLOBAL_ECHO_EC_MU)

	// no mute if 32 - log(dec_pwr) > echo_vad_thr  (no voice activity on decoder)
	VoC_bgt16_rd(NORMAL_MODE_no_echo_VAD_counter_reset,REG1,GLOBAL_ECHO_EC_MIN)

		// reset counter
		VoC_lw16i_short(REG3,4,DEFAULT);

NORMAL_MODE_no_echo_VAD_counter_reset:

	// RA
	VoC_pop16(RA,DEFAULT);

	// echo vad state counter
	VoC_sw16_d(REG3,GLOBAL_ECHO_SUPPRESS_EXP_COUNTER);

	VoC_return;
}


/**************************/
// input in ACC0
// output in REG1  only used ACC0, REG1
/**************************/

void CII_BT_NORM_L_ACC0(void)
{

	VoC_lw16i_set_inc(REG1,0,1);
	VoC_push32(ACC1,DEFAULT);
	VoC_movreg32(ACC1,ACC0,IN_PARALLEL);
	VoC_bez32_r(G_PITCH_NORM_L_3_EXIT,ACC0)
	VoC_bnltz32_r(G_PITCH_NORM_L_3_1,ACC0)
		VoC_not32_r(ACC0,DEFAULT);
G_PITCH_NORM_L_3_1:
		VoC_loop_i_short(31,DEFAULT)
		VoC_startloop0
			VoC_bnlt32_rd(G_PITCH_NORM_L_3_EXIT,ACC0,CONST_0x40000000_ADDR)
			VoC_shr32_ri(ACC0,-1,DEFAULT);
			VoC_inc_p(REG1,IN_PARALLEL);
			VoC_shr32_ri(ACC1,-1,DEFAULT);
		VoC_endloop0
G_PITCH_NORM_L_3_EXIT:
	VoC_movreg32(ACC0,ACC1,DEFAULT);
	VoC_pop32(ACC1,DEFAULT);
	VoC_return
}



/*********************/
// input in REG0, REG1
// output in  REG2
// used register RL6, RL7
// REG0/REG1
/*********************/

void CII_BT_DIV_S(void)
{
	VoC_lw16i(REG2,0x7fff);
	VoC_be16_rr(SCHUR1_DIV_S_11,REG1,REG0)
	VoC_lw16i_set_inc(REG2,0,1);

	VoC_movreg16(RL6_LO, REG0, DEFAULT);
	VoC_movreg16(RL7_LO, REG1, IN_PARALLEL);
	VoC_lw16i_short(RL6_HI,0,DEFAULT);
	VoC_lw16i_short(RL7_HI,0, IN_PARALLEL);

	VoC_loop_i_short(14,DEFAULT)
	VoC_shr32_ri(RL6,-1,IN_PARALLEL);
	VoC_startloop0

		VoC_bgt32_rr(SCHUR1_DIV_S_1,RL7,RL6)
		VoC_sub32_rr(RL6,RL6,RL7,DEFAULT);
		VoC_inc_p(REG2,IN_PARALLEL);
	SCHUR1_DIV_S_1:
		VoC_shr16_ri(REG2,-1,DEFAULT);
		VoC_shr32_ri(RL6,-1,IN_PARALLEL);
		VoC_endloop0
	VoC_bgt32_rr(SCHUR1_DIV_S_11,RL7,RL6)
	VoC_inc_p(REG2,DEFAULT);
SCHUR1_DIV_S_11:

	VoC_return
}


// ****************
// input :  REG2 : reset
//          REG3 : coeffs_buffers, INC3 = 2
// output : none
// used : REG34, ACC0 1
// not modified : REG2
// ****************

void CII_BT_LoadSpFormatFilter(void)
{
    VoC_lw16i_short(FORMAT16,(15-16),DEFAULT);
    VoC_lw16i_short(FORMAT32,-1,DEFAULT);
	VoC_bez16_d(DONE_MSDF,GLOBAL_RESET_ADDR)
	// if pointer == NULL, apply ID filter
	VoC_bnez32_r(LOAD_MSDF,ACC1);

		VoC_lw32z(RL7,DEFAULT);
		VoC_lw32_d(ACC1,CONST_0x40000000_ADDR);

		VoC_loop_i_short((SECTION_MSDF_SIZE-1),DEFAULT)
		VoC_startloop0
		    VoC_sw32inc_p(RL7,REG3,DEFAULT);
		VoC_endloop0;

		VoC_sw32_p(ACC1,REG3,DEFAULT);

		VoC_jump(DONE_MSDF);

LOAD_MSDF:
		// configure addresses of burst
        VoC_shr16_ri(REG3,1,DEFAULT);
 		VoC_cfg_lg(SECTION_MSDF_SIZE,CFG_DMA_SIZE);
      	// store local address
		VoC_sw16_d(REG3,CFG_DMA_LADDR);  // local address
		// store extern address
		VoC_sw32_d(ACC1,CFG_DMA_EADDR); // read non-single access

		VoC_cfg(CFG_CTRL_STALL);

		VoC_NOP();
		// clear the event
		VoC_cfg_lg(CFG_WAKEUP_DMAI,CFG_WAKEUP_STATUS);


DONE_MSDF:
   	VoC_NOP();
	VoC_return;
}


void CII_BT_encoder_homing_frame_test(void)
{
	VoC_lw16i_short(ACC0_LO,0x0008,DEFAULT);
	VoC_lw16i_short(REG4,0,DEFAULT);
	VoC_movreg16(ACC0_HI,ACC0_LO,IN_PARALLEL);

	VoC_loop_i(0,80)
		VoC_lw32inc_p(REG67,REG0,DEFAULT);
	   	VoC_bne32_rr(CII_encoder_homing_frame_test_01,REG67,ACC0)
	VoC_endloop0

	VoC_lw16i_short(REG4,1,DEFAULT);

CII_encoder_homing_frame_test_01:

	VoC_return

}





// ****************
// input : REG0 : filter samples, INC0 = 2
//         REG3 : source buffer,  INC3 = 2
//         REG4 : nb_tap
// output : none
// used : REG02345, ACC01
// ****************


void CII_BT_shift_filter (void)
{

	VoC_push16(REG0,DEFAULT);
	VoC_lw16i(REG2,80);
//	VoC_sub16_dr(REG5,CONST_80_ADDR,REG4);
   VoC_sub16_rr(REG5,REG2,REG4,DEFAULT);
	// pointers
	VoC_lw16i_set_inc(REG2,MIC_FILT_ADDR,2);    // in RAM_X

	// copy old frame values in the temp buffer
	VoC_loop_r_short(REG4,DEFAULT)
	VoC_lw32inc_p(ACC0,REG0,IN_PARALLEL);
	VoC_startloop0
		VoC_lw32inc_p(ACC0,REG0,DEFAULT);
exit_on_warnings = OFF;
		VoC_sw32inc_p(ACC0,REG2,DEFAULT);
exit_on_warnings = ON;
	VoC_endloop0

	// copy new frame values in the temp buffer
	VoC_loop_r_short(REG5,DEFAULT)
	VoC_lw32inc_p(ACC0,REG3,IN_PARALLEL);
	VoC_startloop0
		VoC_lw32inc_p(ACC0,REG3,DEFAULT);
exit_on_warnings = OFF;
		VoC_sw32inc_p(ACC0,REG2,DEFAULT);
exit_on_warnings = ON;
	VoC_endloop0

	// continue copy new frame values in the temp buffer
	// save last values of the new frame
	VoC_pop16(REG0,DEFAULT);

	VoC_loop_r_short(REG4,DEFAULT);
	VoC_startloop0
		VoC_movreg32(ACC1,ACC0,DEFAULT);
		VoC_lw32inc_p(ACC0,REG3,DEFAULT);

exit_on_warnings = OFF;
		VoC_sw32inc_p(ACC0,REG2,DEFAULT);
exit_on_warnings = ON;
		VoC_sw32inc_p(ACC1,REG0,DEFAULT);
	VoC_endloop0

	VoC_return;

}




// ****************
// input : REG0 : filter samples, INC0 = 2
//         REG1 : out buffer,     INC1 = 2
//         REG2 : filter coeffs,  INC2 = 2
//         REG3 : source buffer,  INC3 = 2
// output : none
// used :
// ****************


void CII_BT_RunAntiDistorsionFilter (void)
{
	// ---------------------------------
	// copy and set the appropriate gain
	// gain 1.0 <=> 0x4000
	// ---------------------------------

	VoC_push16(RA,DEFAULT);
	VoC_push16(REG2,DEFAULT);
	VoC_push16(REG1,DEFAULT);


	// STACK 16         STACK32
	// ------------------------------------
	// RA               REG45
	// filt_coeffs
	// out_buffer

	VoC_lw16i_short(REG4,SECTION_MSDF_SIZE,IN_PARALLEL);

	// ****************
	// input : REG0 : filter samples, INC0 = 2
	//         REG3 : source buffer,  INC3 = 2
	//         REG4 : nb_tap
	// output : none
	// used : REG02345, ACC01
	// ****************

	VoC_jal(CII_BT_shift_filter);


	// ----------------------------------------
	// apply filter and store in the out buffer
	// ----------------------------------------


	// STACK 16         STACK32
	// ------------------------------------
	// filt_coeffs
	// out_buffer


	// filt_coeffs & out_buffer
	VoC_lw16_sd(REG2,1,DEFAULT);					// in RAM_Y
	VoC_pop16(REG3,DEFAULT);
			// out_buffer
	 VoC_lw16i_short(INC3,1,IN_PARALLEL);

	// STACK 16         STACK32
	// ------------------------------------
	// filt_coeffs



   VoC_lw16i_short(FORMAT32,3,DEFAULT);
	VoC_lw16i_set_inc(REG0,MIC_FILT_ADDR+1,1);      // in RAM_X
	VoC_lw16i_set_inc(REG1,MIC_FILT_ADDR+1,2);      // in RAM_X

	VoC_loop_i(1,160)

	    VoC_aligninc_pp(REG1,REG1,DEFAULT);
		VoC_inc_p(REG0,IN_PARALLEL);

		VoC_loop_i_short(SECTION_MSDF_SIZE,DEFAULT);
		VoC_lw32z(ACC0,IN_PARALLEL);
		VoC_startloop0
			VoC_bimac32inc_pp(REG1,REG2);
		VoC_endloop0

		VoC_lw16_sd(REG2,0,DEFAULT);				// in RAM_Y
		VoC_shr32_ri(ACC0,-5,DEFAULT);
		VoC_movreg16(REG1,REG0,DEFAULT);
exit_on_warnings = OFF;
		VoC_sw16inc_p(ACC0_HI,REG3,DEFAULT);
exit_on_warnings = ON;

	VoC_endloop1

	VoC_pop16(REG0,DEFAULT);
	VoC_pop16(RA,DEFAULT);

	VoC_lw16i_short(FORMAT32,-1,DEFAULT);
	VoC_return;


}

/**************************************************************************************
 * Function:    Coolsand_BT_AECRunDma
 *
 * Description: Run DMA 
 *
 * Inputs:      no
 *	
 *		REG2:	DMA local address;
 *		REG3:	DMA size;
 *		ACC0:	DMA extern address;
 *
 * Outputs:     no
 *    
 * Used : ALL
 *
 * Version 1.0  Created by  Xuml       08/13/2010
 **************************************************************************************/
void Coolsand_BT_AecRunDma(void)
{
	VoC_push16(RA,DEFAULT);
	VoC_sw16_d(REG2,CFG_DMA_LADDR);
	VoC_sw16_d(REG3,CFG_DMA_SIZE);
	VoC_sw32_d(REG45,CFG_DMA_EADDR);

	VoC_cfg(CFG_CTRL_STALL);
	VoC_NOP();
	VoC_cfg_lg(CFG_WAKEUP_DMAI,CFG_WAKEUP_STATUS);
	VoC_pop16(RA,DEFAULT);
	VoC_NOP();
	VoC_return;
}

/**************************************************************************************
 * Function:    Coolsand_BT_PreProcessFiltering
 *
 * Description: pre-filter.
 *
 * Inputs:   REG0: input addr
 *			 REG2: output addr
 *
 * Outputs:  
 *    
 * Used : ALL
 *
 * Version 1.0  Created by  Xuml       09/09/2010
 **************************************************************************************/

/*
void Coolsand_BT_Agc(void)
{

	VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,1);

	VoC_loop_i(0,160)

		VoC_lw16_p(REG4,REG3,DEFAULT);

		//temp = ABS(in[index]);
		VoC_shr16_ri(REG4,15,DEFAULT);//sign = x >> (sizeof(int) * 8 - 1);
		VoC_movreg16(REG6,REG4,IN_PARALLEL);

		VoC_xor16_rr(REG6,REG4,DEFAULT);//x ^= sign;
		VoC_lw16i_short(FORMAT32,-1,IN_PARALLEL);

		VoC_sub16_rr(REG6,REG6,REG4,DEFAULT); //x -= sign;
		VoC_lw32z(ACC0,IN_PARALLEL);

		//AGC_serror_l = MULTS(temp,AGC_Coeff_l) - AGC_MAXVAL;
		VoC_sub32_rd(ACC0,ACC0,GLOBAL_AGC_MAXVAL);
		
		VoC_lw32_d(REG45,GLOBAL_AGC_Coeff_l);

		VoC_mac32_rr(REG6,REG5,DEFAULT);
		VoC_shru16_ri(REG4,1,IN_PARALLEL);

		VoC_mac32_rr(REG6,REG4,DEFAULT);
		VoC_lw16i_short(FORMAT32,14,IN_PARALLEL);
		
		VoC_lw32_d(ACC1,GLOBAL_AGC_acc_error_l);

		VoC_shr32_ri(ACC1,17,DEFAULT);

		//	if(AGC_serror_l <0)  AGC_serror_l =0;
		VoC_bnltz32_r(Speech_AGC_L1,ACC0)

		VoC_lw16i_short(ACC0_HI,-1,DEFAULT);
		VoC_lw16i_short(ACC0_LO,-1,IN_PARALLEL);
				
Speech_AGC_L1:
	//	AGC_acc_error_l = (AGC_acc_error_l >>17) + (AGC_serror_l <<1); 				
		VoC_add32_rr(ACC1,ACC0,ACC1,DEFAULT);
		VoC_lw16i_short(FORMAT32,14,IN_PARALLEL);

	//	AGC_Coeff_l = AGC_Coeff_l - AGC_acc_error_l ;	
		VoC_sub32_dr(REG45,GLOBAL_AGC_Coeff_l,ACC1);

		VoC_lw16i_short(RL7_LO,0,DEFAULT);
		VoC_lw16i_short(RL7_HI,2,IN_PARALLEL);

		VoC_bgtz32_r(Speech_AGC_L2,REG45)
			VoC_lw16i(REG4,0x7fff);
			VoC_lw16i_short(REG5,0,DEFAULT);
Speech_AGC_L2:

		VoC_bngt32_rr(Speech_AGC_L3,REG45,RL7)
			VoC_movreg32(REG45,RL7,DEFAULT);
Speech_AGC_L3:
	//	out[index] = MULTS(in[index] ,AGC_Coeff_l);
		VoC_shru16_ri(REG4,1,DEFAULT);
		VoC_movreg32(RL7,REG45,IN_PARALLEL);

		VoC_multf32_rp(ACC0,REG4,REG3,DEFAULT);

		VoC_mac32_rp(REG5,REG3,DEFAULT);
		VoC_lw16i_short(FORMAT32,-1,IN_PARALLEL);

		VoC_sw32_d(ACC1,GLOBAL_AGC_acc_error_l);

		VoC_shr32_ri(ACC0,-16,DEFAULT);

		VoC_sw32_d(RL7,GLOBAL_AGC_Coeff_l);

		VoC_sw16inc_p(ACC0_HI,REG3,DEFAULT);
	VoC_endloop0

// VoC_directive: PARSER_OFF
//		VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,1);
//		VoC_NOP();
//		VoC_NOP();
//		fwrite(&RAM_X[((REGS[3].reg==INPUT_SPEECH_BUFFER1_ADDR) ? INPUT_SPEECH_BUFFER1_ADDR : INPUT_SPEECH_BUFFER2_ADDR)/2],1,320,TestFileHandle);
// VoC_directive: PARSER_ON

	
	VoC_return;

}
*/

/**************************************************************************************
 * Function:    Coolsand_BT_Aec
 *
 * Description: AEC.
 *
 * Inputs:   
 *			 
 *
 * Outputs:  
 *    
 * Used : ALL
 *
 * Version 1.0  Created by  Xuml       03/16/2011
 **************************************************************************************/

void Coolsand_BT_Aec(void)
{

	VoC_push16(RA,DEFAULT);
/*
	VoC_lw16d_set_inc(REG0,ENC_INPUT_ADDR_ADDR,2);
	VoC_lw16d_set_inc(REG1,ENC_INPUT_ADDR_ADDR,2);

	VoC_lw16_d(REG2,AEC_MIC_SHIFT_GAIN);

	VoC_lw32inc_p(REG45,REG0,DEFAULT);
	

	VoC_loop_i(0,80)		
		VoC_shr16_rr(REG4,REG2,DEFAULT);
		VoC_shr16_rr(REG5,REG2,IN_PARALLEL);


		VoC_lw32inc_p(REG45,REG0,DEFAULT);		
		VoC_sw32inc_p(REG45,REG1,DEFAULT);		
	VoC_endloop0*/
	
	//backup speech RAMX
	VoC_lw32_d(REG45,GLOBAL_SPEECH_RAMX_BACKUP_ADDR);
    VoC_lw16i(REG2,AEC_GLOBAL_X_BEGIN_ADDR/2);
    VoC_lw16i(REG3,GLOBAL_AEC_RAMX_BACKUP_SIZE);
	VoC_or16_ri(REG5,0x4000);
	
	VoC_jal(Coolsand_BT_AecRunDma);

	//backup speech code
	VoC_lw32_d(REG45,GLOBAL_SPEECH_CODE_BACKUP_ADDR);
    VoC_lw16i(REG2,SECTION_1_START);
    VoC_lw16i(REG3,GLOBAL_AEC_CODE_BACKUP_SIZE);
	VoC_or16_ri(REG5,0x4000);
	VoC_jal(Coolsand_BT_AecRunDma);

  	//load AEC RAMX
	VoC_lw32_d(REG45,GLOBAL_AEC_RAMX_BACKUP_ADDR);
    VoC_lw16i(REG2,AEC_GLOBAL_X_BEGIN_ADDR/2);
    VoC_lw16i(REG3,GLOBAL_AEC_RAMX_BACKUP_SIZE);
	VoC_jal(Coolsand_BT_AecRunDma);

	//load AEC CODE
	VoC_lw32_d(REG45,GLOBAL_AEC_CODE_ADDR);
    VoC_lw16i(REG2,SECTION_1_START);
    VoC_lw16i(REG3,GLOBAL_AEC_CODE_BACKUP_SIZE);
	VoC_jal(Coolsand_BT_AecRunDma);
	
	//AEC
	VoC_jal(Coolsand_BT_AEC);

	//backup AEC RAMX
	VoC_lw32_d(REG45,GLOBAL_AEC_RAMX_BACKUP_ADDR);
    VoC_lw16i(REG2,AEC_GLOBAL_X_BEGIN_ADDR/2);
    VoC_lw16i(REG3,GLOBAL_AEC_RAMX_BACKUP_SIZE);
	VoC_or16_ri(REG5,0x4000);
	VoC_jal(Coolsand_BT_AecRunDma);


	//load speech code
	VoC_lw32_d(REG45,GLOBAL_SPEECH_CODE_BACKUP_ADDR);
    VoC_lw16i(REG2,SECTION_1_START);
    VoC_lw16i(REG3,GLOBAL_AEC_CODE_BACKUP_SIZE);
	VoC_jal(Coolsand_BT_AecRunDma);

	//load speech RAMX
	VoC_lw32_d(REG45,GLOBAL_SPEECH_RAMX_BACKUP_ADDR);
    VoC_lw16i(REG2,AEC_GLOBAL_X_BEGIN_ADDR/2);
    VoC_lw16i(REG3,GLOBAL_AEC_RAMX_BACKUP_SIZE);
	VoC_jal(Coolsand_BT_AecRunDma); 

	VoC_pop16(RA,DEFAULT);
	VoC_NOP();


  	VoC_return;
}



/**************************************************************************************
 * Function:    Coolsand_BT_HighPass
 *
 * Description: no
 *
 * Inputs:       
 *
 * Outputs: 
 *	 
 *    
 * Used : no.
 *
 * NOTE: no.
 *
 * Version 1.0  Created by  Xuml       03/16/2011
 **************************************************************************************/

void Coolsand_BT_HighPassFilter(void)
{

	VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,1);

	VoC_lw16i(REG0,28243);
	VoC_lw16i(REG1,-12411);

	VoC_lw16i(REG2,-14259);


	VoC_lw16_d(REG4,GLOBAL_HIGH_PASS_FILTER2_XN_1_ADDR);
	VoC_lw16_d(REG5,GLOBAL_HIGH_PASS_FILTER2_XN_2_ADDR);

	VoC_lw16_d(REG6,GLOBAL_HIGH_PASS_FILTER2_YN_1_ADDR);
	VoC_lw16_d(REG7,GLOBAL_HIGH_PASS_FILTER2_YN_2_ADDR);

	VoC_lw16i_short(FORMAT32,-1,DEFAULT);
	VoC_lw16i_short(FORMATX,0,DEFAULT);
	VoC_lw16i_short(FORMAT16,(6-16),DEFAULT);

	VoC_loop_i(0,160)

		VoC_multf32_rr(ACC0,REG2,REG4,DEFAULT);//-(((int)14259*(xn_1))<<1)
		VoC_lw32z(ACC1,IN_PARALLEL);

		VoC_lw16i(REG2,14259);			

		VoC_macX_rp(REG2,REG3,DEFAULT);//(int)14259*x
		VoC_macX_rr(REG0,REG6,IN_PARALLEL);//(int)28243*yn_1

		VoC_macX_rr(REG2,REG5,DEFAULT);//(int)14259*xn_2
		VoC_macX_rr(REG1,REG7,IN_PARALLEL);//-(int)12411*yn_2

		VoC_lw16i(REG2,0x0040);	

		VoC_shr32_ri(ACC0,14,DEFAULT);
		VoC_shr32_ri(ACC1,14,DEFAULT);

		VoC_add32_rr(ACC0,ACC0,ACC1,DEFAULT);
		VoC_movreg16(REG5,REG4,IN_PARALLEL);// xn_2=xn_1;

		VoC_shr32_ri(ACC0,-16,DEFAULT);
		VoC_lw16_p(REG4,REG3,IN_PARALLEL)// xn_1=x;

		VoC_movreg16(REG7,REG6,DEFAULT);//  yn_2=yn_1;
		VoC_movreg16(REG6,ACC0_HI,IN_PARALLEL);//  yn_1=y;

		VoC_multf16_rr(REG2,REG6,REG2,DEFAULT);

		VoC_NOP();
		
		VoC_lw16i(REG2,-14259);

		VoC_sw16inc_p(REG2,REG3,DEFAULT);			

	VoC_endloop0

	VoC_sw16_d(REG4,GLOBAL_HIGH_PASS_FILTER2_XN_1_ADDR);
	VoC_sw16_d(REG5,GLOBAL_HIGH_PASS_FILTER2_XN_2_ADDR);

	VoC_sw16_d(REG6,GLOBAL_HIGH_PASS_FILTER2_YN_1_ADDR);
	VoC_sw16_d(REG7,GLOBAL_HIGH_PASS_FILTER2_YN_2_ADDR);

	VoC_return;
}

/**************************************************************************************
 * Function:    Coolsand_BT_NoiseSuppresser
 *
 * Description: no
 *
 * Inputs:       
 *
 * Outputs: 
 *	 
 *    
 * Used : no.
 *
 * NOTE: no.
 *
 * Version 1.0  Created by  Xuml       03/16/2011
 **************************************************************************************/

void Coolsand_BT_NoiseSuppresser(void)
{
	VoC_lw16d_set_inc(REG3,ENC_INPUT_ADDR_ADDR,2);
	VoC_lw16d_set_inc(REG2,ENC_INPUT_ADDR_ADDR,2);

	VoC_lw16i_short(FORMAT16,(9-16),DEFAULT);//must be set with REG6 together
	VoC_lw16i(REG6,512);
	VoC_lw32inc_p(REG01,REG3,DEFAULT);
	VoC_movreg32(REG45,REG01,DEFAULT);

	VoC_loop_i(0,80)
		VoC_bgt16_rd(Coolsand_BT_NoiseSuppress_L0,REG0,CONST_0_ADDR);
		VoC_sub16_dr(REG0,CONST_0_ADDR,REG0);
Coolsand_BT_NoiseSuppress_L0:

		VoC_bgt16_rd(Coolsand_BT_NoiseSuppress_L1,REG1,CONST_0_ADDR);
		VoC_sub16_dr(REG1,CONST_0_ADDR,REG1);
Coolsand_BT_NoiseSuppress_L1:

		VoC_bgt16_rr(Coolsand_BT_NoiseSuppress_L2,REG0,REG6)
		VoC_multf16_rr(REG4,REG0,REG4,DEFAULT);			
Coolsand_BT_NoiseSuppress_L2:

		VoC_bgt16_rr(Coolsand_BT_NoiseSuppress_L3,REG1,REG6)
		VoC_multf16_rr(REG5,REG1,REG5,DEFAULT);			
Coolsand_BT_NoiseSuppress_L3:
		VoC_lw32inc_p(REG01,REG3,DEFAULT);
		VoC_movreg32(REG45,REG01,DEFAULT);
		VoC_sw32inc_p(REG45,REG2,DEFAULT);
	VoC_endloop0
			
	VoC_return;

}

/**************************************************************************************
 * Function:    Coolsand_BT_NotchFilter
 *
 * Description: no
 *
 * Inputs:       
 *
 * Outputs: 
 *	 
 *    
 * Used : no.
 *
 * NOTE: no.
 *
 * Version 1.0  Created by  Xuml       03/16/2011
 **************************************************************************************/

void Coolsand_BT_NotchFilter(void)
{

	VoC_lw16i_short(FORMAT32,0,DEFAULT);
	VoC_lw16i_short(FORMATX,-15,DEFAULT);

	VoC_lw16i(REG0,-16193);
	VoC_lw16i(REG1,-16002);

	VoC_lw16i_set_inc(REG2,GLOBAL_NOTCH_FILTER_XN12_YN12_ADDR,2);
	VoC_lw16i_set_inc(REG3,GLOBAL_NOTCH_FILTER_COEF_ADDR,1);

	VoC_loop_i(1,15)

		VoC_push32(REG23,DEFAULT);

		VoC_lw32inc_p(REG45,REG2,DEFAULT);
		VoC_lw32inc_p(RL6,REG2,DEFAULT);

		VoC_lw32_p(RL7,REG2,DEFAULT);
		VoC_shr32_ri(RL6,-1,IN_PARALLEL);

		VoC_lw16_p(REG2,REG3,DEFAULT);
		VoC_shr32_ri(RL7,-1,IN_PARALLEL);

		VoC_lw16_d(REG3,ENC_INPUT_ADDR_ADDR);
		
		VoC_loop_i(0,160)

			VoC_movreg32(REG67,RL6,DEFAULT);
			VoC_lw32z(ACC1,IN_PARALLEL);

			VoC_multf32_rr(ACC0,REG2,REG4,DEFAULT);//((int)31919*(xn_1))
			VoC_shru16_ri(REG6,1,IN_PARALLEL);
						
			VoC_mac32_rp(REG0,REG3,DEFAULT);//(int)-16193*x
			VoC_mac32_rr(REG2,REG6,IN_PARALLEL);//(int)31919*yn_1

			VoC_mac32_rr(REG0,REG5,DEFAULT);//(int)-16193*xn_2
			VoC_macX_rr(REG2,REG7,IN_PARALLEL);//(int)31919*yn_1

			VoC_movreg16(REG5,REG4,DEFAULT);// xn_2=xn_1;
			VoC_movreg32(REG67,RL7,IN_PARALLEL);
			
			VoC_shru16_ri(REG6,1,DEFAULT);
			VoC_macX_rr(REG1,REG7,IN_PARALLEL);//(int)-16002*yn_2

			VoC_lw16_p(REG4,REG3,DEFAULT);// xn_1=x;
			VoC_mac32_rr(REG1,REG6,IN_PARALLEL);//(int)-16002*yn_2

			VoC_shr32_ri(ACC0,14,DEFAULT);			
			VoC_shr32_ri(ACC1,14,DEFAULT);

			VoC_sub32_rr(ACC0,ACC1,ACC0,DEFAULT);
			VoC_movreg32(RL7,RL6,IN_PARALLEL);//  yn_2=yn_1;
						
			VoC_shr32_ri(ACC0,-16,DEFAULT);			
			VoC_movreg32(RL6,ACC0,IN_PARALLEL);//  yn_1=y;

			VoC_shr32_ri(RL6,-1,DEFAULT);

			VoC_sw16inc_p(ACC0_HI,REG3,DEFAULT);			

		VoC_endloop0

		VoC_pop32(REG23,DEFAULT);

		VoC_shr32_ri(RL6,1,DEFAULT);
		VoC_shr32_ri(RL7,1,IN_PARALLEL);

		VoC_sw32inc_p(REG45,REG2,DEFAULT);
		VoC_inc_p(REG3,IN_PARALLEL);

		VoC_sw32inc_p(RL6,REG2,DEFAULT);
		VoC_sw32inc_p(RL7,REG2,DEFAULT);

	VoC_endloop1

	VoC_return;

}


/**************************************************************************************
 * Function:    Coolsand_BT_NonClipDigitalGain
 *
 * Description: no
 *
 * Inputs:  REG0  
 *			REG1
 *			REG2
 *
 * Outputs: REG0
 *	 
 *    
 * Used : ALL.
 *
 * NOTE: no.
 *
 * Version 1.0  Created by  Xuml       07/05/2011
 **************************************************************************************/
void Coolsand_BT_NonClipDigitalGain(void)
{
#if 0
	voc_short GLOBAL_NON_CLIP_CROSSZEROINDEX[FRAMESIZE],y		
#endif



	VoC_lw16_p(REG4,REG3,DEFAULT);
		
	VoC_push32(REG23,DEFAULT);
	VoC_movreg32(RL7,REG01,IN_PARALLEL);

	VoC_sw16_d(REG4,GLOBAL_DIGITAL_OLD_GAIN_ADDR);

	VoC_lw16i(REG7,FRAMESIZE);

	VoC_add16_rr(REG3,REG1,REG7,DEFAULT);
	VoC_lw16i_short(INC3,2,IN_PARALLEL);

	VoC_loop_i(0,(FRAMESIZE/2))
		VoC_lw32inc_p(REG45,REG2,DEFAULT);
		VoC_lw32inc_p(REG67,REG0,DEFAULT);
		VoC_sw32inc_p(REG45,REG1,DEFAULT);		
		VoC_sw32inc_p(REG67,REG3,DEFAULT);		
	VoC_endloop0
	
	VoC_lw32z(REG67,DEFAULT);//REG6:i;REG7:CrossZeroConter
	VoC_movreg16(REG1,RL7_HI,IN_PARALLEL);//REG0;outbuf;REG1:HistoryData

	VoC_lw16i(REG5,FRAMESIZE*2-1);

	VoC_lw16i_short(INC1,1,DEFAULT);
	VoC_lw16i_short(REG4,1,IN_PARALLEL);

	VoC_lw16i_set_inc(REG0,GLOBAL_NON_CLIP_CROSSZEROINDEX,1);

Coolsand_BT_NonClipDigitalGain_L0:
		VoC_lw16inc_p(REG2,REG1,DEFAULT);
		VoC_lw16_p(REG3,REG1,DEFAULT);
		VoC_bltz16_r(Coolsand_BT_NonClipDigitalGain_L1,REG2)
		VoC_bnltz16_r(Coolsand_BT_NonClipDigitalGain_L1,REG3)
			VoC_sw16inc_p(REG6,REG0,DEFAULT);//CrossZeroIndex[j]=i;
			VoC_add16_rr(REG7,REG7,REG4,IN_PARALLEL);//CrossZeroConter++;

			VoC_lw16i(REG2,FRAMESIZE);
			VoC_bngt16_rr(Coolsand_BT_NonClipDigitalGain_L3,REG2,REG6);
		VoC_jump(Coolsand_BT_NonClipDigitalGain_L2)
Coolsand_BT_NonClipDigitalGain_L1:
		VoC_bgtz16_r(Coolsand_BT_NonClipDigitalGain_L2,REG2)
		VoC_bngtz16_r(Coolsand_BT_NonClipDigitalGain_L2,REG3)
			VoC_sw16inc_p(REG6,REG0,DEFAULT);//CrossZeroIndex[j]=i;
			VoC_add16_rr(REG7,REG7,REG4,IN_PARALLEL);//CrossZeroConter++;
			
			VoC_lw16i(REG2,FRAMESIZE);
			VoC_bngt16_rr(Coolsand_BT_NonClipDigitalGain_L3,REG2,REG6);
Coolsand_BT_NonClipDigitalGain_L2:
		VoC_add16_rr(REG6,REG6,REG4,DEFAULT);//i++
	VoC_bgt16_rr(Coolsand_BT_NonClipDigitalGain_L0,REG5,REG6)

Coolsand_BT_NonClipDigitalGain_L3:

	//REG7:CrossZeroConter++
	//REG6:i


	VoC_lw16i_short(REG2,2,DEFAULT);

// VoC_directive: PARSER_OFF

	printf("CrossZeroConter:%4d  ",REGS[7].reg);
// VoC_directive: PARSER_ON	
	VoC_bgt16_rr(Coolsand_BT_NonClipDigitalGain_End,REG2,REG7)

/*		VoC_lw16i_short(INC0,1,DEFAULT);
			VoC_movreg32(REG01,RL7,IN_PARALLEL);
		
		VoC_lw16i_short(INC1,1,DEFAULT);
		VoC_lw16i_short(REG3,1,IN_PARALLEL);

		VoC_add16_rd(REG2,REG3,GLOBAL_NON_CLIP_CROSSZEROINDEX);
		
		VoC_loop_r_short(REG2,DEFAULT);
		VoC_lw16inc_p(REG4,REG1,IN_PARALLEL);
		VoC_startloop0
			VoC_lw16inc_p(REG4,REG1,DEFAULT);
			VoC_sw16inc_p(REG4,REG0,DEFAULT);		
		VoC_endloop0
		*/


		// find the peak.
		//for (i=0;i<CrossZeroConter-1;i++)
		
		VoC_lw16i_short(INC0,1,DEFAULT);			
		
		VoC_lw16i_short(INC1,1,DEFAULT);
		VoC_lw16i_short(REG3,1,IN_PARALLEL);

		VoC_movreg32(REG01,RL7,DEFAULT);
		VoC_sub16_rr(REG7,REG7,REG3,IN_PARALLEL);

		VoC_lw16i_set_inc(REG2,GLOBAL_NON_CLIP_CROSSZEROINDEX,1);

		VoC_add16_rr(REG0,REG0,REG3,DEFAULT);
		VoC_add16_rr(REG1,REG1,REG3,IN_PARALLEL);

		VoC_movreg32(RL6,REG01,DEFAULT);

		VoC_loop_r(1,REG7)

			VoC_lw16inc_p(REG3,REG2,DEFAULT);

			VoC_sub16_pr(REG4,REG2,REG3,DEFAULT);
			VoC_add16_rr(REG0,REG0,REG3,IN_PARALLEL);
			
			VoC_add16_rr(REG1,REG1,REG3,DEFAULT);
			VoC_lw32z(REG67,IN_PARALLEL);//CrossZeroPeak

			VoC_movreg16(REG3,REG1,DEFAULT);

			VoC_loop_r(0,REG4)
				VoC_lw16inc_p(REG5,REG1,DEFAULT);//int temp_value=HistoryData[CrossZeroIndex[i]+j];
				VoC_bnltz16_r(Coolsand_BT_NonClipDigitalGain_L5,REG5)
					VoC_sub16_rr(REG5,REG6,REG5,DEFAULT);
	Coolsand_BT_NonClipDigitalGain_L5:
				VoC_bngt16_rr(Coolsand_BT_NonClipDigitalGain_L6,REG5,REG7)
					VoC_movreg16(REG7,REG5,DEFAULT);
	Coolsand_BT_NonClipDigitalGain_L6:
				VoC_NOP();
			VoC_endloop0

			VoC_movreg16(REG1,REG3,DEFAULT);

			VoC_bngt16_rd(Coolsand_BT_NonClipDigitalGain_L7,REG7,GLOBAL_DIGITAL_MAXVALUE_ADDR)

				VoC_lw16i_short(REG5,1,DEFAULT);
				VoC_lw32z(ACC0,IN_PARALLEL);
					
				VoC_lw16i_short(FORMAT16,(4-16),DEFAULT);
				VoC_shr32_ri(REG67,16,IN_PARALLEL);

				VoC_lw16i(ACC0_HI,30000);

				VoC_shr32_ri(ACC0,12,DEFAULT);	
				VoC_lw16i_short(REG3,0,IN_PARALLEL);

				VoC_loop_i(0,255)
					VoC_sub32_rr(ACC0,ACC0,REG67,DEFAULT);
					VoC_add16_rr(REG3,REG3,REG5,IN_PARALLEL);

					VoC_bgt32_rr(Coolsand_BT_NonClipDigitalGain_L8,REG67,ACC0);
				VoC_endloop0
					
			VoC_jump(Coolsand_BT_NonClipDigitalGain_L8);
	Coolsand_BT_NonClipDigitalGain_L7:
			VoC_lw16i(REG5,150);
	
			VoC_lw16i_short(FORMAT16,(4-16),DEFAULT);		
			VoC_lw16_d(REG3,GLOBAL_DIGITAL_GAIN_ADDR);
			VoC_bez16_d(Coolsand_BT_NonClipDigitalGain_L8,GLOBAL_DIGITAL_GAIN_ADDR)

			VoC_bgt16_rr(Coolsand_BT_NonClipDigitalGain_L9,REG7,REG5)

				VoC_lw16_d(REG7,GLOBAL_DIGITAL_OLD_GAIN_ADDR);
				//VoC_lw16i_short(FORMAT16,(4-16),DEFAULT);		
				VoC_lw16_d(REG3,GLOBAL_NOISE_DIGITAL_GAIN_ADDR);

				VoC_bngt16_rr(Coolsand_BT_NonClipDigitalGain_L8,REG7,REG3)
					VoC_lw16i_short(REG3,1,DEFAULT);
					VoC_sub16_rr(REG3,REG7,REG3,DEFAULT);				
				VoC_jump(Coolsand_BT_NonClipDigitalGain_L8);
Coolsand_BT_NonClipDigitalGain_L9:
			VoC_lw16_d(REG7,GLOBAL_DIGITAL_OLD_GAIN_ADDR);
		//	VoC_lw16i_short(FORMAT16,(4-16),DEFAULT);		
			VoC_lw16_d(REG3,GLOBAL_DIGITAL_GAIN_ADDR);

			VoC_bgt16_rr(Coolsand_BT_NonClipDigitalGain_L8,REG7,REG3)
					VoC_lw16i_short(REG3,2,DEFAULT);
					VoC_add16_rr(REG3,REG7,REG3,DEFAULT);				
Coolsand_BT_NonClipDigitalGain_L8:	
			
			VoC_loop_r(0,REG4)

				VoC_multf16_rp(REG7,REG3,REG1,DEFAULT);	

				VoC_NOP();

				VoC_NOP();

				VoC_sw16inc_p(REG7,REG1,DEFAULT);

			VoC_endloop0

			VoC_sw16_d(REG3,GLOBAL_DIGITAL_OLD_GAIN_ADDR);

			//GLOBAL_RECEIVER_DIGITAL_MAXVALUE_ADDR
			VoC_movreg32(REG01,RL6,DEFAULT);

		VoC_endloop1


Coolsand_BT_NonClipDigitalGain_End:


	VoC_pop32(REG23,DEFAULT);
	VoC_lw16i_short(INC0,2,IN_PARALLEL);

	VoC_lw16_d(REG4,GLOBAL_DIGITAL_OLD_GAIN_ADDR);

	VoC_movreg32(REG01,RL7,DEFAULT);
	VoC_lw16i_short(INC1,2,IN_PARALLEL);
	
	VoC_sw16_p(REG4,REG3,DEFAULT);
	VoC_lw16i_short(INC2,2,IN_PARALLEL);

	VoC_lw32inc_p(REG45,REG1,DEFAULT);		

	VoC_loop_i(0,(FRAMESIZE/2))
		VoC_lw32inc_p(REG45,REG1,DEFAULT);
		VoC_sw32inc_p(REG45,REG0,DEFAULT);		
	VoC_endloop0
		
	VoC_loop_i(0,(FRAMESIZE/2))
		VoC_lw32inc_p(REG45,REG1,DEFAULT);
		VoC_sw32inc_p(REG45,REG2,DEFAULT);		
	VoC_endloop0

	VoC_return;

}
