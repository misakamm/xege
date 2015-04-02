# Microsoft Developer Studio Project File - Name="graphics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=graphics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "graphics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "graphics.mak" CFG="graphics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "graphics - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "graphics - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "graphics - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\Include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=postbuild.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "graphics - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/vc6/lib/graphics.lib"

!ENDIF 

# Begin Target

# Name "graphics - Win32 Release"
# Name "graphics - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\lpng\adler32.c
# End Source File
# Begin Source File

SOURCE=..\color.cpp
# End Source File
# Begin Source File

SOURCE=..\lpng\compress.c
# End Source File
# Begin Source File

SOURCE=..\lpng\crc32.c
# End Source File
# Begin Source File

SOURCE=..\lpng\deflate.c
# End Source File
# Begin Source File

SOURCE=..\egecontrolbase.cpp
# End Source File
# Begin Source File

SOURCE=..\egegapi.cpp
# End Source File
# Begin Source File

SOURCE=..\graphics.cpp
# End Source File
# Begin Source File

SOURCE=..\lpng\gzclose.c
# End Source File
# Begin Source File

SOURCE=..\lpng\gzlib.c
# End Source File
# Begin Source File

SOURCE=..\lpng\gzread.c
# End Source File
# Begin Source File

SOURCE=..\lpng\gzwrite.c
# End Source File
# Begin Source File

SOURCE=..\image.cpp
# End Source File
# Begin Source File

SOURCE=..\lpng\infback.c
# End Source File
# Begin Source File

SOURCE=..\lpng\inffast.c
# End Source File
# Begin Source File

SOURCE=..\lpng\inflate.c
# End Source File
# Begin Source File

SOURCE=..\lpng\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\logo.cpp
# End Source File
# Begin Source File

SOURCE=..\math.cpp
# End Source File
# Begin Source File

SOURCE=..\music.cpp
# End Source File
# Begin Source File

SOURCE=..\lpng\png.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngerror.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngget.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngread.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngset.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\lpng\pngwutil.c
# End Source File
# Begin Source File

SOURCE=..\random.cpp
# End Source File
# Begin Source File

SOURCE=..\lpng\trees.c
# End Source File
# Begin Source File

SOURCE=..\lpng\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\lpng\zutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\lpng\crc32.h
# End Source File
# Begin Source File

SOURCE=..\lpng\deflate.h
# End Source File
# Begin Source File

SOURCE=..\ege.h
# End Source File
# Begin Source File

SOURCE=..\ege_head.h
# End Source File
# Begin Source File

SOURCE=..\graphics.h
# End Source File
# Begin Source File

SOURCE=..\lpng\gzguts.h
# End Source File
# Begin Source File

SOURCE=..\lpng\inffast.h
# End Source File
# Begin Source File

SOURCE=..\lpng\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\lpng\inflate.h
# End Source File
# Begin Source File

SOURCE=..\lpng\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\lpng\png.h
# End Source File
# Begin Source File

SOURCE=..\lpng\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\lpng\pngdebug.h
# End Source File
# Begin Source File

SOURCE=..\lpng\pnginfo.h
# End Source File
# Begin Source File

SOURCE=..\lpng\pnglibconf.h
# End Source File
# Begin Source File

SOURCE=..\lpng\pngpriv.h
# End Source File
# Begin Source File

SOURCE=..\lpng\pngstruct.h
# End Source File
# Begin Source File

SOURCE=..\lpng\trees.h
# End Source File
# Begin Source File

SOURCE=..\lpng\zconf.h
# End Source File
# Begin Source File

SOURCE=..\lpng\zlib.h
# End Source File
# Begin Source File

SOURCE=..\lpng\zutil.h
# End Source File
# End Group
# End Target
# End Project
