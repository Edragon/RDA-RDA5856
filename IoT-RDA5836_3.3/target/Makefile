## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := target

LOCAL_API_DEPENDS :=  platform                          \
                      target/${CT_TARGET}               \
                      platform/stack                    \
                      platform/vpp                      \
                      platform/rtos/common              \
                      platform/system/calib               \
                      platform/rtos/sx                  \
                      platform/rtos/std                 \
                      platform/chip/hal                 \
                      platform/chip/hal/${CT_ASIC}      \
                      platform/chip/defs                \
                      platform/chip/bcpu                \
                      platform/chip/pal                 \
                      platform/chip/${CT_ASIC}/hal/     \
					  platform/chip/${CT_ASIC}/boot/    \
                      platform/chip/comn                \
					  platform/chip/comn/hal            \
					  platform/chip/comn/boot           \
                      platform/edrv/pmd                 \
                      platform/edrv/pmd/${PM_MODEL}     \
                      platform/edrv/rfd                 \
                      platform/edrv/memd                \
                      platform/edrv/gpio                \
                      platform/service                      \
                      platform/service/base                 \
					  platform/service/bt					\
					  platform/service/bt/btadp				\
                      platform/service/dsm \
                      platform/edrv/memd/${FLSH_MODEL}  \
                      platform/edrv/mcd/mcd_${MCD_IF}   \
                      platform/edrv/lcdd                \
                      platform/edrv/lcdd/${LCD_MODEL}/${LCD_IF} \
                      platform/edrv/camdold             \
                      platform/edrv/camdold/${CAMERA_MODEL} \
                      platform/edrv/tsd                 \
                      platform/edrv/tsd/${TSD_MODEL}    \
                      platform/system/mdi/                     \
                      platform/service/usbdevice  \
                      platform/service/usbdevice/uctls  \
                      platform/service/usbdevice/storage  \
                      platform/service/usbdevice/storage/mc   \
                      platform/service/usbdevice/transport/boscsi   \
                      platform/edrv/aud                 \
                      platform/edrv/aud/codec_gallite   \
                      platform/edrv/aud/codec_wm8960   \
                      platform/edrv/aud/null            \
                      platform/edrv/aud/linein          \
                      platform/edrv/aud/fm              \
                      platform/edrv/aud/bt              \
                      platform/service/base/bal                         \

ifeq "${CSW_EXTENDED_API_CAMERA}" "1"
LOCAL_API_DEPENDS +=  platform/edrv/camd
endif

ifeq "${CSW_EXTENDED_API_AUDIO_VIDEO}" "1"
LOCAL_API_DEPENDS +=  platform/svc/avps
LOCAL_API_DEPENDS +=  platform/svc/mps
else
LOCAL_API_DEPENDS +=  platform/svc/aps
endif



#deprecated MCD_MODEL
ifneq (${MCD_MODEL},)
$(warning MCD_MODEL is deprecated, use MCD_IF instead)
endif # MCD_MODEL

#mcd as optional config:
ifneq (${MCD_IF},)
LOCAL_API_DEPENDS += platform/edrv/mcd                  \
                     platform/edrv/mcd/mcd_${MCD_IF}

LOCAL_EXPORT_FLAG += TGT_WITH_MCD TGT_WITH_MCD_IF_${MCD_IF}
endif # MCD_IF

#fm as optional config:
ifneq (${FM_MODEL},)
ifneq ($(findstring rda5888,${FM_MODEL}), )

LOCAL_API_DEPENDS += platform/edrv/fmd                  \
                                         platform/edrv/nfcd                  \
                                         platform/edrv/nfcd/pn544                  \
                     platform/edrv/fmd/rda5888

LOCAL_EXPORT_FLAG += TGT_WITH_FM TGT_WITH_FM_MODEL_${FM_MODEL}

else

LOCAL_API_DEPENDS += platform/edrv/fmd                  \
                     platform/edrv/fmd/${FM_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_FM TGT_WITH_FM_MODEL_${FM_MODEL}

endif

endif # FM_MODEL

#gsensor as optional config:
ifneq (${GSENSOR_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/gsensor                  \
                     platform/edrv/gsensor/${GSENSOR_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_GSENSOR TGT_WITH_GSENSOR_MODEL_${GSENSOR_MODEL}
endif # GSENSOR_MODEL

#gpiospi as optional config:
ifneq (${SPI_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/spigpio                  \
                     platform/edrv/spigpio/${SPI_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_GPIOSPI TGT_WITH_GPIOSPI_MODEL_${SPI_MODEL}
endif # SPI_MODEL



#gpioi2c as optional config:
ifneq (${I2C_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/i2cgpio                  \
                     platform/edrv/i2cgpio/${I2C_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_GPIOI2C TGT_WITH_GPIOI2C_MODEL_${I2C_MODEL}
endif # I2C_MODEL

#bt as optional config:
ifneq (${BT_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/btd
LOCAL_API_DEPENDS += platform/edrv/btd/${BT_MODEL}
LOCAL_API_DEPENDS += platform/edrv/btd/rdaDriver
LOCAL_API_DEPENDS += platform/edrv/btd/rdaCommon/rda_bt_lib

LOCAL_EXPORT_FLAG += TGT_WITH_BT
endif # BT_MODEL

#atv as optional config:
ifneq (${ATV_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/atvd

LOCAL_EXPORT_FLAG += TGT_WITH_ATV TGT_WITH_ATV_MODEL_${ATV_MODEL}
endif # ATV_MODEL

#ts as optional config:
ifneq (${TSD_MODEL},)
LOCAL_API_DEPENDS += platform/edrv/tsd                  \
                     platform/edrv/tsd/${TSD_MODEL}

LOCAL_EXPORT_FLAG += TGT_WITH_TS TGT_WITH_TS_MODEL_${TSD_MODEL}
endif # TSD_MODEL

LOCAL_MODULE_DEPENDS := ${TARGET_MODULE_DEPENDS}

# This is a top-level module
IS_TOP_LEVEL := yes

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= yes

# Ask target to register the extra modules.
ifeq "${TGT_REGISTER_PLATFORM_VERSIONS}" "1"
LOCAL_EXPORT_FLAG += TGT_REGISTER_PLATFORM_VERSIONS
endif #TGT_REGISTER_PLATFORM_VERSIONS

# Some USB flags.
#include ${SOFT_WORKDIR}/platform/service/usbdevice/uctls/uctls.def

LOCAL_EXPORT_FLAG += FLASH_BASE=${FLASH_BASE}

ifneq "${CT_PRODUCT}" ""
LOCAL_EXPORT_FLAG += CT_PRODUCT=${CT_PRODUCT}
# Always compile the file using CT_PRODUCT.
LOCAL_SRC_FORCE_BUILD := target_config.c
endif # CT_PRODUCT


# Add Module define
LOCAL_EXPORT_FLAG += CT_TARGET=${CT_TARGET}
LOCAL_EXPORT_FLAG += CT_ASIC=${CT_ASIC}
LOCAL_EXPORT_FLAG += CT_CHIP_DIE=${CT_CHIP_DIE}
LOCAL_EXPORT_FLAG += CT_CHIP_PKG=${CT_CHIP_PKG}
LOCAL_EXPORT_FLAG += PA_MODEL=${PA_MODEL}
LOCAL_EXPORT_FLAG += FLSH_MODEL=${FLSH_MODEL}
LOCAL_EXPORT_FLAG += FM_MODEL=${FM_MODEL}
LOCAL_EXPORT_FLAG += ATV_MODEL=${ATV_MODEL}

# For modules that support driver automatic selection
empty :=
space := $(empty) # end of the line
LOCAL_EXPORT_FLAG += LCD_MODEL=$(subst ${space},/,$(strip ${LCD_MODEL}))
LOCAL_EXPORT_FLAG += CAMERA_MODEL=$(subst ${space},/,$(strip ${CAMERA_MODEL}))
LOCAL_EXPORT_FLAG += BT_MODEL=$(subst ${space},/,$(strip ${BT_MODEL}))


## ------------------------------------------------------------------------- ##
##                         Source files                                      ##
## ------------------------------------------------------------------------- ##
include src/make.srcs

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

