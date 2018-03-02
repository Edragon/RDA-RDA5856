## --------------Don't change the line below------------------ ##
# For more information on the different variables, and others you
# may define, see ${SOFT_DIR}/Makefile.skel and/or read the doc
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

LOCAL_NAME := platform/edrv/fmd/rda5802na

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
                     platform/base/common \
                     target  \
                     platform/base/sx \
                     platform/service/base/bal \
                     platform/chip/hal \
                     platform/chip/defs\
                     platform/stack \
                     platform/edrv/fmd \
                     platform/edrv/pmd \
                     platform/edrv \
                    platform/edrv/i2cgpio \

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


