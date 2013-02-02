@echo off
call setenv

set COMPILER=


"%VC12PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest12.vcxproj" /Rebuild "Debug|Win32"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2012 debug
	goto end1
)
"%VC12PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest12.vcxproj" /Rebuild "Release|Win32"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2012 release
	goto end1
)
"%VC12PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest12.vcxproj" /Rebuild "Debug|x64"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2012 debug x64
	goto end1
)
"%VC12PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest12.vcxproj" /Rebuild "Release|x64"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2012 release x64
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
pause
:end2
