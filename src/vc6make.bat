@echo off
call setenv
"%VC6PATH%\..\Common\MSDev98\Bin\MSDEV.COM" "graphics/graphics.dsw" /make "graphics - Win32 Release" /CLEAN
"%VC6PATH%\..\Common\MSDev98\Bin\MSDEV.COM" "graphics/graphics.dsw" /make "graphics - Win32 Release"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
