## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/pmd/pmu_5856e

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS ?= platform \
                     platform/system/calib \
					 platform/service \
                     platform/rtos/common \
                     platform/rtos/sx \
                     platform/chip/defs \
                     platform/chip/hal \
                     platform/chip/hal/${CT_ASIC} \
                     platform/chip/${CT_ASIC}/defs \
					 platform/chip/${CT_ASIC}/regs \
                     platform/chip/comn/hal \
                     platform/chip/comn \
                     platform/chip/${CT_ASIC}/hal \
                     platform/edrv \
                     platform/edrv/dualsimd \
                     platform/edrv/pmd \
                     platform/edrv/rfd \
                     platform/edrv/tsd/  \
                     platform/edrv/tsd/${TSD_MODEL} \
					 platform/service/base/bal \
                     target \

LOCAL_ADD_INCLUDE := platform/edrv/pmd/src


# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS ?= 

ifeq "$(strip ${FM_MODEL})" "rdafm_5856e"
LOCAL_EXPORT_FLAG += RDAFM_5856e
endif

ifeq "$(strip ${BT_MODEL})" "rdabt_5856e"
LOCAL_EXPORT_FLAG += RDABT_5856e
endif

ifeq "${LCD_BACKLIGHT_CHARGE_PUMP}" "0"
LOCAL_EXPORT_FLAG += LCD_BACKLIGHT_CHARGE_PUMP=0
else
LOCAL_EXPORT_FLAG += LCD_BACKLIGHT_CHARGE_PUMP=1
endif

ifeq "${AUD_SPK_LINE_USAGE}" "left"
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_ONLY=1 AUD_SPK_LINE_R_ONLY=0
else
ifeq "${AUD_SPK_LINE_USAGE}" "right"
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_ONLY=0 AUD_SPK_LINE_R_ONLY=1
else
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_ONLY=0 AUD_SPK_LINE_R_ONLY=0
endif
endif

# The default value of L&R double mode depends on class modes
ifeq "${AUD_SPK_LINE_L_R_DOUBLE}" "0"
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_R_DOUBLE=0
endif
ifeq "${AUD_SPK_LINE_L_R_DOUBLE}" "1"
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_R_DOUBLE=1
endif

ifeq "${AUD_SPK_LINE_CLASS_MODE}" "class_ab"
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_CLASS_AB=1 AUD_SPK_LINE_R_CLASS_AB=1 AUD_SPK_LINE_NO_CLASS_K=1
endif
ifeq "${AUD_SPK_LINE_CLASS_MODE}" "class_d"
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_CLASS_AB=0 AUD_SPK_LINE_R_CLASS_AB=0 AUD_SPK_LINE_NO_CLASS_K=1
endif
ifeq "${AUD_SPK_LINE_CLASS_MODE}" "class_k"
LOCAL_EXPORT_FLAG += AUD_SPK_LINE_L_CLASS_AB=0 AUD_SPK_LINE_R_CLASS_AB=0 AUD_SPK_LINE_NO_CLASS_K=0
endif

LOCAL_EXPORT_FLAG += 

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
# Generate the CoolProfile header automatically during the make process.
AUTO_XCP2H ?= yes

## ------------------------------------- ##
##      List all your sources here       ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


