# Microsoft Developer Studio Project File - Name="PixelToaster" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PixelToaster - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PixelToaster.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line.
!MESSAGE For example:
!MESSAGE 
!MESSAGE NMAKE /f "PixelToaster.mak" CFG="PixelToaster - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PixelToaster - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PixelToaster - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "" 
# PROP Scc_LocalPath "" 
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PixelToaster - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# PROP Target_Name "PixelToaster"
# PROP Target_Ext ".lib"
# PROP Linker_Output "$(OutDir)\PixelToaster.lib"
# PROP Precompiled_Header 0
# PROP Precompiled_Header_File "" 
# PROP Precompiled_Header_Path "" 
# PROP Enable_Profiling 0
# PROP Profile_Guided_Optimization 0
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W4 /EHsc /EHc /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "PLATFORM_WINDOWS" /fp:fast /Ob2 /GL /GA /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:$(OutDir)\PixelToaster.lib

!ELSEIF  "$(CFG)" == "PixelToaster - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# PROP Target_Name "PixelToaster"
# PROP Target_Ext ".lib"
# PROP Linker_Output "$(OutDir)\PixelToaster.lib"
# PROP Precompiled_Header 0
# PROP Precompiled_Header_File "" 
# PROP Precompiled_Header_Path "" 
# PROP Enable_Profiling 0
# PROP Profile_Guided_Optimization 0
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /EHsc /EHc /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "PLATFORM_WINDOWS" /fp:fast /Ob2 /GL /GA /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:$(OutDir)\PixelToaster.lib

!ENDIF 

# Begin Target

# Name "PixelToaster - Win32 Release"
# Name "PixelToaster - Win32 Debug"

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"

# Begin Source File

SOURCE=PixelToaster.cpp
# End Source File

# End Group

# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"

# Begin Source File

SOURCE=PixelToaster.h
# End Source File

# Begin Source File

SOURCE=PixelToasterCommon.h
# End Source File

# Begin Source File

SOURCE=PixelToasterConversion.h
# End Source File

# Begin Source File

SOURCE=PixelToasterWindows.h
# End Source File

# End Group

# End Target
# End Project