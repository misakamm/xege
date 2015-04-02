@echo off
call setenv
"%VC13PATH%\..\Common7\IDE\devenv.com" "graphics13/graphics13.vcxproj" /Rebuild "Release|x64"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
"%VC13PATH%\..\Common7\IDE\devenv.com" "graphics13/graphics13.vcxproj" /Build "Release|Win32"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
