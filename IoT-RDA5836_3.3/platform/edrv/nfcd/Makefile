## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/edrv/nfcd

LOCAL_API_DEPENDS := platform \
				 	 platform/rtos/common \
					 platform/chip/hal \
                     platform/rtos/sx \
					 platform/rtos/std \
					 target \


ifneq (${NFC_MODEL},)
LOCAL_MODULE_DEPENDS := platform/edrv/nfcd/${NFC_MODEL}
endif

# This is a top-level module
IS_TOP_LEVEL = yes
# Copy moudle lib to ./lib/${ASIC}
IS_MODULE_LIB := yes
# This needs to deliver environment related files also
DELIV_ENV_FILES := 0

# Flags exported throughout the build
LOCAL_EXPORT_FLAG +=


## ------------------------------------------------------------------------- ##
##	                       Source files                                      ##
## ------------------------------------------------------------------------- ##
# No source files in the top-level itself...
S_SRC :=
C_SRC := 
INPUT_FILES :=  

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


