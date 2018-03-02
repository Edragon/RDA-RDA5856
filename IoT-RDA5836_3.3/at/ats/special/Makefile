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
LOCAL_NAME := at/ats/special

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_DEPEND_LIBS := 

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                     application/systeminc/mmi \
                     at/ats/cfg \
                     platform \
                     platform/rtos/sx \
                     platform/chip/defs \
                     platform/chip/hal platform/chip/comn/hal platform/chip/${CT_ASIC}/hal \
                     platform/edrv/aud\
                     platform/service/wifi \
                     platform/service \
                     platform/edrv/pmd \
                     platform/edrv/aud \
                     platform/svc/uctls \
                     platform/system/stack \
                     at/ate at/utility \
                     platform/system/calib \
                     platform/system/mdi/mmc \
                     platform/system/mdi/alg/sco_vpp \
                     platform/system/mdi/alg/sbc_dec_voc \
                     platform/system/mdi/alg/audiojpeg_dec_voc \
                     target

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

LOCAL_EXPORT_FLAG += \
		HAVE_CFW_CONFIG \
		MF32_BASE_REG_DISABLE \
		MF32_NO_RFM_RH \
		MF32_NO_RFM \
		MF32_CFW_MMI_TWO_TASKS

ifeq "${CT_RELEASE}" "cool_profile"
	LOCAL_EXPORT_FLAG += CSW_PROFILING
endif
# Sound recorder
ifeq "${SOUND_RECORDER_SUPPORT}" "1"
LOCAL_EXPORT_FLAG  += SOUND_RECORDER_SUPPORT
endif
## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
#C_SRC := ${notdir ${wildcard src/*.c}}
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
