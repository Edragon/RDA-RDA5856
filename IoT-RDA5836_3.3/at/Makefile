## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := at

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \
                    at/ate \
                    at/ats \
                    at/utility \

# This is a top-level module
IS_TOP_LEVEL := yes
# Module lib copy to ./lib/${ASIC}
IS_MODULE_LIB := yes
## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
