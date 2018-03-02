## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/i2cgpio/i2c_gpio

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
                     platform/rtos/common \
                     target  \
                     platform/rtos/${TARGET_OS} \
                     platform/chip/${CT_ASIC}/hal \
                     platform/chip/comn/hal \
                     platform/chip/comn/ \
                     platform/chip/defs\
                     platform/stack \
                     platform/edrv/fmd \
                     platform/edrv/i2cgpio \
                     platform/edrv/pmd \
                     platform/edrv \


# TODO : remove
LOCAL_API_DEPENDS += platform/system/calib


                     
# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := 

## ------------------------------------- ##
##      List all your sources here       ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


