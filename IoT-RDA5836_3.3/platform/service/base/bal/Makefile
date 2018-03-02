## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/base/bal

# Compile the sub-modules, except when the "base" must be used as a library.
ifeq ($(findstring apps/gui/packed,${GLOBAL_BINARY_LIBS}),)

# list all modules APIs that are neccessary to compile this module
# some extra dependencies are added for Jade
LOCAL_API_DEPENDS := \
				platform \
				platform/service \
				platform/service/base \
				platform/service/usbdevice \
				platform/service/net/tcpip \
				platform/service/usbdevice/storage \
				platform/chip/hal \
				platform/chip/${CT_ASIC}/hal \
				platform/chip/${CT_ASIC}/regs\
        platform/chip/comn/hal \
        platform/chip/comn\
				platform/chip/defs \
				platform/chip/regs/${CT_ASIC} \
				platform/rtos/sx \
				platform/edrv/lcdd \
				platform/edrv/pmd \
				platform/edrv/pmd/${PM_MODEL} \
				platform/edrv/fmd \
				platform/edrv/aud \
				platform/system/mdi\
				platform/edrv/gpio \
				platform/service/dsm \
				platform/service/bt \
				platform/service/bt/btadp \
				platform/system/mdi/mmc \
				platform/system/mdi/ars \
				platform/service/dm \
				platform/edrv/memd \
				apps/ap_main \
				platform/service/gui/rgui \
				apps/ap_gui    \
				apps/system \
				apps/ap_bluetooth \
				apps/ap_fm \
				apps/projects_cfg/${CT_PROJECT} \
				target \
			    

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \


else # GLOBAL_BINARY_LIBS

endif # GLOBAL_BINARY_LIBS

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
