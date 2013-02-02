call setenv
set path=%MINGW64PATH%\bin;

mingw32-make.exe -f MakefileWin64 clean
mingw32-make.exe -f MakefileWin64 all
if not %ErrorLevel% == 0 (
        pause
) else (
    mkdir ..\lib\mingw64
    move /y libgraphics64.a ..\lib\mingw64\
)


