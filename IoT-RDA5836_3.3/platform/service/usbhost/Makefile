## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/usbhost

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS ?= platform                           \
                     platform/service                  \
                     platform/service/usbdevice                  \
                     platform/service/usbdevice/umss             \
                     platform/service/usbdevice/uctls             \
                     platform/chip/hal                  \
					 platform/chip/${CT_ASIC}/hal \
		     platform/chip/${CT_ASIC}/regs \
                     platform/chip/comn/hal \
                     platform/chip/comn\
                     platform/chip/defs                 \
                     platform/chip/regs/${CT_ASIC} \
                     platform/rtos/sx \
                     platform/edrv/mcd	\
                     

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS ?=

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##

LOCAL_EXPORT_FLAG +=

# This is a top-level module
IS_TOP_LEVEL := yes
# Module lib copy to ./lib/${ASIC}
IS_MODULE_LIB := yes
## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
