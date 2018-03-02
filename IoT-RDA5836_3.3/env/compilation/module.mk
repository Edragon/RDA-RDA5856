

## src - module compile from srouce code
## lib - module is a lib, and the lib provide by vendor
## no -  the module is not needed for the target
##-----------------------------------------------------------------------------
## Chip Module: boot, hal, regs
## 
## RTOS Module: sx
##
## EDRV Module: FM, PM, AUD, GPIO, FLASH, etc 
##
## SERVICE Module: bt, wifi
## 

### Don't change below code unless you know what you do

################################################################################
## NO. 1 chip 
################################################################################

ifeq ("${TGT_MOD_CHIP}"  , "src")
  LOCAL_MODULE_DEPENDS += platform/chip/${CT_ASIC}
else
	ifeq ("${TGT_MOD_CHIP}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/lib${CT_ASIC}_${CT_RELEASE}.a
	endif 
endif 


	 
################################################################################
## NO. 2 RTOS
################################################################################
ifeq  ("${TGT_MOD_RTOS}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/rtos
else
    ifeq  ("${TGT_MOD_RTOS}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/librtos_${CT_RELEASE}.a
	endif
endif


################################################################################
## EDRV 
################################################################################

################################################################################
## NO.3 UART
################################################################################
ifeq  ("${TGT_MOD_EDRV_UART}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/uart2
else
    ifeq  ("${TGT_MOD_EDRV_AUD_CODEC_GALLITE}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libuart2_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.4 AUD
################################################################################


################################################################################
## NO.5 GPIO
################################################################################

ifeq  ("${TGT_MOD_EDRV_GPIO}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/gpio
else
    ifeq  ("${TGT_MOD_EDRV_GPIO}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libgpio_${CT_RELEASE}.a
	endif
endif

ifeq  ("${TGT_MOD_EDRV_I2CGPIO}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/i2cgpio
else
    ifeq  ("${TGT_MOD_EDRV_I2CGPIO}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libi2cgpio_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.6 AUD
################################################################################
ifeq  ("${TGT_MOD_EDRV_AUD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/aud
else
    ifeq  ("${TGT_MOD_EDRV_AUD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libaud_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.7 BTD
################################################################################

ifeq  ("${TGT_MOD_EDRV_BTD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/btd
else
    ifeq  ("${TGT_MOD_EDRV_BTD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libbtd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.8 FMD
################################################################################

ifeq  ("${TGT_MOD_EDRV_FMD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/fmd
else
    ifeq  ("${TGT_MOD_EDRV_FMD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libfmd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.9 NFCD 
################################################################################
ifeq  ("${TGT_MOD_EDRV_NFCD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/nfcd
else
    ifeq  ("${TGT_MOD_EDRV_NFCD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libnfcd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.10 LCDD
################################################################################
ifeq  ("${TGT_MOD_EDRV_LCDD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/lcdd
else
    ifeq  ("${TGT_MOD_EDRV_LCDD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/liblcdd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.11 MCD 
################################################################################
ifeq  ("${TGT_MOD_EDRV_MCD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/mcd
else
    ifeq  ("${TGT_MOD_EDRV_MCD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libmcd_${CT_RELEASE}.a
	endif
endif

## Flash driver 
## 
################################################################################
## NO.12 MEMD 
################################################################################
ifeq  ("${TGT_MOD_EDRV_MEMD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/memd
else
    ifeq  ("${TGT_MOD_EDRV_MEMD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libmemd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.13 PMD
################################################################################
ifeq  ("${TGT_MOD_EDRV_PMD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/pmd
else
    ifeq  ("${TGT_MOD_EDRV_PMD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libpmd_${CT_RELEASE}.a
	endif
endif

## XCV 
##
################################################################################
## NO.14 RFD 
################################################################################
ifeq  ("${TGT_MOD_EDRV_RFD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/rfd
else
    ifeq  ("${TGT_MOD_EDRV_RFD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/librfd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.15 TSD 
################################################################################

ifeq  ("${TGT_MOD_EDRV_TSD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/tsd
else
    ifeq  ("${TGT_MOD_EDRV_TSD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libtsd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.16 WIFID 
################################################################################

ifeq  ("${TGT_MOD_EDRV_WIFID}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/wifid
else
    ifeq  ("${TGT_MOD_EDRV_WIFID}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libwifid_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.17 CAM
################################################################################
ifeq  ("${TGT_MOD_EDRV_CAMDOLD}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/camdold
else
    ifeq  ("${TGT_MOD_EDRV_CAMDOLD}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libcamdold_${CT_RELEASE}.a
	endif
endif


################################################################################
## NO.18 GPS
################################################################################
ifeq  ("${TGT_MOD_EDRV_GPS}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/gpsd
else
    ifeq  ("${TGT_MOD_EDRV_GPS}"  , "lib")
        LOCAL_LIB += lib/${CT_ASIC}/libgpsd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.19 SIM
################################################################################
ifeq  ("${TGT_MOD_EDRV_SIM}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/simd
else
    ifeq  ("${TGT_MOD_EDRV_SIM}"  , "lib")
        LOCAL_LIB += lib/${CT_ASIC}/libsimd_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.20 SE security equipment
################################################################################
ifeq  ("${TGT_MOD_EDRV_SE}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/se
else
    ifeq  ("${TGT_MOD_EDRV_SE}"  , "lib")
        LOCAL_LIB += lib/${CT_ASIC}/libse_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.20 voice trigger model
################################################################################
ifeq  ("${TGT_MOD_EDRV_VOICETRIGGER}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/voicetrigger
else
    ifeq  ("${TGT_MOD_EDRV_VOICETRIGGER}"  , "lib")
        LOCAL_LIB += lib/${CT_ASIC}/libvoicetrigger_${CT_RELEASE}.a
	endif
endif


################################################################################
## NO.20 temperature sensor model
################################################################################
ifeq  ("${TGT_MOD_EDRV_TEMPSENSOR}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/tempsensor
else
    ifeq  ("${TGT_MOD_EDRV_TEMPSENSOR}"  , "lib")
        LOCAL_LIB += lib/${CT_ASIC}/libtempsensor_${CT_RELEASE}.a
	endif
endif
##
## Service
##
################################################################################
## NO.18 DRIVER 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_DRIVER}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/driver
else
    ifeq  ("${TGT_MOD_SERVICE_DRIVER}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libdriver_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.19 PM
################################################################################
ifeq  ("${TGT_MOD_SERVICE_PM}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/pm
else
    ifeq  ("${TGT_MOD_SERVICE_PM}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libpm_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.20 DM
################################################################################
ifeq  ("${TGT_MOD_SERVICE_DM}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/dm
else
    ifeq  ("${TGT_MOD_SERVICE_DM}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libdm_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.21 DSM
################################################################################

ifeq  ("${TGT_MOD_SERVICE_DSM}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/dsm
else
    ifeq  ("${TGT_MOD_SERVICE_DSM}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libdsm_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.22 USBDEVICE 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_USBDEVICE}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/usbdevice
else
    ifeq  ("${TGT_MOD_SERVICE_USBDEVICE}"  , "lib")
         LOCAL_LIB += platform/lib/${CT_ASIC}/libusbdevice_${CT_RELEASE}.a
	endif
endif



ifeq  ("${TGT_MOD_SERVICE_USBHOST}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/usbhost
else
    ifeq  ("${TGT_MOD_SERVICE_USBHOST}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libusbhost_${CT_RELEASE}.a
	endif
endif


################################################################################
## NO.23 TOOLS 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_TOOLS}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/tools
else
    ifeq  ("${TGT_MOD_SERVICE_TOOLS}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libtools_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.24 CFW 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_CFW}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/cfw
else
    ifeq  ("${TGT_MOD_SERVICE_CFW}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libcfw_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.25 GUI
################################################################################
ifeq  ("${TGT_MOD_SERVICE_GUI}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/gui
else
    ifeq  ("${TGT_MOD_SERVICE_GUI}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libgui_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.26  AT 
################################################################################

ifeq  ("${TGT_MOD_SERVICE_AT}"  , "src")
    LOCAL_MODULE_DEPENDS += at
else
    ifeq  ("${TGT_MOD_SERVICE_AT}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libat_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.27 NET
################################################################################
ifeq  ("${TGT_MOD_SERVICE_NET}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/net
else
    ifeq  ("${TGT_MOD_SERVICE_NET}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libnet_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.28 BT
################################################################################
ifeq  ("${TGT_MOD_SERVICE_BT}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/bt
else
    ifeq  ("${TGT_MOD_SERVICE_BT}"  , "lib")
    	ifeq  ("${COMBINED_HOST}"  , "0")
    	   ifeq  ("${BLUETOOTH_USE_MARLIN}"  , "1")
    	  	LOCAL_LIB += platform/lib/${CT_ASIC}/libbt_marlin_${CT_RELEASE}.a
    	   else
        	LOCAL_LIB += platform/lib/${CT_ASIC}/libbt_${CT_RELEASE}.a
           endif
       	else
       	    ifeq ("${CHIP_SUPPORT_LE}", "yes")
        	LOCAL_LIB += platform/lib/${CT_ASIC}/libbt_le_combined_${CT_RELEASE}.a
            else
        	LOCAL_LIB += platform/lib/${CT_ASIC}/libbt_combined_${CT_RELEASE}.a
            endif
       	endif
    endif
endif

################################################################################
## NO.29 IOTAPI 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_IOTAPI}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/iotapi
else
    ifeq  ("${TGT_MOD_SERVICE_IOTAPI}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libiotapi_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.30 MDI 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_MDI}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/system/mdi
else
    ifeq  ("${TGT_MOD_SERVICE_MDI}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libmdi_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.30 ZSP 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_ZSP}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/system/zsp
else
    ifeq  ("${TGT_MOD_SERVICE_ZSP}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libzsp_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.31 BASE 
################################################################################
ifeq  ("${TGT_MOD_SERVICE_BASE}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/base
else
    ifeq  ("${TGT_MOD_SERVICE_BASE}"  , "lib")
         LOCAL_LIB += platform/lib/${CT_ASIC}/libbase_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.32 WIFI
################################################################################
ifeq  ("${TGT_MOD_SERVICE_WIFI}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/wifi
else
    ifeq  ("${TGT_MOD_SERVICE_WIFI}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libwifi_${CT_RELEASE}.a
    endif
endif

################################################################################
## NO.33 GSM STACK
################################################################################
ifeq  ("${TGT_MOD_SERVICE_GSM_STACK}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/system/stack
else
    ifeq  ("${TGT_MOD_SERVICE_GSM_STACK}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libstack_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.34 AGPS
################################################################################
ifeq  ("${TGT_MOD_SERVICE_AGPS}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/agps
else
    ifeq  ("${TGT_MOD_SERVICE_AGPS}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libagps_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.35 CALIB
################################################################################
ifeq  ("${TGT_MOD_SERVICE_CALIB}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/system/calib
else
    ifeq  ("${TGT_MOD_SERVICE_CALIB}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libcalib_${CT_RELEASE}.a
	endif
endif


################################################################################
## NO.36 FDLibm
################################################################################
ifeq  ("${TGT_MOD_SERVICE_FDLIBM}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/fdlibm
else
    ifeq  ("${TGT_MOD_SERVICE_FDLIBM}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libfdlibm_${CT_RELEASE}.a
	endif
endif
################################################################################
## NO.37 LED
################################################################################
ifeq  ("${TGT_MOD_EDRV_LED}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/edrv/led
else
    ifeq  ("${TGT_MOD_EDRV_LED}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libled_${CT_RELEASE}.a
	endif
endif

################################################################################
## NO.38 YOC Service
################################################################################
ifeq  ("${TGT_MOD_SERVICE_YOC}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/yoc
else
    ifeq  ("${TGT_MOD_SERVICE_YOC}"  , "lib")
        LOCAL_LIB += platform/lib/${CT_ASIC}/libyoc_${CT_RELEASE}.a
	endif
endif


################################################################################
## Always include target 
################################################################################
LOCAL_MODULE_DEPENDS += target


################################################################################
## Always include apps in case TGT_MOD_APP_LIST is not emtpy 
################################################################################
ifneq ("${TGT_MOD_APP_LIST}", "")

ifneq ("${TFLASH_PROG}" , "yes")
LOCAL_MODULE_DEPENDS += apps 
endif 

endif 

## Contains the base module ./target, other cutomers' modules or thirdparty module
## should also add in below configuration file
#include ${SOFT_WORKDIR}/env/compilation/custom_module.mk
