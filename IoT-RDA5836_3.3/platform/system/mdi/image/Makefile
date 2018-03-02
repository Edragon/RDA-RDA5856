## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/system/mdi/image

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS :=  \
                platform \
                platform/service \
                platform/rtos/sx \
                platform/rtos/std \
                platform/chip/defs \
                platform/chip/hal \
			    platform/chip/${CT_ASIC}/hal \
                platform/chip/comn/hal \
                platform/chip/comn\
                platform/system/mdi \
                platform/system/mdi/alg/common \
                platform/system/mdi/alg/audiojpeg_dec_voc \
                platform/system/mdi/mmc \



# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAG += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.

# voc vid dec
ifeq "${MEDIA_VOCVID_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += MEDIA_VOCVID_SUPPORT
endif


#CT_MIPS16_CFLAGS := -mno-mips16
## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files
%/pngread.o: MYCFLAGS=-O3
%/pnghandlefunc.o: MYCFLAGS=-O3
%/mmi_png_dec.o: MYCFLAGS=-O3

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
