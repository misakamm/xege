@echo off
call setenv

set COMPILER=

set path=%MINGW64PATH%\bin;
g++ -O3 "graphicstest/maintest.cpp" -L./ -s -lgraphics64 -lgdi32 -limm32 -lmsimg32 -lole32 -loleaut32 -lwinmm -luuid -mwindows --static -oz64.exe
if %ErrorLevel% == 0 (
	echo MinGW64 compile SUCCESS
) else (
	set COMPILER=MinGW64
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
pause
:end2
