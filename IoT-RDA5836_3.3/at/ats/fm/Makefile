#**************************************************************#
# COPY THIS FILE AS "Makefile" IN THE "src" DIR OF YOUR MODULE #
# AND CUSTOMIZE IT TO FIT YOUR NEEDS.                          #
#**************************************************************#


## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := at/ats/fm

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_DEPEND_LIBS := 

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := platform \
                     platform/rtos/sx \
                     platform/chip/defs \
                     platform/chip/hal \
					 platform/chip/${CT_ASIC}/hal \
                     platform/chip/comn/hal \
                     platform/chip/comn\
                     platform/edrv/aud\
                     platform/service/base \
                     platform/service/base/bal \
                     platform/service/gui/rgui \
                     platform/service/bt/btadp \
                     platform/edrv/fmd \
                     platform/service/wifi \
                     platform/service \
                     apps/ap_main \
                     apps/system \
                     apps/ap_camera \
                     apps/ap_fm \
                     apps/ap_wifispeaker \
                     at/ate \
                     at/ats \
                     at/utility\
                     platform/service/net/tcpip \
                     platform/service/net/dlna \
                     platform/service/net/dlna/util \
                     platform/service/net/dlna/net \
                     platform/service/net/dlna/upnp \
                     platform/service/net/dlna/upnp/ssdp \
                     platform/service/net/dlna/upnp/event \
                     platform/service/net/dlna/upnp/control \
                     platform/service/net/dlna/http \
                     platform/service/net/dlna/xml \
                     platform/service/net/dlna/soap \
                     platform/service/net/dlna/avrender \
					 at/ate \
					 platform/edrv/pmd \
                     platform/edrv/aud \
                     platform/svc/uctls \
                     platform/netdevice \
                     platform/netdevice/upnpstd/av \
                     target/${CT_TARGET} \

# Set this to any non-null string to signal a module which 
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_ENTRY_POINT := no

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := no

# For a toplevel module, define which tests to include in delivery
# Skip the 'tests' in the names. Beware that everything from the 
# given tests directories will be included...
TOPLEVEL_DELIVER_TESTS := 

# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
IS_TOP_LEVEL := no

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
C_SRC := ${notdir ${wildcard src/*.c}}


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
