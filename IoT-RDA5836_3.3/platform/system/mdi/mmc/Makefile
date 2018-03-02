## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/system/mdi/mmc

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS :=

# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                    platform/chip/5857/regs \
					platform/service/bt \
					platform/service/bt/btadp \
					platform/system/mdi \
					platform/system/mdi/alg/common \
					platform/system/mdi/alg/isp \
					platform/system/mdi/alg/wma_decoder \
					platform/system/mdi/alg/amrring_codec_voc \
					platform/system/mdi/alg/audiojpeg_dec_voc \
					platform/system/mdi/alg/sbc_enc \
					platform/system/mdi/alg/audiojpegenc \
					platform/system/mdi/alg/preview\
					platform/system/mdi/alg/avi_dec \
					platform/system/mdi/alg/avi_enc \
					platform/system/mdi/alg/h263_dec \
					platform/system/mdi/alg/h264_dec \
					platform/system/mdi/alg/mpeg4_dec \
					platform/system/mdi/alg/rm_dec \
					platform/system/mdi/alg/ra8lbr_dec \
					platform/system/mdi/alg/aac_dec_c \
					platform/system/mdi/alg/mp3mad_dec_c \
					platform/system/mdi/alg/amr_dec_c \
					platform/system/mdi/alg/isomedia_demux \
					platform/system/mdi/alg/sbc_dec		\
					platform/system/mdi/alg/sbc_dec_voc \
                                        platform/system/mdi/alg/plc \
					platform/system/mdi/alg/effect_echo \
					platform/system/mdi/alg/sco_vpp\
					platform/system/mdi/alg/analogtv \
					platform/system/mdi/alg/pngdec \
					platform/system/mdi/alg/aec		\
					platform/system/mdi/ars      \
					at/ate     \
					at/utility  \
					platform/service \
					platform/edrv/lcdd \
					platform/chip/defs \
					platform/ \
					platform/rtos/std \
					platform/rtos/common \
					platform/rtos/sx \
					platform/system/calib \
					platform/chip/regs/$(CT_ASIC)\
					platform/chip/defs \
					platform/chip/hal \
					platform/chip/${CT_ASIC}/hal \
                    platform/chip/comn/hal \
                    platform/chip/comn\
					platform/chip/$(CT_ASIC)/regs\
					platform/edrv/aud \
					platform/edrv/aud/fm \
					platform/edrv/fmd \
					target \
					target/${CT_TARGET} \
					platform/edrv/camdold \
					platform/edrv/atvd \
					platform/edrv/pmd \
					platform/edrv/cammd \
					platform \
					platform/svc/uctls \
					platform/svc/umss/storage/ram      \
					platform/svc/umss      \
					platform/svc/umss/transport/boscsi \
					platform/svc/utraces	\
					platform/svc/ars         \
					platform/vpp/speech \
					platform/system/mdi/alg/noise_suppress \
					platform/chip/5857/regs \
                    apps/ap_main\
                    apps/system\
                    apps/ap_camera\
                    apps/ap_wifispeaker \
					apps/ap_btspeaker \
					platform/service/base  \
					platform/service/base/bal  \
					platform/service/bt \
					platform/service/bt/btadp \
					
LOCAL_ADD_INCLUDE := platform/system/mdi/alg/wma_decoder/src/

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := no


# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.

# Video player
ifeq "${VIDEO_PLAYER_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += VIDEO_PLAYER_SUPPORT
endif

# Analog TV
ifeq "${ANALOG_TV_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += ANALOG_TV_SUPPORT
endif

# Sound recorder
ifeq "${SOUND_RECORDER_SUPPORT}" "1"
LOCAL_EXPORT_FLAG  += SOUND_RECORDER_SUPPORT
endif

# Video recorder
ifeq "${VIDEO_RECORDER_SUPPORT}" "1"
LOCAL_EXPORT_FLAG  += VIDEO_RECORDER_SUPPORT
endif

# Camera motion detection
ifeq "${CAM_MD_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += CAM_MD_SUPPORT
endif

# MIDI format
ifeq "${MEDIA_MIDI_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += MEDIA_MIDI_SUPPORT
endif

# RM format
ifeq "${MEDIA_RM_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += MEDIA_RM_SUPPORT
endif

# H264 format
ifeq "${MEDIA_H264_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += MEDIA_H264_SUPPORT
endif

# DRA format
ifeq "${MEDIA_DRA_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += MEDIA_DRA_SUPPORT
endif

# voc vid dec
ifeq "${MEDIA_VOCVID_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += MEDIA_VOCVID_SUPPORT
endif

# FM line-in record
ifeq "${FM_LINEIN_RECORD}" "1"
LOCAL_EXPORT_FLAG += FM_LINEIN_RECORD
endif

LOCAL_EXPORT_FLAG += 

#CFLAGS += -isystem 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##

include src/make.srcs


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
