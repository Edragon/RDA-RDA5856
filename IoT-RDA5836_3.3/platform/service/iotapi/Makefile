## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/service/iotapi


# Add includes from other modules we do not wish to link to
# FIXME: Improve the inclusion of target dependencies ! 
LOCAL_API_DEPENDS := \
					platform \
					platform/service \
					platform/rtos/sx \
					platform/system/mdi \
					platform/system/mdi/include \
					platform/service/base/bal \
					platform/service/base/system \
					platform/service/bt \
					platform/service/bt/btadp \
					platform/service/bt/btadp\include \
					platform/chip/hal \
					platform/chip/${CT_ASIC}/hal \
                    platform/chip/comn/hal \
                    platform/chip/comn \
					platform/edrv/wifid/rdawifi \
					platform/service/wifi \
					platform/edrv/wifid/${WIFI_MODEL} \
					platform/service/net/tcpip \
					platform/edrv \
					apps \
					apps/ap_main \
					apps/ap_main/include \
					apps/ap_btspeaker\
                    apps/ap_btspeaker/include \
					target \

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := \


# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := no

# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG +=

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
#S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
#C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
