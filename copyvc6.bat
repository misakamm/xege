call setver
set path=C:\Program Files\7-Zip
set pubpath=pub/%egever%/bin

copy include\* vc6\VC98\Include\
mkdir vc6\VC98\Include\ege
copy include\ege\* vc6\VC98\Include\ege\
copy lib\vc6\lib\* vc6\VC98\Lib\
mkdir "%pubpath%"
del /Q "%pubpath%\vc6_ege%egever%.7z"
7z a -t7z -bd -mx=9 -ms=on "%pubpath%\vc6_ege%egever%.7z" "vc6\*"
if not "%EGE_NOPAUSE%" == "1" (
        pause
)

