@echo off
call setenv

set MINGWPATH=%MINGW64_492PATH%
set MINGWDIR=%MINGW64_492DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\x86_64-w64-mingw32\bin

set COMPILER=

g++ -O3 "egetest/maintest.cpp" -I../include -L./ -L../lib/%MINGWDIR% -s -lgraphics64 -lgdi32 -limm32 -lmsimg32 -lole32 -loleaut32 -lwinmm -luuid -mwindows -m64 --static -oz64.exe
if %ErrorLevel% == 0 (
	echo %MINGWDIR% compile SUCCESS
) else (
	set COMPILER=%MINGWDIR%
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
:end2
pause
