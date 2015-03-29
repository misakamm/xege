@echo off
call setenv

REM 注意，不能使用 /Rebuild，而应该使用/Build。如果使用前者，则Win32的库会被VC自动删掉

REM 以下三个变量用于确定编译环境
REM VC的安装路径
set VCPATH=%VC13PATH%
REM ege的vc项目的目录
set PROJECT_DIR=vc2013
REM ege的vc项目的名称
set PROJECT_NAME=graphics13.sln

REM 一下三个变量用于记录编译时的状态
set CCMODE=
set FAILOR=
set SUCCESSOR=

REM 开始编译
set CCMODE=Release-Win32
"%VCPATH%\..\Common7\IDE\devenv.com" "%PROJECT_DIR%/%PROJECT_NAME%" /Build "Release|Win32"
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %CCMODE%
) else (
    set FAILOR=%FAILOR%  %CCMODE%
)

set CCMODE=Release-x64
"%VCPATH%\..\Common7\IDE\devenv.com" "%PROJECT_DIR%/%PROJECT_NAME%" /Build "Release|x64"
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %CCMODE%
) else (
    set FAILOR=%FAILOR%  %CCMODE%
)



echo.
echo ----------------------------------------------
echo 编译%PROJECT_NAME%
echo 编译成功的项目有: %SUCCESSOR%
echo 编译失败的项目有: %FAILOR%
echo.
echo ----------------------------------------------
pause