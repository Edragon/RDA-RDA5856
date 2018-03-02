## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/service/gui

# Compile the sub-modules, except when the "base" must be used as a library.
ifeq ($(findstring platfrom/gui/packed,${GLOBAL_BINARY_LIBS}),)

# list all modules APIs that are neccessary to compile this module
# some extra dependencies are added for Jade
LOCAL_API_DEPENDS := \
                    platform/service/base/system
			    

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \
		platform/service/gui/rgui


else # GLOBAL_BINARY_LIBS

endif # GLOBAL_BINARY_LIBS

IS_TOP_LEVEL := yes
# Module lib copy to ./lib/${ASIC}
IS_MODULE_LIB := yes
## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
#C_SRC := ${notdir ${wildcard src/*.c}}

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
