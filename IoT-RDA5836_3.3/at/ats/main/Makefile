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
LOCAL_NAME := at/ats/main

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_DEPEND_LIBS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := at/ate \
					 at/utility \
					 apps/ap_main\
					 platform/service/base  \
					 platform/service/base/bal  \
					 platform \
					 platform/rtos/sx \
					 platform/chip/defs \
					 platform/chip/hal \
					 platform/chip/${CT_ASIC}/hal \
					 platform/chip/comn/hal \
					 platform/chip/comn\
					 platform/edrv/pmd\
					 platform/service

#Add all at module "include" path
AP_AT_MAIN_DEPENDS := ${filter-out Makefile, ${notdir ${wildcard ../*}}}
LOCAL_API_DEPENDS +=${foreach AP_AT_MAIN_DEPENDS_PATH, ${AP_AT_MAIN_DEPENDS},${dir ${LOCAL_NAME}}${AP_AT_MAIN_DEPENDS_PATH}}


# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := no

# For a toplevel module, define which tests to include in delivery
# Skip the 'tests' in the names. Beware that everything from the 
# given tests directories will be included...
TOPLEVEL_DELIVER_TESTS := 


## --------------------------- ##
## Override toplevel settings  ##
## --------------------------- ##

# Set more dependencies as binary : they will not be compiled from source.
LOCAL_BINARY_LIBS ?=

# Set dependencies as source : the library will be generated from source
# This supersedes the BINARY (local+global) setting in case of conflict
LOCAL_SRC_LIBS ?=


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
