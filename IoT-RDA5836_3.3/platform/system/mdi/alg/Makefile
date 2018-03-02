## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##

include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := platform/system/mdi/alg

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"
LOCAL_MODULE_DEPENDS := \
						platform/system/mdi/alg/wma_decoder \
						platform/system/mdi/alg/audiojpeg_dec_voc \
						platform/system/mdi/alg/sbc_dec_voc	\
						platform/system/mdi/alg/sbc_enc \
						platform/system/mdi/alg/sco_vpp	\
						platform/system/mdi/alg/ecc_voc	\
						platform/system/mdi/alg/noise_suppress \
						platform/system/mdi/alg/cmath \
						platform/system/mdi/alg/analog_audio_iir \
            platform/system/mdi/alg/plc \
            platform/system/mdi/alg/effect_echo \
						platform/system/mdi/alg/audiojpegenc \
						
#						platform/system/mdi/alg/sbc_dec	\
						platform/system/mdi/alg/avi_enc \
						platform/system/mdi/alg/preview \
						platform/system/mdi/alg/common \
						platform/system/mdi/alg/pngdec \

ifeq "${VIDEO_PLAYER_SUPPORT}" "1"

LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/avi_dec \
						platform/system/mdi/alg/h263_dec \
						platform/system/mdi/alg/mpeg4_dec \
						platform/system/mdi/alg/isomedia_demux

ifeq "${MEDIA_RM_SUPPORT}" "1" 						
LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/rm_dec \
						platform/system/mdi/alg/ra8lbr_dec
endif

ifeq "${MEDIA_H264_SUPPORT}" "1" 						
LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/h264_dec
endif
ifeq "${MEDIA_DRA_SUPPORT}" "1" 						
LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/dra_dec
LOCAL_BINARY_LIBS    += platform/system/mdi/alg/dra_dec
endif

ifeq "${MEDIA_VOCVID_SUPPORT}" "1" 						
LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/aac_dec_c
LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/mp3mad_dec_c
LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/amr_dec_c
endif

endif
						


# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
#						platform/system/mdi \


ifeq "${ANALOG_TV_SUPPORT}" "1"
LOCAL_MODULE_DEPENDS += platform/system/mdi/alg/analogtv
endif
					

# Set this to a non-null string to signal a toplevel module, like 
# phy but not like phy/kitchensink. This defines the behavior of 
# make deliv
IS_TOP_LEVEL := yes


# This can be used to define some preprocessor variables to be used in 
# the current module, but also exported to all dependencies.
# This is especially useful in an ENTRY_POINT modules
# Ex. : LOCAL_EXPORT_FLAGS += OS_USED DEBUG will result in 
# -DOS_USED -DDEBUG being passed on each subsequent compile command.
LOCAL_EXPORT_FLAG += 




## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
# Assembly / C code
S_SRC :=
C_SRC := 

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk




