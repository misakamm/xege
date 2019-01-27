@rem 在以下一行设置你的mingw路径，建议使用code::blocks自带的mingw版本来编译，路径可以带空格或者中文，需要使用gbk编码
set MINGWPATH=C:\Dev-Cpp\MinGW64

set path=%MINGWPATH%\bin;%MINGWPATH%\x86_64-w64-mingw32\bin

mingw32-make.exe -f MakefileWin clean
mingw32-make.exe -f MakefileWin all

if not %ErrorLevel% == 0 (
    rem 编译失败
) else (
    rem 编译完成，请在本目录下找libgraphics.a文件
)

pause

