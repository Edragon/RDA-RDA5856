## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/aud/fm

LOCAL_API_DEPENDS := \
					platform \
					platform/edrv \
					platform/rtos/common \
					platform/rtos/sx \
					platform/system/calib \
				    platform/chip/hal \
				    platform/chip/defs \
					platform/chip/comn/hal \
					platform/chip/${CT_ASCI}/hal \
					platform/chip/comn\
					platform/edrv/aud \
					platform/edrv/aud/codec_gallite \

FM_USING_I2S ?= 0
ifeq "${FM_USING_I2S}" "1"
LOCAL_EXPORT_FLAG += FM_USING_I2S
endif

ifeq "${FM_MODEL}" "rda5802e_gallite"
LOCAL_EXPORT_FLAG += INTERNAL_FM
endif
ifeq "${FM_MODEL}" "rdafm_8809"
LOCAL_EXPORT_FLAG += INTERNAL_FM
endif
ifeq "${FM_MODEL}" "rdafm_8809p"
LOCAL_EXPORT_FLAG += INTERNAL_FM
endif
ifeq "${FM_MODEL}" "rdafm_5855"
LOCAL_EXPORT_FLAG += INTERNAL_FM
endif

# FM line-in record
ifeq "${FM_LINEIN_RECORD}" "1"
LOCAL_EXPORT_FLAG += FM_LINEIN_RECORD
endif

## Include source files
include src/make.srcs

# C_SRC := ${notdir ${wildcard *.c}}
# S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

