## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/dsm


# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
                     platform/system/mdi \
                     platform/rtos/sx \
                     platform/chip/defs \
                     platform/chip/hal \
	             platform/chip/${CT_ASIC}/hal \
	             platform/chip/${CT_ASIC}/regs \
                     platform/chip/${CT_ASIC}/boot \
                     platform/chip/comn/hal \
	             platform/chip/comn/boot \
                     platform/chip/comn\
                     platform/service \
                     platform/service/dsm \
                     platform/service/base \
                     platform/service/base/bal \
                     platform/edrv/lcdd \
                     platform/edrv/memd \
                     platform/svc/cams \
                     platform/svc/imgs \
                     platform/svc/mps \
                     platform/svc/mrs \
                     platform/svc/uctls \
                     platform/svc/umss \
		     target \



# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS :=  

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##

LOCAL_EXPORT_FLAG += USER_DATA_BLOCK_SIZE=${USER_DATA_BLOCK_SIZE} USER_DATA_SIZE=${USER_DATA_SIZE}

IS_TOP_LEVEL := yes
# Module lib copy to ./lib/${ASIC}
IS_MODULE_LIB := yes


## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
