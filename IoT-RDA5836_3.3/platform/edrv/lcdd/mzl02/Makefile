## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/lcdd/mzl02

LOCAL_API_DEPENDS := \
        platform \
        platform/rtos/common \
        platform/chip/hal \
        platform/edrv/lcdd \
        platform/edrv/lcdd/mzl02 \
        platform/edrv/pmd \
        platform/edrv \
        target \
        platform/rtos/sx \
        platform/chip/defs

LOCAL_ADD_INCLUDE := platform/edrv/lcdd/src

ifeq (${LCD_IF}, gouda)
LOCAL_MODULE_DEPENDS := ${LOCAL_NAME}/gouda

IS_TOP_LEVEL := yes

else

C_SRC := ${notdir ${wildcard src/*.c}}
S_SRC :=

endif

## --------------Don't change the line below------------------ ##

include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
