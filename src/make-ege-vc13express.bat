@echo off
call setenv
set COMPILER=MS Build 2013 (replace "VC2013 Express for Desktop")
set CCMODE=0

rem vs 2013 for desktop ��֧�������б��룬���ֻ�ܸ���msbuild
rem "%VC13PATH%\..\Common7\IDE\devenv.com" "vc2013/graphics13.sln" /Rebuild "Release|x64"
rem "%VC13PATH%\..\Common7\IDE\devenv.com" "vc2013/graphics13.sln" /Build "Release|Win32"
rem if not %ErrorLevel% == 0 (
rem        pause
rem        goto end
rem )

"%MSBUILD13PATH%\bin\msbuild.exe" "vc2013/graphics13.sln" /t:Build  /p:Configuration=Release /p:Platform=Win32 /p:VisualStudioVersion=12.0
if %ErrorLevel% == 0 (
	rem Success, Continue
) else (
	set CCMODE=Win32 Release
	goto endfail
)

"%MSBUILD13PATH%\bin\msbuild.exe" "vc2013/graphics13.sln" /t:Build  /p:Configuration=Release /p:Platform=x64 /p:VisualStudioVersion=12.0
if %ErrorLevel% == 0 (
	rem Success, Continue
) else (
	set CCMODE=x64 Release
	goto endfail
)


goto end


:endfail
echo "%COMPILER% ����ʧ��"
echo "ʧ�ܰ汾��%CCMODE%"
echo Ҫ��ȷִ�б����룬��Ҫ΢�� MSBuild 2013 ���ߵ�֧��
echo You need to download MSBuild tools from microsoft.
echo ����Ҫȥ΢��������ز���װ MSBuild ����
echo http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs#DownloadFamilies_3
:end

pause
