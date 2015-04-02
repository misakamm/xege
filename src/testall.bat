@echo off
call setenv

set COMPILER=

call setup

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

call testmingw48
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

call testvc12
if %ErrorLevel% == 0 (
	rem SUCCESS
) else (
	goto end1
)

call testvc13
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
