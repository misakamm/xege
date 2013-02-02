call setenv
set path=%MINGW47PATH%\bin;%MINGW47PATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all
if not %ErrorLevel% == 0 (
    pause
) else (
    mkdir ..\lib\mingw4.7
    move /y libgraphics.a ..\lib\mingw4.7\
)

