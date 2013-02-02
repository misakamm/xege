@echo off
call setenv

set COMPILER=

set path=%MINGW45PATH%\bin;%MINGW45PATH%\mingw32\bin
g++ -O3 "graphicstest/maintest.cpp" -L./ -s -lgraphics -lgdi32 -limm32 -lmsimg32 -lole32 -loleaut32 -lwinmm -luuid -mwindows --static -oz.exe
if %ErrorLevel% == 0 (
	echo MinGW32 compile SUCCESS
) else (
	set COMPILER=MinGW45
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
pause
:end2
