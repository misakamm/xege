@echo off
call setenv

set MINGWPATH=%MINGW32_481PATH%
set MINGWDIR=%MINGW32_481DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\mingw32\bin

set COMPILER=0


g++ -O3 "egetest/maintest.cpp" -L../lib/%MINGWDIR% -s -lgraphics -lgdi32 -limm32 -lmsimg32 -lole32 -loleaut32 -lwinmm -luuid -mwindows -m32 --static -oz.exe
if %ErrorLevel% == 0 (
	echo %MINGWDIR% compile SUCCESS
) else (
	set COMPILER=%MINGWDIR%
	goto end1
)

goto end2
:end1
echo "%COMPILER% 编译失败"
:end2
echo 现在可以运行z.exe
pause
