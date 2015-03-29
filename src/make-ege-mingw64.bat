@echo off
rem 在setenv.bat中设置你的mingw路径，建议使用Dev-C++自带的mingw版本来编译，路径可以带空格或者中文，需要使用gbk编码
rem set MINGW64PATH=C:\Program Files (x86)\Dev-Cpp\MinGW64

set MINGWPATH=%MINGW64_PATH%
set MINGWDIR=%MINGW64_DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\x86_64-w64-mingw32\bin

set CCMODE64=0
set CCMODE32=0

REM 用64位模式编译。在makefile中使用编译参数 -m64 （如果不指定，则默认为 -m64）
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin64 clean
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin64 all
if %ErrorLevel% == 0 (
    mkdir ..\lib\%MINGWDIR%
    move /y libgraphics64.a ..\lib\%MINGWDIR%\
	set CCMODE64=成功
) else (
    set CCMODE64=失败 
	echo "64位库文件编译失败"
	pause
)

REM 用32位模式编译。在makefile中使用编译参数 -m32
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin clean
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin all
if %ErrorLevel% == 0 (
    mkdir ..\lib\%MINGWDIR%
    move /y libgraphics.a ..\lib\%MINGWDIR%\
    set CCMODE32=成功
) else (
    set CCMODE32=失败 
	echo "32位库文件编译失败"
    pause
)

echo "64位库编译%CCMODE64%。"
echo "32位库编译%CCMODE32%。"
echo "32位和64位库文件请到..\lib\%MINGWDIR%中查看"

pause

