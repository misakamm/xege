@echo off
rem ��setenv.bat���������mingw·��������ʹ��Dev-C++�Դ���mingw�汾�����룬·�����Դ��ո�������ģ���Ҫʹ��gbk����
rem set MINGW32PATH=D:\mingw\mingw4.7.1
rem set MINGW32PATH=C:\Program Files (x86)\Dev-Cpp\MinGW32


set MINGWPATH=%MINGW32_PATH%
set MINGWDIR=%MINGW32_DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all
if not %ErrorLevel% == 0 (
    echo "����ʧ��"
    pause
) else (
    mkdir ..\lib\%MINGWDIR%
    move /y libgraphics.a ..\lib\%MINGWDIR%\
echo "�뵽..\lib\%MINGWDIR%Ŀ¼���ҿ��ļ�libgraphics.a��"	
)

pause

