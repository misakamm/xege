@echo off
call setenv
set MINGWPATH=%MINGW32_481PATH%
set MINGWDIR=%MINGW32_481DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all

echo.
echo.

if not %ErrorLevel% == 0 (
    echo "����ʧ��"
) else (
    mkdir ..\lib\%MINGWDIR%
    move /y libgraphics.a ..\lib\%MINGWDIR%\
	echo .
	echo "�뵽..\lib\%MINGWDIR%Ŀ¼���ҿ��ļ�libgraphics.a��"	
)

echo.
echo.
pause

