## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/system/mdi/ars

# list all modules APIs that are neccessary to compile this module
LOCAL_API_DEPENDS := platform \
					 platform/rtos/common \
					 platform/chip/hal \
					 platform/chip/${CT_ASIC}/hal \
                     platform/chip/comn/hal \
                     platform/chip/comn\
					 platform/chip/defs \
					 platform/edrv/aud \
					 platform/system/mdi \
                     platform/vpp/speech \
                     target \
                     target/${CT_TARGET} \
					 platform/rtos/sx \
 					 platform/system/calib \
					 platform/system/mdi/alg/sco_vpp\
 					 
 					 

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS :=

LOCAL_EXPORT_FLAG += ARS_PROFILE

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs


# TODO: remove this
WITHOUT_WSHADOW := 1

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
