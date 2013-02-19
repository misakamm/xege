call setver

cd src
call clear
cd ..

set path=C:\Program Files\7-Zip
set pubpath=pub/%egever%/src
mkdir "%pubpath%"

del /Q "%pubpath%\egesrc%egever%.7z"
7z a -t7z -bd -mx=9 -ms=on "%pubpath%\egesrc%egever%.7z" "src\*"
if not "%EGE_NOPAUSE%" == "1" (
        pause
)

