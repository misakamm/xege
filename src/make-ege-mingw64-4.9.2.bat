@echo off
call setenv

set MINGWPATH=%MINGW64_492PATH%
set MINGWDIR=%MINGW64_492DIR%

set path=%MINGWPATH%\bin;%MINGWPATH%\x86_64-w64-mingw32\bin

set CCMODE64=0
set CCMODE32=0

REM ��64λģʽ���롣��makefile��ʹ�ñ������ -m64 �������ָ������Ĭ��Ϊ -m64��
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin64 clean
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin64 all
if %ErrorLevel% == 0 (
    mkdir ..\lib\%MINGWDIR%
    move /y libgraphics64.a ..\lib\%MINGWDIR%\
	set CCMODE64=�ɹ�
) else (
    set CCMODE64=ʧ�� 
)

REM ��32λģʽ���롣��makefile��ʹ�ñ������ -m32
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin clean
mingw32-make.exe -f %MINGWMAKEFILE%MakefileWin all
if %ErrorLevel% == 0 (
    mkdir ..\lib\%MINGWDIR%
	echo.
    move /y libgraphics.a ..\lib\%MINGWDIR%\
    set CCMODE32=�ɹ�
) else (
    set CCMODE32=ʧ�� 
)
echo.
echo.
echo 64λ�����%CCMODE64%
echo 32λ�����%CCMODE32%
echo 32λ��64λ���ļ��뵽..\lib\%MINGWDIR%�в鿴
echo.
echo.
pause
