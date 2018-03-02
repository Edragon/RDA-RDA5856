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
LOCAL_NAME :=  platform/service/dm

# list all modules APIs that are neccessary to compile this module
# FIXME: Improve the inclusion of target dependencies !
LOCAL_API_DEPENDS := \
	platform \
	platform/chip/hal \
	platform/chip/${CT_ASIC}/hal \
    platform/chip/comn/hal \
    platform/chip/comn\
	platform/rtos/common \
	platform/chip/defs \
	platform/stack \
	platform/rtos/sx \
	platform/system/calib \
	platform/service \
	platform/service/cfw \
	platform/chip/pal \
	platform/svc/ars \
	platform/svc/vois \
	platform/vpp/speech \
	platform/phy \
	target \
	target/${CT_TARGET} \
	platform/edrv/aud \
	platform/edrv/pmd \
  platform/edrv/pmd/${PM_MODEL} \
	platform/edrv/lcdd \
	platform/edrv/memd \
	platform/edrv/mcd/ \
	platform/edrv/mcd/${MCD_MODEL} \
	platform/edrv/tsd/ \
	platform/edrv/tsd/${TSD_MODEL} \
	platform/edrv/fmd/ \
	platform/edrv/fmd/${FM_MODEL} \
	platform/system/mdi/ars/ \
	platform/system/mdi/mmc/ \
	platform/system/mdi/ \
	platform/system/mdi/alg/sco_vpp \

ifeq (${CSW_EXTENDED_API_AUDIO_VIDEO},1)
LOCAL_API_DEPENDS += \
	platform/svc/mps \
	
endif

# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := \



# For a toplevel module, define which tests to include in delivery
# Skip the 'tests' in the names. Beware that everything from the 
# given tests directories will be included...
TOPLEVEL_DELIVER_TESTS := 

# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += __NEW_ENV__

include mk

IS_TOP_LEVLE := yes
IS_MODULE_LIB := yes

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
