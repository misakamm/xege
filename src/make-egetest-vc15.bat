@echo off
call setenv

set COMPILER=


"%VC15PATH%\..\Common7\IDE\devenv.com" "egetest/graphicstest15.vcxproj" /Rebuild "Release|Win32"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2015 release
	goto end1
)

"%VC15PATH%\..\Common7\IDE\devenv.com" "egetest/graphicstest15.vcxproj" /Rebuild "Release|x64"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2015 release x64
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
:end2
pause
