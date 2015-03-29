@echo off
rem 在setenv.bat中设置你的mingw路径，建议使用Dev-C++自带的mingw版本来编译，路径可以带空格或者中文，需要使用gbk编码
rem set MINGW32PATH=D:\mingw\mingw4.7.1
rem set MINGW32PATH=C:\Program Files (x86)\Dev-Cpp\MinGW32


set MINGWPATH=%MINGW32_PATH%
set MINGWDIR=%MINGW32_DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all
if not %ErrorLevel% == 0 (
    echo "编译失败"
    pause
) else (
    mkdir ..\lib\%MINGWDIR%
    move /y libgraphics.a ..\lib\%MINGWDIR%\
echo "请到..\lib\%MINGWDIR%目录下找库文件libgraphics.a。"	
)

pause

