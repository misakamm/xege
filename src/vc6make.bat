"D:\VC6\Common\MSDev98\Bin\MSDEV.COM" "graphics/graphics.dsw" /make "graphics - Win32 Release" /CLEAN
"D:\VC6\Common\MSDev98\Bin\MSDEV.COM" "graphics/graphics.dsw" /make "graphics - Win32 Release"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
