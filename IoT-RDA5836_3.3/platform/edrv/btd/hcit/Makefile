## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/edrv/btd/hcit

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                platform \
                platform/mdi \
                platform/rtos/std \
                platform/rtos/sx \
                platform/chip/defs \
                platform/chip/hal \
                platform/chip/comn\
                platform/chip/comn/hal \
                platform/chip/${CT_ASIC}/hal \
                platform/chip/hal/${CT_ASIC} \
                platform/service \
                platform/edrv \
                platform/edrv/btd/rdaDriver \
                platform/edrv/pmd \
                platform/service/bt \
                platform/service/bt/btadp \
                target \


# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += 
WITHOUT_WERROR := 1

## ------------------------------------------------------------------------- ##
##                           Source files                                      ##
## ------------------------------------------------------------------------- ##
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


