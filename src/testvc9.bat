@echo off
call setenv

set COMPILER=

"%VC9PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest09.vcproj" /Rebuild "Debug|Win32"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2008 debug
	goto end1
)
"%VC9PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest09.vcproj" /Rebuild "Release|Win32"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2008 release
	goto end1
)
"%VC9PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest09.vcproj" /Rebuild "Debug|x64"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2008 debug x64
	goto end1
)
"%VC9PATH%\..\Common7\IDE\devenv.com" "graphicstest/graphicstest09.vcproj" /Rebuild "Release|x64"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC2008 release x64
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
pause
:end2
