@echo off
call setenv

set COMPILER=

"%VC6PATH%\..\Common\MSDev98\Bin\MSDEV.COM" "graphicstest/graphicstest06.dsw" /make "ALL" /CLEAN
"%VC6PATH%\..\Common\MSDev98\Bin\MSDEV.COM" "graphicstest/graphicstest06.dsw" /make "ALL"
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	set COMPILER=VC6
	goto end1
)

goto end2
:end1
echo "%COMPILER% ±‡“Î ß∞‹"
pause
:end2
