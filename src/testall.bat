@echo off
call setenv

set COMPILER=

call testvc6
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	goto end1
)

call testmingw35
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	goto end1
)

call testmingw47
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	goto end1
)

call testmingw64
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	goto end1
)

call testvc9
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	goto end1
)

call testvc10
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	goto end1
)

echo "全部编译成功"
goto end2
:end1
pause
:end2
