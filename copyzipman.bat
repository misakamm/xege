call setver
set path=C:\Program Files\7-Zip
set pubpath=pub/%egever%/man
mkdir "%pubpath%"

del /Q "%pubpath%\egeman%egever%.7z"
7z a -t7z -bd -mx=9 -ms=on "%pubpath%\egeman%egever%.7z" "man\*"
if not "%EGE_NOPAUSE%" == "1" (
        pause
)

