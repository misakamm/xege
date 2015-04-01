@echo off
call setenv

set COMPILER=


"%VC10PATH%\..\Common7\IDE\devenv.com" "egetest/graphicstest10.vcxproj" /Rebuild "Release|Win32"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2010 release
	goto end1
)

"%VC10PATH%\..\Common7\IDE\devenv.com" "egetest/graphicstest10.vcxproj" /Rebuild "Release|x64"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2010 release x64
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
:end2
pause
