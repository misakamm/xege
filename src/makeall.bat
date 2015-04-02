@echo off
rd /s /q "..\lib"
call make-vc6
call make-vc9
call make-vc10
call make-vc12
call make-vc13
call make-mingw3.4
call make-mingw4.7
call make-mingw4.8
call make-mingw64
@del /Q *~
@del /Q *.o
@del /Q *.swp
if not "%EGE_NOPAUSE%" == "1" (
        pause
)

