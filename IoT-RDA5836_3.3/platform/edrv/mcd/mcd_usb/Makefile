## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/mcd/mcd_usb

LOCAL_API_DEPENDS := \
                     platform \
                     platform/chip/hal \
                     platform/chip/regs \
                     platform/chip/regs/${CT_ASIC} \
                     platform/chip/defs \
		             platform/chip/${CT_ASIC}/hal \
					 platform/chip/${CT_ASIC}/regs \
                     platform/chip/comn/hal \
                     platform/chip/comn\
                     platform/edrv/mcd \
                     platform/edrv/pmd \
                     platform/edrv \
                     target \
                     platform/rtos/sx \
					 platform/service


C_SRC := mcd_usb.c \
				 usb_hcd.c
S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
