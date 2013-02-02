"D:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.com" "graphics12/graphics12.vcxproj" /Rebuild "Release|x64"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
"D:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.com" "graphics12/graphics12.vcxproj" /Build "Release|Win32"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
