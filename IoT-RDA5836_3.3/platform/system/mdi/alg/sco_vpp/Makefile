## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/system/mdi/alg/sco_vpp

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
						platform \
						platform/system/mdi \
						platform/system/mdi/alg/wma_decoder \
						platform/system/mdi/alg/amrring_codec_voc \
						platform/system/mdi/alg/audiojpeg_dec_voc \
						platform/system/mdi/alg/sbc_vpp \
						platform/system/mdi/alg/avi_dec \
						platform/system/mdi/alg/avi_enc \
						platform/system/mdi/alg/analog_audio_iir \
						platform/system/mdi/alg/voc_common \
						platform/service \
						platform/chip/defs \
						platform/system/calib \
						platform/rtos/common \
						platform/ \
						application/coolmmi/debug \
						application/systeminc/mmi \
						platform/chip/hal \
						platform/chip/${CT_ASIC}/hal \
                        platform/chip/comn/hal \
                        platform/chip/comn\
						platform/rtos/sx \
						platform/edrv \
						platform/edrv/aud \
						target/ \
						target/${CT_TARGET} \


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

#CFLAGS += -isystem 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
