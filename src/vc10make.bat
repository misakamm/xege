"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com" "graphics10/graphics10.vcxproj" /Rebuild "Release|x64"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com" "graphics10/graphics10.vcxproj" /Build "Release|Win32"
if not %ErrorLevel% == 0 (
        pause
        goto end
)
:end
