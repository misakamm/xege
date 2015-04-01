echo yy | call make-ege-mingw32-4.7.2
echo yy | call make-ege-mingw32-4.8.1
echo yy | call make-ege-mingw64-4.9.2
echo yy | call setup
@del /Q *~
@del /Q *.o
@del /Q lpng\*.o
@del /Q *.swp

pause