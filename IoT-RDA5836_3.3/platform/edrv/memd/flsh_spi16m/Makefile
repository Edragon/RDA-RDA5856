## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/memd/flsh_spi16m

LOCAL_API_DEPENDS := \
                        platform \
                        platform/rtos/common \
                        platform/chip/hal \
                        platform/chip/defs \
						platform/chip/${CT_ASIC}/hal \
						platform/chip/${CT_ASIC}/boot \
                        platform/chip/comn/hal \
                        platform/chip/comn \
                        platform/chip/${CT_ASIC}/defs \
                        platform/edrv/memd \
                        platform/edrv \
                        platform/rtos/${TARGET_OS} \
                        platform/rtos/std

C_SRC := drv_flsh.c drv_flsh_map.c
S_SRC :=

LOCAL_EXPORT_FLAG += USER_DATA_BASE=${USER_DATA_BASE}
LOCAL_EXPORT_FLAG += USER_DATA_SIZE=${USER_DATA_SIZE}
LOCAL_EXPORT_FLAG += USER_DATA_BLOCK_SIZE=${USER_DATA_BLOCK_SIZE}
LOCAL_EXPORT_FLAG += FLASH_SIZE=${FLASH_SIZE}
LOCAL_EXPORT_FLAG += FLASH_BASE=${FLASH_BASE}
## --------------Don't change the line below------------------ ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
