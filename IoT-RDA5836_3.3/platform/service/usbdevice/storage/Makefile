## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/usbdevice/storage

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS ?= platform                           \
                     platform/service                  \
                     platform/service/usbdevice                  \
                     platform/service/usbdevice/storage             \
                     platform/chip/hal                  \
				     platform/chip/${CT_ASIC}/hal \
                     platform/chip/comn/hal \
                     platform/chip/comn\
                     platform/chip/defs                 \
                     platform/rtos/sx \
                     platform/edrv/aud \
                     platform/edrv/gpio

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS ?=

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##

LOCAL_EXPORT_FLAG +=

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
