## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := apps

# List of submodules which contain code we need to include in the final lib
LOCAL_MODULE_DEPENDS := \
                        #apps/ap_main \
                        #apps/ap_bluetooth \
                        #apps/ap_music \
						# system moved to ./platform/service/base/
                        #apps/system \
                        #apps/ap_usbdisk \
                        #apps/ap_idle\
                        #apps/ap_charging \

                        
LOCAL_MODULE_DEPENDS += $(foreach thisapp, ${TGT_MOD_APP_LIST}, apps/${thisapp})


#Wifi radio box module
ifeq "${WIFI_RADIO_BOX_SUPPORT}" "YES"
#LOCAL_MODULE_DEPENDS+=apps/ap_wifispeaker
endif

#Ipcamera module
ifneq "${CAMERA_MODEL}" ""
#LOCAL_MODULE_DEPENDS+=apps/ap_camera
endif

# This is a top-level module
IS_TOP_LEVEL := yes

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

