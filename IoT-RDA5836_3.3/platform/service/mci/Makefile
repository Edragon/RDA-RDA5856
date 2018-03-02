## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/mci


# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
                     application/coolmmi \
                     application/coolmmi/media/common \
                     application/coolmmi/mmi_csdapp/MDI \
                     platform/system/mdi \
                     platform/rtos/sx \
                     platform/chip/defs \
                     application/coolmmi/debug \
                     platform/chip/hal \
				     platform/chip/${CT_ASIC}/hal \
                     platform/chip/comn/hal \
                     platform/chip/comn\
                     platform/service \
                     platform/edrv/lcdd \
                     platform/svc/cams \
                     platform/svc/imgs \
                     platform/svc/mps \
                     platform/svc/mrs \
                     platform/svc/uctls \
                     platform/svc/umss \



# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS :=  

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
