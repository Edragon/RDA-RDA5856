## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_WORKDIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/rfd/xcv_5856e

LOCAL_API_DEPENDS := platform \
                     platform/rtos/common \
                     platform/chip/hal \
                     platform/chip/defs \
                     platform/chip/regs/${CT_ASIC} \
                     platform/chip/${CT_ASIC}/defs \
                     platform/chip/${CT_ASIC}/hal \
		     platform/chip/${CT_ASIC}/regs \
                     platform/chip/comn/hal \
		     platform/chip/comn \
                     platform/edrv \
                     platform/edrv/rfd \
                     platform/rtos/sx \
                     platform/system/calib \

LOCAL_MODULE_DEPENDS := 

ifeq "${PRE_P5_BOARD}" "1"
LOCAL_EXPORT_FLAG += PRE_P5_BOARD
endif

C_SRC := drv_xcv.c drv_xcv_init.c
S_SRC :=

## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
