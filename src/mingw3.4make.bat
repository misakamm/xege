call setenv
set path=%MINGW35PATH%\bin;%MINGW35PATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all
if not %ErrorLevel% == 0 (
    pause
) else (
    mkdir ..\lib\mingw3.4
    move /y libgraphics.a ..\lib\mingw3.4\
)

