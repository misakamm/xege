@echo off
call setenv
"%VC12PATH%\..\Common7\IDE\devenv.com" "graphics12/graphics12.vcxproj" /Rebuild "Release|x64"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
"%VC12PATH%\..\Common7\IDE\devenv.com" "graphics12/graphics12.vcxproj" /Build "Release|Win32"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
