## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := apps/ap_record

# Compile the sub-modules, except when the "base" must be used as a library.
ifeq ($(findstring apps/ap_record,${GLOBAL_BINARY_LIBS}),)

# list all modules APIs that are neccessary to compile this module
# some extra dependencies are added for Jade
LOCAL_API_DEPENDS := \
            platform \
						target/${CT_TARGET} \
						platform/chip/comn \
						platform/service/base \
						platform/chip/comn/hal \
						platform/service/base/bal \
						platform/service/gui/rgui \
						platform/service/bt/btadp \
						platform/system/mdi \
            platform/service \
            platform/edrv/lcdd \
            platform/edrv/fmd \
						at/ate \
            apps/ap_gui \
            platform/mdi \
            apps/ap_main \
            apps/ap_music \
            apps/system \
    	    	apps/gui \
			    

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \


else # GLOBAL_BINARY_LIBS

endif # GLOBAL_BINARY_LIBS

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
