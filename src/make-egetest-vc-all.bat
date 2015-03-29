@echo off
call setenv

set COMPILER=
set FAILOR=
set SUCCESSOR=

set COMPILER=vc15
call make-egetest-vc15
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %COMPILER%
) else (
    set FAILOR=%FAILOR%  %COMPILER%
)

set COMPILER=vc13
call make-egetest-vc13
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %COMPILER%
) else (
    set FAILOR=%FAILOR%  %COMPILER%
)

set COMPILER=vc12
call make-egetest-vc12
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %COMPILER%
) else (
    set FAILOR=%FAILOR%  %COMPILER%
)

set COMPILER=vc10
call make-egetest-vc10
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %COMPILER%
) else (
    set FAILOR=%FAILOR%  %COMPILER%
)

set COMPILER=mingw32-4.7.2
call make-egetest-mingw32-4.7.2
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %COMPILER%
) else (
    set FAILOR=%FAILOR%  %COMPILER%
)

set COMPILER=mingw32-4.8.1
call make-egetest-mingw32-4.8.1
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %COMPILER%
) else (
    set FAILOR=%FAILOR%  %COMPILER%
)

call make-egetest-mingw64-4.9.2
set COMPILER=mingw32-4.9.2
if %ErrorLevel% == 0 (
	set SUCCESSOR=%SUCCESSOR%  %COMPILER%
) else (
    set FAILOR=%FAILOR%  %COMPILER%
)

echo ±‡“Î≥…π¶: %SUCCESSOR%
echo ±‡“Î ß∞‹: %FAILOR%

pause
