call setver
set path=C:\Program Files\7-Zip
set pubpath=pub/%egever%/bin
mkdir "%pubpath%"
del /Q "%pubpath%\ege%egever%.zip"
7z a -tzip -bd -mx=9 -mm=LZMA "%pubpath%\ege%egever%.zip" "lib\*" "include\*" "说明.txt" "demo\src\*" "man\*"
del /Q "%pubpath%\ege%egever%.7z"
7z a -t7z -bd -mx=9 -ms=on "%pubpath%\ege%egever%.7z" "lib\*" "include\*" "说明.txt" "demo\src\*" "man\*"
if not "%EGE_NOPAUSE%" == "1" (
        pause
)

