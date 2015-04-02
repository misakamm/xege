call setenv
set path=%MINGW48PATH%\bin;%MINGW48PATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all
if not %ErrorLevel% == 0 (
    pause
) else (
    mkdir ..\lib\mingw4.8.1\lib
    move /y libgraphics.a ..\lib\mingw4.8.1\lib\
)
