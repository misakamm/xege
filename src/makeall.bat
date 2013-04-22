@echo off
call vc6make
call vc9make
call vc10make
call vc12make
call mingw3.4make
call mingw4.7make
call mingw64make
call setup
@del /Q *~
@del /Q *.o
@del /Q *.swp
if not "%EGE_NOPAUSE%" == "1" (
        pause
)

