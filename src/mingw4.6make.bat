call setenv
set path=%MINGW46PATH%\bin;%MINGW46PATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all
if not %ErrorLevel% == 0 (
    pause
) else (
    mkdir ..\lib\mingw4.6
    move /y libgraphics.a ..\lib\mingw4.6\
)

