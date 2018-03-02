## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/aud/codec_gallite

LOCAL_API_DEPENDS := \
					target \
					target/${CT_TARGET} \
					platform \
					platform/rtos/common \
					platform/rtos/sx \
					platform/system/calib \
					platform/chip/${CT_ASIC}/hal \
					platform/chip/${CT_ASIC}/regs\
					platform/chip/comn\
					platform/chip/comn/hal\
					platform/chip/hal \
		    	                platform/chip/hal/${CT_ASIC} \
					platform/chip/defs \
					platform/edrv \
					platform/edrv/aud \
					platform/edrv/pmd \
					platform/edrv/rfd \
					platform/chip/regs/$(CT_ASIC)   \

## Include source files
include src/make.srcs

# C_SRC := ${notdir ${wildcard *.c}}
# S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

