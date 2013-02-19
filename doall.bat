set EGE_NOPAUSE=1

cd src
call makeall.bat
call testall.bat
cd ..
call copyziplib.bat
call copyzipman.bat
call copyzipsrc.bat
pause

