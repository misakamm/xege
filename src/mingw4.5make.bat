call setenv
set path=%MINGW45PATH%\bin;%MINGW45PATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all
if not %ErrorLevel% == 0 (
    pause
) else (
    mkdir ..\lib\mingw4.5
    move /y libgraphics.a ..\lib\mingw4.5\
)

