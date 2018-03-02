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

# Name of the module
LOCAL_NAME := at/ats

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
#LOCAL_DEPEND_LIBS := apps/at/ats/special
                     
# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := at/ats/main
LOCAL_MODULE_DEPENDS += $(foreach mod,${AT_SUPPORT_MOD},at/ats/${mod})

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := yes


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


