# Microsoft Developer Studio Project File - Name="Simulator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Simulator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Simulator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Simulator.mak" CFG="Simulator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simulator - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Simulator - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Simulator - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Bin/Simulator.exe"

!ELSEIF  "$(CFG)" == "Simulator - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MMI_ON_WIN32" /D "CSD_KAL_MNT" /D "BUILD_DATE" /D "BUILD_TIME" /D "DAF_DECODE" /D "BT_SUPPORT" /D "CFW_TCPIP_SUPPORT" /D "__SLIMMMI_TCPIP__" /D "WIFI_SUPPORT" /D "_WINSOCKAPI_" /D "WPCAP" /D "__AT_SUPPORT__" /D CT_ERES= /D AUD_MAX_LEVEL=15 /D SUPPORT_SOCKET_CUST_DEFINE_NUM=32 /D "__WIFI_RADIO_BOX__" /D "IP_CAMERA_SUPPORT" /D "CAMERA_SUPPORT" /FR /YX /FD /GZ /Zm200 @"IncOption.txt" /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib Setupapi.lib libstack.lib GBMMI.lib gb_lt.lib gb_sc.lib _gbapi_.lib wpcap.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ws2_32.lib Iphlpapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Bin/Simulator.exe" /pdbtype:sept /libpath:"..\lib\gb\lib" /libpath:"..\lib" /libpath:"lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=copy ..\platform\csw\include\event.h SimulatorLayer\firmware\include\event.h
PostBuild_Desc=build resource files
PostBuild_Cmds=copy ..\apps\projects_cfg\128_64\resource\res.bin Bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Simulator - Win32 Release"
# Name "Simulator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ATDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawLed8s.cpp
# End Source File
# Begin Source File

SOURCE=.\keypad.cpp
# End Source File
# Begin Source File

SOURCE=.\screen.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\simdevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator.rc
# End Source File
# Begin Source File

SOURCE=.\SimulatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TraceModule.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CoolSimulator.ico
# End Source File
# Begin Source File

SOURCE=.\res\Simulator.ico
# End Source File
# Begin Source File

SOURCE=.\res\Simulator.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ATDlg.h
# End Source File
# Begin Source File

SOURCE=.\keypad.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\lwipopts.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\screen.h
# End Source File
# Begin Source File

SOURCE=.\ScreenDlg.h
# End Source File
# Begin Source File

SOURCE=.\simdevice.h
# End Source File
# Begin Source File

SOURCE=.\Simulator.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorStart.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TraceModule.h
# End Source File
# End Group
# Begin Group "SimulatorLayer"

# PROP Default_Filter ""
# Begin Group "firmware"

# PROP Default_Filter ""
# Begin Group "firmwareInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\base_prv.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\bt_drv.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\cfw.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\cmn_defs.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\cos.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\cp_csw.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\cs_types.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\csw.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\csw_mem_prv.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\csw_shell.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\cswtype.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\dbg.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\dm.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\drv.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\errorcode.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\event.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\fmd_m.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\fs.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\hal_host.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\hal_uart.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\lcdd_m.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\pm.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\include\sockets.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\stdkey.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\sul.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\sxs_io.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\tm.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\ts.h
# End Source File
# End Group
# Begin Group "firmwareSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\cos_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\dm_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\drv_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\fmd_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\FS_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\Mci_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\pm_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\sul_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\sxr_os.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\TM_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\uart_win32.c
# End Source File
# End Group
# End Group
# Begin Group "MCI"

# PROP Default_Filter ""
# Begin Group "mciInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mciinc\ap_audio.h
# End Source File
# Begin Source File

SOURCE=..\platform\mdi\include\cpu_share.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mciInc\mci.h
# End Source File
# End Group
# Begin Group "mciSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\aac_info.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\amr_info.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\ap_audio.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\ap_id3_parser.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\daf_info.c
# End Source File
# Begin Source File

SOURCE=..\platform\mdi\mmc\src\mmc_audiocommon.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\simulator_audio_mci.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\vm_info.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\wav_info.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\mci\mcisrc\wma_info.c
# End Source File
# End Group
# End Group
# Begin Group "BTAdp"

# PROP Default_Filter ""
# Begin Group "btinc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimulatorLayer\bluetooth\include\hcit_win32_usb.h
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\rdaStack\include\rdabt_hcit.h
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\rdaStack\include\rdabt_main.h
# End Source File
# End Group
# Begin Group "btsrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimulatorLayer\bluetooth\src\hcit_win32_usb.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\bluetooth\src\papi_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\bluetooth\src\rdabt_main.c
# End Source File
# End Group
# End Group
# Begin Group "WifiAdp"

# PROP Default_Filter ""
# Begin Group "wifiInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\csw\wifi\include\wifi_common.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\wifi\include\wifi_config.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\wifi\include\wifi_os.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\wifi\include\winpcap_for_wifi.h
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\wifi\rdawifi\include\wlan_sdio.h
# End Source File
# End Group
# Begin Group "wifisrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\csw\wifi\src\wifi_task.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\wifi\src\winpcap_for_wifi.c
# End Source File
# End Group
# End Group
# End Group
# Begin Group "mmi_customer"

# PROP Default_Filter ""
# Begin Group "mmi_customerInclude"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\target_res\t800_6432\mmi_customer\include\custresdef.h
# End Source File
# Begin Source File

SOURCE=..\target_res\t800_6432\mmi_customer\include\gui_setting.h
# End Source File
# Begin Source File

SOURCE=..\target_res\t800_6432\mmi_customer\include\populateres.h
# End Source File
# Begin Source File

SOURCE=..\target_res\t800_6432\mmi_customer\include\resource.h
# End Source File
# End Group
# End Group
# Begin Group "apps"

# PROP Default_Filter ""
# Begin Group "System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\system\src\Aud_main.c
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\Aud_main.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\src\cam_main.c
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\cam_main.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\src\device.c
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\device.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\globalconstants.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\src\History.c
# End Source File
# Begin Source File

SOURCE=..\apps\system\src\MultimediaTask.c
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\MultimediaTask.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\src\TaskInit.c
# End Source File
# End Group
# Begin Group "ap_calendar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_calendar\src\ap_calendar.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_calendar\include\ap_calendar.h
# End Source File
# End Group
# Begin Group "ap_gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_common.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_delete.c

!IF  "$(CFG)" == "Simulator - Win32 Release"

!ELSEIF  "$(CFG)" == "Simulator - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_dialog.c

!IF  "$(CFG)" == "Simulator - Win32 Release"

!ELSEIF  "$(CFG)" == "Simulator - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_directory.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_input.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_list.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_menu.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_playlist.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_progress.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_scroll.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_searchlist.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_gui\src\gui_slider.c
# End Source File
# End Group
# Begin Group "ap_main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_main\src\ap_common.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\include\ap_common.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\src\ap_fselector.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\include\ap_gui.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\src\ap_media.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\include\ap_media.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\src\ap_message.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\include\ap_message.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\src\ap_playlist.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\src\ap_testmode.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\src\enhanced_oggtag.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\src\MainTask.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_main\include\MainTask.h
# End Source File
# End Group
# Begin Group "ap_music"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_music\src\ap_music.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_music\include\ap_music.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_music\src\ap_music_lyric.c
# End Source File
# End Group
# Begin Group "ap_fm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_fm\src\ap_fm.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_fm\include\ap_fm.h
# End Source File
# End Group
# Begin Group "ap_record"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_record\src\ap_record.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_record\include\ap_record.h
# End Source File
# End Group
# Begin Group "ap_setting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_setting\src\ap_setting.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_setting\include\ap_setting.h
# End Source File
# End Group
# Begin Group "ap_idle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_idle\src\ap_idle.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_idle\include\ap_idle.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_idle\src\ap_timer.c
# End Source File
# End Group
# Begin Group "ap_usbdisk"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_usbdisk\src\ap_usbdisk.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_usbdisk\include\ap_usbdisk.h
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Group "gui_lcd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\gui\gui_lcd\src\gui_lcd_display.c
# End Source File
# End Group
# Begin Group "gui_led"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\gui\gui_led8s\src\gui_led8s_display.c
# End Source File
# End Group
# Begin Group "GuiInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\gui\include\gui.h
# End Source File
# Begin Source File

SOURCE=..\apps\gui\include\gui_const.h
# End Source File
# Begin Source File

SOURCE=..\apps\gui\include\gui_lcd_display.h
# End Source File
# Begin Source File

SOURCE=..\apps\gui\include\gui_led8s_display.h
# End Source File
# Begin Source File

SOURCE=..\apps\gui\include\gui_resource.h
# End Source File
# End Group
# Begin Group "GuiSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\gui\src\gui_resource.c
# End Source File
# End Group
# Begin Group "gui_rgblcd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\gui\gui_rgblcd\src\gui_rgblcd_display.c
# End Source File
# End Group
# End Group
# Begin Group "ap_linein"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_linein\src\ap_linein.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_linein\include\ap_linein.h
# End Source File
# End Group
# Begin Group "ap_charging"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_charging\src\ap_charging.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_charging\include\ap_charging.h
# End Source File
# End Group
# Begin Group "ap_ime"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_ime\include\ConfigureBusiness.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\ConfigureManager.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\ConfigureRelation.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\Engine.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\EngineConfig.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\gbapi.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\gbApp.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\gbdef.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\gbim.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\gbtype.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\gbwchar.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_configItem.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_configList.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_ConfigureBusiness.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_ConfigureRelation.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_Engine.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_EngineConfig.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_gb_en.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_gb_sc.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_GBEngineTest_V5.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_Misc.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_ProduceBusiness.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_produceItem.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_produceItem_develop.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\src\ime_uni2gbk.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\Misc.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\ProduceBusiness.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_ime\include\ProduceManager.h
# End Source File
# End Group
# Begin Group "rdabt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\src\rdabt_common.c
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\include\rdabt_common.h
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\src\rdabt_manager.c
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\include\rdabt_manager.h
# End Source File
# End Group
# Begin Group "ap_bt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_bluetooth\src\ap_bt_common.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_bluetooth\src\ap_hfp.c
# End Source File
# End Group
# Begin Group "at"

# PROP Default_Filter ""
# Begin Group "ate"

# PROP Default_Filter ""
# Begin Group "ateInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\at\ate\include\at_common.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\include\at_define.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\include\at_errcode.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\include\at_module.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\include\at_parser.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\include\at_sa.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\include\at_uart.h
# End Source File
# End Group
# Begin Group "ateSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\at\ate\src\at_cmdlist.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\src\at_command_table.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\src\at_common.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\src\at_main.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\src\at_module.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\src\at_parser.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\src\at_sa.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ate\src\at_uart.c
# End Source File
# End Group
# End Group
# Begin Group "ats"

# PROP Default_Filter ""
# Begin Group "at_wifi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\at\ats\wifi\src\at_cmd_wifi.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ats\wifi\include\at_cmd_wifi.h
# End Source File
# End Group
# Begin Group "at_tcpip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\at\ats\tcpip\src\at_cmd_tcpip.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\ats\tcpip\include\at_cmd_tcpip.h
# End Source File
# End Group
# End Group
# Begin Group "utility"

# PROP Default_Filter ""
# Begin Group "utilInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\at\utility\include\at_cp.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\utility\include\at_trace.h
# End Source File
# Begin Source File

SOURCE=..\apps\at\utility\include\at_utility.h
# End Source File
# End Group
# Begin Group "utilSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\at\utility\src\at_trace.c
# End Source File
# Begin Source File

SOURCE=..\apps\at\utility\src\at_utility.c
# End Source File
# End Group
# End Group
# End Group
# Begin Group "ap_wifispeaker"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_wifispeaker\src\DownLoadMusic.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_wifispeaker\include\downloadmusic.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_wifispeaker\src\PlayMedia.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_wifispeaker\include\PlayMedia.h
# End Source File
# End Group
# Begin Group "ap_camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_camera\src\ap_camera.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_camera\include\ap_camera.h
# End Source File
# End Group
# End Group
# Begin Group "BluetoothAdapter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\src\rdabt_a2dp.c
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\include\rdabt_a2dp.h
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\include\rdabt_a2dp.h
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\src\rdabt_hfp.c
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\include\rdabt_hfp.h
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\src\rdabt_spp.c
# End Source File
# Begin Source File

SOURCE=..\platform\edrv\btd\BluetoothAdapter\include\rdabt_spp.h
# End Source File
# End Group
# Begin Group "TcpipLayers"

# PROP Default_Filter ""
# Begin Group "Adapters"

# PROP Default_Filter ""
# Begin Group "AdaptersInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\system\include\adp_events.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\soc_api.h
# End Source File
# End Group
# Begin Group "AdaptersSrc"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "CfwAps"

# PROP Default_Filter ""
# Begin Group "CfwApsInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\csw\cfw\aps\include\aps.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\aps\include\csw_ppp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\aps\include\ppp_cp.h
# End Source File
# End Group
# Begin Group "CfwApsSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\csw\cfw\aps\src\aps_main.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\aps\src\aps_tcpip_main.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\aps\src\csw_ppp.c
# End Source File
# End Group
# End Group
# Begin Group "Tcpip"

# PROP Default_Filter ""
# Begin Group "TcpipInc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_api.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_api_msg.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_arch.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_arch_cc.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_arch_perf.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_arch_sys.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_autoip.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_debug.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_def.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_dhcp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_dns.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_err.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_etharp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_icmp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_icmp6.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_igmp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_inet.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_inet6.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_inet_chksum.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_init.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_ip.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_ip6.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_ip6_addr.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_ip_addr.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_ip_frag.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_mem.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_memp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_memp_std.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_netbuf.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_netdb.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_netif.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_netifapi.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_opt.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_pbuf.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_ppp_oe.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_raw.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_sio.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_slipif.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_snmp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_snmp_asn1.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_snmp_msg.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_snmp_structs.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_sockets.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_stats.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_sys.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_tcp.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_tcp_impl.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_timers.h
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\include\tcpip_udp.h
# End Source File
# End Group
# Begin Group "TcpipSrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_api_lib.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_api_msg.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_arch_sys.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_asn1_dec.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_asn1_enc.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_autoip.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_def.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_dhcp.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_dns.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_err.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_etharp.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_ethernetif.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_icmp.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_icmp6.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_igmp.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_inet.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_inet6.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_inet_chksum.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_init.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_ip.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_ip6.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_ip6_addr.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_ip_addr.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_ip_frag.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_mem.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_memp.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_mib2.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_mib_structs.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_msg_in.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_msg_out.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_netbuf.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_netdb.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_netif.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_netifapi.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_pbuf.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_raw.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_sockets.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_stats.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_sys.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_tcp.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_tcp_in.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_tcp_out.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_timers.c
# End Source File
# Begin Source File

SOURCE=..\platform\csw\cfw\tcpip\src\tcpip_udp.c
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Simulator : {33A1B74A-EA53-4D59-B5C6-7E87200DD475}
# 	2:5:Class:CBitmapInfoHeader
# 	2:10:HeaderFile:bitmapinfoheader.h
# 	2:8:ImplFile:bitmapinfoheader.cpp
# End Section
