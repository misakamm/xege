@echo off
call setenv
"%VC10PATH%\..\Common7\IDE\devenv.com" "graphics10/graphics10.vcxproj" /Rebuild "Release|x64"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
"%VC10PATH%\..\Common7\IDE\devenv.com" "graphics10/graphics10.vcxproj" /Build "Release|Win32"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
