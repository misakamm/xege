@echo off & setlocal enabledelayedexpansion
title Build EGE Release Package
color 0A
@echo off
cls

REM We set default ege version below
set egever=13.04.02

REM Tools we use to make ege release package
set compressor=7z.exe a -t7z -mx=9 -ms=on

REM Files to compress
set filerelease=AUTHORS ChangeLog CONTRIBUTORS COPYING README VERSION include lib man demo publish.bat 7z.exe
set filefull=%filerelease% src

REM Try to read version from file VERSION
if not exist VERSION echo.VERSION not found! & goto :CHOOSE_INPUT
for /f "tokens=*" %%a in (VERSION) do (
  set egever=%%a
)
echo.Current EGE Version is %egever%

:CHOOSE_INPUT 
choice /C ci /T 10 /D c /M "Use current version [C]; Manual input new version[I]" 
REM Manual input 
if errorlevel 2 goto :INPUT_VER
REM Use current version
if errorlevel 1 goto :START_PACK

:INPUT_VER
set /p egever=Please input new version:
echo.New version is %egever%

:START_PACK
echo.  Start packing ......

set pubpath=publish\%egever%
set pkgfull=ege-%egever%-full.7z
set pkgrelease=ege-%egever%-release.7z
mkdir "%pubpath%" 1>nul 2>nul
if %errorlevel% GEQ 2 echo.Error when making dir: %pubpath% & goto :end
echo.  Removing old packages ......
del /Q "%pubpath%\%pkgfull%" 1>nul 2>nul
del /Q "%pubpath%\%pkgrelease%" 1>nul 2>nul
echo.  Packing %pkgrelease% ......
%compressor% "%pubpath%\%pkgrelease%" %filerelease% 1>nul
echo.  Packing %pkgfull% ......
%compressor% "%pubpath%\%pkgfull%" %filefull% 1>nul

echo.
echo.Please see %pubpath%\ to find the packages
echo.
if not "%EGE_NOPAUSE%" == "1" (
        pause
)
:end
