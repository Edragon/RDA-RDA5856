## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/aud/bt

LOCAL_API_DEPENDS := \
					platform \
					platform/edrv \
					platform/rtos/common \
					platform/system/calib\
					platform/rtos/sx \
					platform/chip/hal \
					platform/chip/comn/hal \
					platform/chip/comn\
					platform/chip/${CT_ASIC}/hal \
					platform/chip/defs \
					platform/edrv/aud \
					platform/svc/vois \
					platform/edrv/aud/codec_gallite \


## Include source files
include src/make.srcs

# C_SRC := ${notdir ${wildcard *.c}}
# S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

