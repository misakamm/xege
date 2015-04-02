call ../setenv
mkdir "..\..\lib\vc6\lib"
"%VC6PATH%\Bin\LIB.EXE" /out:"..\..\lib\vc6\lib\graphics.lib" "Release\graphics.lib" "..\Lib\GdiPlus.lib"
echo link lib graphics.lib success
