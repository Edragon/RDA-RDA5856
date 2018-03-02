## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/usbdevice

LOCAL_MODULE_DEPENDS :=     \
        platform/service/usbdevice/storage   \
        platform/service/usbdevice/audio   \
        platform/service/usbdevice/hid   \

         
LOCAL_API_DEPENDS := \
		platform \
         platform/service                  \
         platform/service/usbdevice                  \
         platform/service/usbdevice/storage             \
         platform/service/usbdevice/audio             \
         platform/chip/hal                  \
		 platform/chip/${CT_ASIC}/hal \
         platform/chip/comn/hal \
         platform/chip/comn\
         platform/chip/defs                 \
         platform/rtos/sx               \
		 platform/phy \
		 platform/chip/defs \
		 platform/edrv/aud

# This is a top-level module
IS_TOP_LEVEL := yes
# Module lib copy to ./lib/${ASIC}
IS_MODULE_LIB := yes

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
LOCAL_EXPORT_FLAG +=

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC := 
#setjmp.S
C_SRC := ${notdir ${wildcard src/*.c}}

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
