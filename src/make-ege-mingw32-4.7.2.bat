call setenv

set MINGWPATH=%MINGW32_472PATH%
set MINGWDIR=%MINGW32_472DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all

echo.
echo.

if not %ErrorLevel% == 0 (
    echo "编译失败"
) else (
    mkdir ..\lib\%MINGWDIR%
    move /y libgraphics.a ..\lib\%MINGWDIR%\
	echo.
	echo "请到..\lib\%MINGWDIR%目录下找库文件libgraphics.a"	
)

echo.
echo.
pause

