## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/lcdd/ssd1306/gouda

LOCAL_API_DEPENDS := \
        platform \
        platform/rtos/common \
				platform/chip/${CT_ASIC}/hal \
				platform/chip/comn/hal \
				platform/chip/comn\
        platform/edrv/lcdd \
        platform/chip/regs/$(CT_ASIC)   \
        platform/edrv/lcdd/ssd1306 \
        platform/edrv/pmd \
        platform/edrv \
        target \
        platform/rtos/sx \
        platform/chip/defs

LOCAL_ADD_INCLUDE := platform/edrv/lcdd/src

C_SRC := ${notdir ${wildcard src/*.c}}
S_SRC :=


## --------------Don't change the line below------------------ ##

include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
