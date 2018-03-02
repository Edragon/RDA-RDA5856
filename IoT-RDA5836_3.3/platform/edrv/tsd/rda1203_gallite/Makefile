## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/tsd/rda1203_gallite

LOCAL_API_DEPENDS ?= platform \
                     platform/chip/hal \
					 platform/chip/${CT_ASIC}/hal \
                     platform/chip/comn/hal \
                     platform/chip/comn\
                     platform/chip/defs \
                     platform/service/dsm \
                     platform/edrv/tsd \
                     platform/edrv \
                     platform/service \
                     target \
                     platform/rtos/sx \
                     platform/edrv/pmd \
                     platform/edrv/pmd/rda1203_gallite

C_SRC := tsd.c
S_SRC :=


## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
