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
LOCAL_NAME := apps/ap_main

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"

LOCAL_MODULE_DEPENDS := 


# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                     ${MMI_CONFIG} \
                     ${MMI_CUSTOMER} \
                     ${MMI_CUSTOMER}/CustResource \
                     platform \
                     platform/service/base  \
                     platform/service/base/bal  \
                     at/ate \
                     at/utility \
                     platform/system/mdi \
                     platform/rtos/std \
                     platform/stack \
                     platform/rtos/sx \
                     platform/chip/defs \
                     platform/chip/hal \
                     platform/chip/comn \
                     platform/chip/comn/hal \
                     platform/chip/${CT_ASIC}/hal \
										 platform/chip/${CT_ASIC}/regs \
                     platform/service \
                     platform/edrv/pmd \
                     platform/edrv/fmd \
                     platform/svc/uctls \
                     platform/svc/umss      \
                     platform/svc/umss/storage/ram      \
                     platform/svc/umss/transport/boscsi \
                     platform/svc/utraces	\
                     platform/svc/uvideos	\
                     platform/edrv/pmd \
                     platform/edrv/gpio \
                     platform/service/bt/rdaStack \
                     platform/service/bt/rdaStack/stack \
                     platform/service/bt/btadp \
                     platform/service/iotapi \
                     platform/service/bt  \
                     platform/service/usbdevice  \
                     platform/service/usbdevice/storage  \
                     platform/system/mdi \
                     apps/gui \
                     apps/ap_music \
                     platform/service/gui/rgui    \
                     apps/ap_usbdisk \
                     apps/ap_idle \
                     apps/ap_linein \
                     apps/ap_charging \
                     apps/ap_record \
                     apps/ap_btspeaker\
                     apps/ap_setting \
                     apps/ap_fm \
                     apps/ap_calendar \
                     apps/ap_camera \
										 target \
										 platform/service/gui/rgui \
                     platform/edrv/tsd/  \
                     platform/edrv/tsd/${TSD_MODEL} \


GEN_REVISION_NUMBER := yes
VERSION_SRC := ap_common.c

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##

# Assembly / C code
#S_SRC := ${notdir ${wildcard *.S}}
#C_SRC := ${notdir ${wildcard *.c}}
S_SRC := ${notdir ${wildcard src/*.S}} # uncomment to take all .S files
C_SRC := ${notdir ${wildcard src/*.c}} # uncomment to take all .c files

# Added the src dir of the parent module to get the auto-generated version header file.
LOCAL_ADD_INCLUDE += application/src

# Simulation specific inputs
INPUT_FILES :=  

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##

include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


