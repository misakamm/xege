@echo off
call setenv
"%VC9PATH%\..\Common7\IDE\devenv.com" "graphics08/graphics08.vcproj" /Rebuild "Release|x64"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
"%VC9PATH%\..\Common7\IDE\devenv.com" "graphics08/graphics08.vcproj" /Rebuild "Release|Win32"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
