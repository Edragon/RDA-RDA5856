#**************************************************************#
# COPY THIS FILE AS "Makefile" IN THE "src" DIR OF YOUR MODULE #
# AND CUSTOMIZE IT TO FIT YOUR NEEDS.                          #
#**************************************************************#


## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := RDA5856

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"




ifneq "${BUILD_BTCPU_IMAGE}" "1"
ifneq "${BUILD_BCPU_IMAGE}" "1"

include ${SOFT_WORKDIR}/env/compilation/module.mk
else # "${BUILD_BCPU_IMAGE}" "1"
LOCAL_MODULE_DEPENDS += platform/chip/bcpu
LOCAL_MODULE_DEPENDS += platform/rtos/std
LOCAL_MODULE_DEPENDS += platform/chip/${CT_ASIC}/patch
endif

else # "${BUILD_BTCPU_IMAGE}" "1"
LOCAL_MODULE_DEPENDS += platform/chip/${CT_ASIC}/btcpu
#LOCAL_MODULE_DEPENDS += platform/chip/${CT_ASIC}
LOCAL_MODULE_DEPENDS += platform/rtos/std
LOCAL_MODULE_DEPENDS += platform/edrv/btd
#LOCAL_MODULE_DEPENDS += platform/service/bt
ifeq  ("${TGT_MOD_SERVICE_BT}"  , "src")
    LOCAL_MODULE_DEPENDS += platform/service/bt
else
    ifeq  ("${TGT_MOD_SERVICE_BT}"  , "lib")
    	ifeq  ("${COMBINED_HOST}"  , "0")
    	   ifeq ("${BLUETOOTH_USE_MARLIN}"  , "1")
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
LOCAL_MODULE_DEPENDS += target
endif

ifeq (0, 1)


LOCAL_MODULE_DEPENDS := 

#LOCAL_MODULE_DEPENDS += apps
#LOCAL_MODULE_DEPENDS += platform/chip
#LOCAL_MODULE_DEPENDS += platform/rtos
#LOCAL_MODULE_DEPENDS += platform/edrv
#LOCAL_MODULE_DEPENDS += platform/service
#LOCAL_MODULE_DEPENDS += target

ifneq "${BUILD_BTCPU_IMAGE}" "1"
ifneq "${TFLASH_PROG}" "yes"
LOCAL_MODULE_DEPENDS += ${APP_BASEDIR}
endif
LOCAL_MODULE_DEPENDS += platform/chip
LOCAL_MODULE_DEPENDS += platform/rtos
LOCAL_MODULE_DEPENDS += platform/edrv
LOCAL_MODULE_DEPENDS += platform/service
LOCAL_MODULE_DEPENDS += target
else
#LOCAL_MODULE_DEPENDS += platform/chip
LOCAL_MODULE_DEPENDS += platform/chip/${CT_ASIC}
LOCAL_MODULE_DEPENDS += platform/chip/${CT_ASIC}/btcpu
LOCAL_MODULE_DEPENDS += platform/rtos/std
LOCAL_MODULE_DEPENDS += platform/edrv/btd
LOCAL_MODULE_DEPENDS += platform/service/bt
endif

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := 

#LOCAL_LIB += lib/gb/lib/libGB_dev_tc_plus_ml.a
#LOCAL_LIB += lib/gb/lib/libGB_v5.1_dev.a
#LOCAL_LIB += lib/gb/lib/libGB_debug.a
#LOCAL_LIB += lib/gb/lib/libGB_dev_tc.a
#LOCAL_LIB += lib/gbmllib/lib/libgbmllib_debug.a

ifneq ($(findstring rda,${BT_MODEL}),)
ifeq "${CT_USER}" "CUSTOM"
LOCAL_LIB += lib/librdabthost.a
else
ifneq ($(findstring BT_NO_TRACE,${TARGET_EXPORT_FLAG}),)
#LOCAL_LIB += platform/edrv/btd/rdaStack/librdabthost_release.a
else
#LOCAL_LIB += platform/edrv/btd/rdaStack/librdabthost_debug.a
endif
endif
ifeq "${CHIP_HAS_BTCPU}" "1"
#LOCAL_LIB += platform/edrv/btd/rdaDriver/src/libmips_be_core.a
#LOCAL_LIB += platform/edrv/btd/rdaDriver/src/libprth_chimera_be_hal.a
endif
endif

#AEC_NS_PITCH_lib
#LOCAL_LIB += platform/mdi/alg/noise_suppress/src/libnoise_suppress_release.a
ifeq "${CT_USER}" "CUSTOM"
LOCAL_LIB += lib/libbase_release.a  
LOCAL_LIB += lib/libcsw_release.a  
endif

ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_240_320" 
#LOCAL_LIB += application/jmetoc/libjmetoc_240_320.a
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_176_220" 
#LOCAL_LIB += application/jmetoc/libjmetoc_176_220.a
endif
#ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_128_160" 
#LOCAL_LIB += application/jmetoc/libjmetoc_128_160.a
#endif
#ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_128_128" 
#LOCAL_LIB += application/jmetoc/libjmetoc_128_128.a
#endif
#ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_220_176" 
#LOCAL_LIB += application/jmetoc/libjmetoc_220_176.a
#endif
ifeq "$(strip ${MMI_SUPPORT_WAPMMS})" "YES" 
ifeq "$(strip ${MMI_SUPPORT_JAT_SEC})" "YES" 
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libcommon_cool_profile.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libtransport_cool_profile.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libbrowser_cool_profile.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libjmms_cool_profile.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libsb.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libsbpms.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libsbrc5.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libshared_ssl.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libsslplus.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libsslplusmm.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libsslplusrad.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libshared_wtls.a
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libwtls.a
else
#LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libcommon_cool_profile_nosec.a
#LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libtransport_cool_profile_nosec.a
#LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libbrowser_cool_profile_nosec.a
#LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libjmms_cool_profile_nosec.a
endif

ifeq "$(strip ${MMI_SUPPORT_JAT_OMA_DLD})" "YES" 
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libdc_cool_profile.a
endif
#LOCAL_LIB += application/thirdpartylibs/jwidgets/lib/libjwidgets_debug.a

ifeq "$(strip ${MMI_SUPPORT_JAT_OTA})" "YES" 
LOCAL_LIB += application/thirdpartylibs/wapmms/lib/libota_cool_profile.a
endif
endif

ifeq "$(strip ${MMI_SUPPORT_VIDEOCHAT})" "YES" 
LOCAL_LIB += application/thirdpartylibs/kkmitlib/lib/libkkmitlib_debug.a
endif

ifeq "$(strip ${MMI_SUPPORT_IPTV})" "YES" 
LOCAL_LIB += application/thirdpartylibs/zyxlib/lib/libZyxlib_debug.a
LOCAL_LIB += application/thirdpartylibs/zyxlib/lib/libzyxvc_debug.a
LOCAL_LIB += application/thirdpartylibs/zyxlib/lib/libzypc_debug.a
LOCAL_LIB += application/thirdpartylibs/zyxlib/lib/libcartoon_debug.a
endif

endif 
# VOIS needs this lib. As we want keep only one VOIS lib, we have to add this lib alway.
#ifeq "$(strip ${MMI_SUPPORT_MAGIC_SOUND})" "YES" 
#LOCAL_LIB += application/thirdpartylibs/magic_sound/lib/lib_ejae.a
#endif

# Set this to any non-null string to signal a module which
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_ENTRY_POINT := yes



# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
#LOCAL_EXPORT_FLAG += PAL_NO_DBGTRC PAL_NO_PRINTF
#LOCAL_EXPORT_FLAG += AERO2
#LOCAL_EXPORT_FLAG += USE_CT_CALIB
#LDFLAGS += --wrap sxs_BackTrace 
#LOCAL_EXPORT_FLAG += __GPRS__


# The following are only usable with the romulator
#LOCAL_EXPORT_FLAG += PAL_PROFILING
#LOCAL_EXPORT_FLAG += HAL_PROFILING
#LOCAL_EXPORT_FLAG += PAL_NO_TRACE
LOCAL_EXPORT_FLAG += RFD_NO_TRACE

#Add by lixp for WAP/MMS
#LOCAL_EXPORT_FLAG += WAP_MMS_SUPPORT
# Start the Stack ?
#LOCAL_EXPORT_FLAG += __SXS_ONLY__

# Ask target to register the extra modules.
export TGT_REGISTER_PLATFORM_VERSIONS := 1

# Version of the MMI.
#LOCAL_EXPORT_FLAG += COOLMMI_VERSION_STRING=${CT_TARGET}

#LOCAL_EXPORT_FLAG += __DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__

WITH_FLASH:=1


## --------------------------- ##
## Override toplevel settings  ##
## --------------------------- ##

# Set more dependencies as binary : they will not be compiled from source.
LOCAL_BINARY_LIBS ?=

# Set dependencies as source : the library will be generated from source
# This supersedes the BINARY (local+global) setting in case of conflict
LOCAL_SRC_LIBS ?=

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##


## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##

# Assembly / C code
S_SRC := 
C_SRC := 

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##

include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


