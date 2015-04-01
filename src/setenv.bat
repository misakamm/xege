@echo off
REM 微软自2005年之后用年份作为VS的发行代号，与其内部版本号不一致

REM  内部版本号     宏版本号         发行版本号
REM MS VC++ 12.0 _MSC_VER = 1800 (Visual C++ 2013)
REM MS VC++ 11.0 _MSC_VER = 1700 (Visual C++ 2012)
REM MS VC++ 10.0 _MSC_VER = 1600 (Visual C++ 2010)
REM MS VC++ 9.0  _MSC_VER = 1500 (Visual C++ 2008)
REM MS VC++ 8.0  _MSC_VER = 1400 (Visual C++ 2005)
REM MS VC++ 7.1  _MSC_VER = 1310 (Visual C++ 7.1)
REM MS VC++ 7.0  _MSC_VER = 1300 (Visual C++ 7.0)
REM MS VC++ 6.0  _MSC_VER = 1200 (Visual C++ 6.0)
REM MS VC++ 5.0  _MSC_VER = 1100 (Visual C++ 5.0)
echo.
echo.
echo ==========注意：如果你的Visual C++不是安装在C:\Program Files (x86)目录==========
echo ==========   ，请你先按Ctrl-C，然后用文本编辑器修改 setenv.bat 的内容  ==========
echo.
echo.

set VC10PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC
set VC12PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC
set VC13PATH=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
set VC15PATH=C:\Program Files (x86)\Microsoft Visual Studio 13.0\VC

rem MSBuild 不依赖于VC++的安装（不装VC++也能用MSBuild编译）
rem 对于VS2013 Express，只能使用MSBuild 2013执行命令行编译
rem 下载地址：http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs#DownloadFamilies_3
set MSBUILD13PATH=C:\Program Files (x86)\MSBuild\12.0
rem 其它版本的MSBuild(假如有的话)
set MSBUILD10PATH=C:\Program Files (x86)\MSBuild\10.0
set MSBUILD12PATH=C:\Program Files (x86)\MSBuild\11.0
set MSBUILD15PATH=C:\Program Files (x86)\MSBuild\13.0

echo.
echo.
echo ==========注意：如果你使用mingw（gcc/g++）编译，请你先按Ctrl-C退出批处理==========
echo ==========      然后用文本编辑器修改 setenv.bat 中关于mingw的内容      ==========
echo.
echo.
pause

rem set MINGWMAKEFILE=mingw
rem set MINGWPATH=C:\Dev-Cpp
set MINGW_BASE_PATH=D:\mingw
set MINGW32_472DIR=mingw-4.7.2
set MINGW32_472PATH=%MINGW_BASE_PATH%\%MINGW32_472DIR%
set MINGW32_481DIR=mingw-4.8.1
set MINGW32_481PATH=%MINGW_BASE_PATH%\%MINGW32_481DIR%
rem set MINGW64_492DIR=mingw64-4.9.2
rem set MINGW64_492PATH=%MINGW_BASE_PATH%\%MINGW64_492DIR%

REM 由于来自http://www.drangon.org/mingw/的mingw64-4.9.2无法编译32位库，故此放弃
REM 改为使用TDM发行的MinGW64
set MINGW64_492DIR=TDM-GCC-64
set MINGW64_492PATH=%MINGW_BASE_PATH%\%MINGW64_492DIR%

rem 我们使用Dev-C++中自带的MinGW。
rem 注意：你需要去看清楚Dev-Cpp安装的是哪个版本的MinGW！
set MINGW32_DIR=mingw32
set MINGW32PATH=C:\Program Files (x86)\Dev-Cpp\MinGW32
set MINGW64_DIR=mingw64
set MINGW64PATH=C:\Program Files (x86)\Dev-Cpp\MinGW64