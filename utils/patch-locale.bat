@echo off

rem USAGE: patch-locale.bat <command> <args...>

setlocal enableDelayedExpansion

call :localeName patch
%*
call :localeName unpatch
exit /b

rem https://raymai97.github.io/myblog/msvc-support-utf8-string-literal-since-vc6#update-for-vc60-the-key-is-localename

:localeName
set _path_="HKCU\Control Panel\International"
set _name_=LocaleName
if "%~1"=="patch" (
	call :localeName get _localeName_
	call :localeName set en-US
) else if "%~1"=="unpatch" (
	call :localeName set !_localeName_!
) else if "%~1"=="get" (
	for /f "tokens=3 skip=2" %%i in ('reg query !_path_! /v !_name_!') do (
		set _localeName_=%%i
	)
) else if "%~1"=="set" (
	reg add !_path_! /v !_name_! /d "%~2" /f >nul || exit/b 1
)
exit /b
:localeNameEnd
