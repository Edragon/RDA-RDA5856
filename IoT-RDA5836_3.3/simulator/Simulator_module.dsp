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
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MMI_ON_WIN32" /D "CSD_KAL_MNT" /D "BUILD_DATE" /D "DAF_DECODE" /D "BT_SUPPORT" /D CT_ERES= /FR /YX /FD /GZ /Zm200 @"IncOption.txt" /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib Setupapi.lib libstack.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Bin/Simulator.exe" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=build resource files
PostBuild_Cmds=copy ..\apps\projects_cfg\project_name\resource\res.bin Bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Simulator - Win32 Release"
# Name "Simulator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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
# Begin Source File

SOURCE=.\keypad.h
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

SOURCE=.\SimulatorLayer\firmware\include\lcdd_m.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\pm.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\stdkey.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\sul.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\include\tm.h
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

SOURCE=.\SimulatorLayer\firmware\src\pm_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\sul_win32.c
# End Source File
# Begin Source File

SOURCE=.\SimulatorLayer\firmware\src\TM_win32.c
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

SOURCE=..\apps\system\src\device.c
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\device.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\include\globalconstants.h
# End Source File
# Begin Source File

SOURCE=..\apps\system\src\TaskInit.c
# End Source File
# End Group
# Begin Group "ap_bluetooth"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\apps\ap_bluetooth\src\ap_bluetooth.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_bluetooth\include\ap_bluetooth.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_bluetooth\src\ap_headset.c
# End Source File
# Begin Source File

SOURCE=..\apps\ap_bluetooth\include\ap_headset.h
# End Source File
# Begin Source File

SOURCE=..\apps\ap_bluetooth\src\ap_obex.c
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
