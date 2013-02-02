"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.com" "graphics08/graphics08.vcproj" /Rebuild "Release|x64"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.com" "graphics08/graphics08.vcproj" /Rebuild "Release|Win32"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
