
## target is customer' base module
##
## Other customer modules or thirdpart libs should also add here. 
## 

ifeq ("${TGT_MOD_APP_BTSPEAKER}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_btspeaker
endif 

ifeq ("${TGT_MOD_APP_CALENDAR}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_calendar
endif 

ifeq ("${TGT_MOD_APP_BTDONGLE}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_btdongle
endif 

ifeq ("${TGT_MOD_APP_CAMERA}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_camera
endif 

ifeq ("${TGT_MOD_APP_CHARGING}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_charging
endif   
 
ifeq ("${TGT_MOD_APP_DOWNLOAD}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_wifispeaker
endif   
 
ifeq ("${TGT_MOD_APP_FM}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_fm
endif  
  
ifeq ("${TGT_MOD_APP_IME}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_ime
endif  
  
ifeq ("${TGT_MOD_APP_LINEIN}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_linein
endif  
 
ifeq ("${TGT_MOD_APP_MUSIC}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_music
endif 
  
ifeq ("${TGT_MOD_APP_RECORD}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_record
endif  

ifeq ("${TGT_MOD_APP_SETTING}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_setting
endif 

ifeq ("${TGT_MOD_APP_STORAGE}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_storage
endif  

ifeq ("${TGT_MOD_APP_USBDISK}"  , "src")
LOCAL_MODULE_DEPENDS += apps/ap_usbdisk
endif 
   
## The module alwyas compile from source code
LOCAL_MODULE_DEPENDS += target



