@echo off & setlocal enabledelayedexpansion
title Build EGE Release Package
color 0A
@echo off
cls

REM We set default ege version below
set egever=13.04.02

REM Tools we use to make ege release package
REM The following line can select proper compress algorithm according
REM   to the filename extension of the archive files
set compressor=7z.exe a -mx=9
REM If we need compress to .7z, use the follow line
REM set compressor=7z.exe a -t7z -mx=9 -ms=on

REM Files to compress
set filerelease=AUTHORS ChangeLog CONTRIBUTORS COPYING README VERSION include lib man demo publish.bat 7z.exe 7z.dll
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
set pkg7zrelease=ege-%egever%-release.7z
set pkg7zfull=ege-%egever%-full.7z
set pkgziprelease=ege-%egever%-release.zip
set pkgzipfull=ege-%egever%-full.zip
mkdir "%pubpath%" 1>nul 2>nul
if %errorlevel% GEQ 2 echo.Error when making dir: %pubpath% & goto :end
echo.  Removing old packages ......
del /Q "%pubpath%\%pkg7zrelease%" 1>nul 2>nul
del /Q "%pubpath%\%pkg7zfull%" 1>nul 2>nul
del /Q "%pubpath%\%pkgziprelease%" 1>nul 2>nul
del /Q "%pubpath%\%pkgzipfull%" 1>nul 2>nul

echo.  Packing %pkg7zrelease% ......
%compressor% "%pubpath%\%pkg7zrelease%" %filerelease% 1>nul
echo.  Packing %pkg7zfull% ......
%compressor% "%pubpath%\%pkg7zfull%" %filefull% 1>nul
echo.  Packing %pkgziprelease% ......
%compressor% "%pubpath%\%pkgziprelease%" %filerelease% 1>nul
echo.  Packing %pkgzipfull% ......
%compressor% "%pubpath%\%pkgzipfull%" %filefull% 1>nul
echo.
echo.Please see %pubpath%\ to find the packages
echo.
if not "%EGE_NOPAUSE%" == "1" (
        pause
)
:end
