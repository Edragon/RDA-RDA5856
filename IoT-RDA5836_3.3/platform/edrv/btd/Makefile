## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/edrv/btd

LOCAL_API_DEPENDS := target \
				platform \
				platform/base/sx \
				platform/chip/hal \
				platform/chip/${CT_ASIC}/hal \
                platform/chip/comn/hal \
                platform/chip/comn\
				platform/chip/${CT_ASIC}/defs \
				platform/edrv \
				platform/edrv/pmd \
				platform/edrv/btd/rdaDriver \
				platform/service \
				platform/service/bt \
				platform/service/bt/btadp \
				platform/service/base/system \
				platform/rtos/sx \
				platform/rtos/std \
				platform/rtos/common \
			    apps/gui \
			    apps/system \
			    apps/projects_cfg/${CT_PROJECT} \ 
				

ifneq ($(findstring rda,${BT_MODEL}),)
#LOCAL_MODULE_DEPENDS += platform/edrv/btd/BluetoothAdapter
LOCAL_MODULE_DEPENDS += platform/edrv/btd/rdaDriver
LOCAL_MODULE_DEPENDS += platform/edrv/btd/hcit
LOCAL_MODULE_DEPENDS += platform/edrv/btd/bttask
LOCAL_EXPORT_FLAG 	 += __BT_RDABT__
endif # rda

ifneq ($(findstring rda586,${BT_MODEL}),)
ifneq ($(findstring rda587,${BT_MODEL}),)
$(error You can NOT put rda586x and rda587x in BT_MODEL list at the same time!!!)
endif # rda587
endif # rda586

ifneq ($(findstring rda5875,${BT_MODEL}),)
ifneq ($(filter rdabt_8809,${BT_MODEL}),)
$(error You can NOT put rda5875 and rdabt_8809 in BT_MODEL list at the same time!!!)
endif # rda587
endif # rda586

ifneq ($(findstring rda586,${BT_MODEL}),)

	LOCAL_EXPORT_FLAG += __RDA_BT_5868_SERIES__
	
	ifneq ($(findstring rda5868e,${BT_MODEL}),)
		LOCAL_EXPORT_FLAG += __RDA_CHIP_R10_5868E__
	endif # rda5868e
	
endif # rda587x

ifneq ($(findstring rda587,${BT_MODEL}),)

	LOCAL_EXPORT_FLAG += __RDA_BT_587x_SERIES__

	ifneq ($(filter rda5870,${BT_MODEL}),)
		LOCAL_EXPORT_FLAG += __RDA_CHIP_R8_5870__
	endif # rda5870

	ifneq ($(filter rda5872,${BT_MODEL}),)
		LOCAL_EXPORT_FLAG += __RDA_CHIP_R10_5872__
	endif # rda5872

	ifneq ($(filter rda5872p,${BT_MODEL}),)
		LOCAL_EXPORT_FLAG += __RDA_CHIP_R11_5872P__
	endif # rda5872p

	ifneq ($(filter rda5872h,${BT_MODEL}),)
		LOCAL_EXPORT_FLAG += __RDA_CHIP_R10_5872H__
	endif # rda5872h

endif # rda587x

ifneq ($(findstring rda5875,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_BT_5875_SERIES__
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R11_5875__
endif # rda5872
ifneq ($(findstring rda5876,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_BT_5875_SERIES__
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R12_5876__
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R16_5876P__
endif # rda5876

ifneq ($(findstring rda599,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_BT_587x_SERIES__	
ifneq ($(findstring rda5990,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_BT_5875_SERIES__
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R12_5990__
endif # rda5990
endif #rda599x

ifneq ($(filter rdabt_8809,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_BT_587x_SERIES__	
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R17_8809__
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R18_8809E__
endif #rda599x
ifneq ($(filter rdabt_8809e,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_BT_587x_SERIES__	
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R17_8809__
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R18_8809E__
endif #rda599x

ifneq ($(filter rdabt_8809p,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R19_8809P__
endif

ifneq ($(filter rdabt_5855,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R20_5855__
endif

ifneq ($(filter rdabt_5856,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R21_5856__
endif

ifneq ($(filter rdabt_5856e,${BT_MODEL}),)
	LOCAL_EXPORT_FLAG += __RDA_CHIP_R22_5856E__
endif

ifeq ($(BT_SHARE_CRYSTAL),1)
	LOCAL_EXPORT_FLAG += __RDA_SHARE_CRYSTAL__
endif # BT_CRYSTAL_SHARED

# This is a top-level module
IS_TOP_LEVEL = yes
# Copy module lib to ./lib/${ASIC}
IS_MODULE_LIB := yes
# This needs to deliver environment related files also
DELIV_ENV_FILES := 0


# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Generates the CoolProfile headers automatically.
#AUTO_XCP2H ?= yes

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

empty :=
space := $(empty) # end of the line
# Compilation mode for the version string.
LOCAL_EXPORT_FLAG += BTD_BT_MODEL=$(subst ${space},/,$(strip ${BT_MODEL}))

# 32K clock connection
ifeq "${BT_USE_32K_CLK_PIN}" "1"
LOCAL_EXPORT_FLAG += BT_USE_32K_CLK_PIN
endif

# Break interrupt wakeup mode
ifeq "${BT_UART_BREAK_INT_WAKEUP}" "1"
LOCAL_EXPORT_FLAG += BT_UART_BREAK_INT_WAKEUP
endif

# Macro for BT stack build
LOCAL_EXPORT_FLAG += PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED=0 \
                     _REENTRANT \
                     COOLSAND

# Macros for different CT_RELEASE. Should be in env, put here temporarily
ifeq ("$(CT_RELEASE)", "debug")
LOCAL_EXPORT_FLAG += RDA_BT_DEBUG
LOCAL_EXPORT_FLAG += DEBUG=1
else
LOCAL_EXPORT_FLAG += DEBUG=0
endif

# Flags exported throughout the build
LOCAL_EXPORT_FLAG +=


## ------------------------------------------------------------------------- ##
##	                       Source files                                      ##
## ------------------------------------------------------------------------- ##
# No source files in the top-level itself...
S_SRC :=
C_SRC := btd_map.c
INPUT_FILES :=  

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk


