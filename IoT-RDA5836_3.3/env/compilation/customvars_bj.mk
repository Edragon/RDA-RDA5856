#Please add the follow line in your customvars.mk
#include ${SOFT_WORKDIR}/env/compilation/customvars_bj.mk

## Extra flags exported to all makefiles.
GLOBAL_EXPORT_FLAG += 

## This will define the default target for which you will compile,
## if you don't specify a CT_TARGET on the command line.
#CT_TARGET ?= granite_fpga
#CT_TARGET ?= granite_eng
#CT_TARGET ?= granite_eng_norom
#CT_TARGET ?= greenstone_fpga
#CT_TARGET ?= greenstone_fpga_real
CT_TARGET ?= greenstone_eng_fake16m
#CT_TARGET ?= greenstone_cs2308a

## This is useful to compile your code as a Ramrun.
#CT_RAMRUN := 1

## We can force to use a Romuator here (normally this is defined by the target).
#CT_ROMULATOR = yes

## This defines the release mode in which you compile.
CT_RELEASE ?= cool_profile

## Define VERBOSE to get more information during the make process.
#VERBOSE = 1

## To use the ccache tool (accelerate the compilation).
#CCACHE_ENABLE := 1

#WITH_PERL := 1
#WITH_SVN ?= 1

AUTO_XCP2H := no
ifeq "${CT_FIRST}" "YES"
AUTO_XMD2H := yes
else
AUTO_XMD2H := no
endif

CT_USER ?=MMI

ifdef CT_PRODUCT
GLOBAL_EXPORT_FLAG += CT_PRODUCT=${CT_PRODUCT}
endif

# Quick fix for USB problem
GLOBAL_EXPORT_FLAG += DIRTY_WORKAROUND_FOR_USB_CT1010
# Work around for frame reentry assert
GLOBAL_EXPORT_FLAG += DIRTY_WORKAROUND_FOR_FRAME_REENTRY
# Quick fix for USB Re-download
#GLOBAL_EXPORT_FLAG += DIRTY_WORKAROUND_FOR_USBDOWNLOAD

