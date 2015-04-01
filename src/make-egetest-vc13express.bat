@echo off

call setenv

set COMPILER=MS Build 2013 (replace "VC2013 Express for Desktop")


"%MSBUILD13PATH%\bin\msbuild.exe" "egetest/graphicstest13.sln" /t:Clean

set CCMODE=
set FAILOR=
set SUCCESSOR=

set CCMODE=Release-Win32
"%MSBUILD13PATH%\bin\msbuild.exe" "egetest/graphicstest13.sln"  /t:Rebuild  /p:Configuration=Release /p:Platform=Win32 /p:VisualStudioVersion=12.0
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %CCMODE%
) else (
    set FAILOR=%FAILOR%  %CCMODE%
)

set CCMODE=Release-x64
"%MSBUILD13PATH%\bin\msbuild.exe" "egetest/graphicstest13.sln" /t:Rebuild  /p:Configuration=Release /p:Platform=x64 /p:VisualStudioVersion=12.0
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %CCMODE%
) else (
    set FAILOR=%FAILOR%  %CCMODE%
)
echo.
echo ----------------------------------------------
echo 要正确执行本编译，需要微软 MSBuild 2013 工具的支持
echo You need to download MSBuild tools from microsoft.
echo 你需要去微软官网下载并安装 MSBuild 工具
echo http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs#DownloadFamilies_3
echo.
echo 编译成功的项目有: %SUCCESSOR%
echo 编译失败的项目有: %FAILOR%
echo.
echo ----------------------------------------------
pause
