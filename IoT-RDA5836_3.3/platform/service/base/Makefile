## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/base

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := \
	platform \
	platform/chip/hal \
	platform/rtos/common \
	platform/chip/defs \
	platform/stack \
	platform/service \

# list all the modules that need to be compiled prior to using this module
ifeq "${WIFI_MODEL}" "mcuwifi"
LOCAL_MODULE_DEPENDS := \
	platform/service/base/cos \

else

LOCAL_MODULE_DEPENDS := \
	platform/service/base/cos \
	platform/service/base/ffs \
	platform/service/base/sul \
	platform/service/base/tm \
	platform/service/base/ml \
	platform/service/base/ver \
	platform/service/base/vds \
	platform/service/base/bal\
	platform/service/base/reg \
	platform/service/base/tools\

#platform/service/base/nvram \

ifeq "${GSM_SUPPORT}" "yes"
LOCAL_MODULE_DEPENDS += platform/service/base/db
endif

endif	

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


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
