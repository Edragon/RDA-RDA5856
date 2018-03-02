## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/system/mdi/alg/noise_suppress

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
						platform/system/mdi \
						platform/system/mdi/mmc \
						platform/system/mdi/alg/common \
						platform/system/mdi/alg/amrring_codec_voc \
						platform/system/mdi/alg/noise_suppress \
						platform/system/mdi/alg/voc_common \
						platform/ \
						platform/service/ \
                  		platform/rtos/std \
                		platform/rtos/sx \
                		platform/chip/regs/$(CT_ASIC)\
                		platform/chip/defs \
                		platform/chip/hal \
					    platform/chip/${CT_ASIC}/hal \
                        platform/chip/comn/hal \
                        platform/chip/comn\
                		platform/edrv/aud \
						application/coolmmi/debug \
						application/systeminc/mmi \
						application/coolmmi/interface/config \




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
# Assembly / C code
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files
%/fftwrap.o: MYCFLAGS=-O3
%/filterbank.o: MYCFLAGS=-O3
%/kiss_fft.o: MYCFLAGS=-O3
%/kiss_fftr.o: MYCFLAGS=-O3
%/mdf.o: MYCFLAGS=-O3
%/preprocess.o: MYCFLAGS=-O3
%/smallft.o: MYCFLAGS=-O3
%/testecho.o: MYCFLAGS=-O3
## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
