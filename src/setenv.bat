@echo off
REM ΢����2005��֮���������ΪVS�ķ��д��ţ������ڲ��汾�Ų�һ��

REM  �ڲ��汾��     ��汾��         ���а汾��
REM MS VC++ 12.0 _MSC_VER = 1800 (Visual C++ 2013)
REM MS VC++ 11.0 _MSC_VER = 1700 (Visual C++ 2012)
REM MS VC++ 10.0 _MSC_VER = 1600 (Visual C++ 2010)
REM MS VC++ 9.0  _MSC_VER = 1500 (Visual C++ 2008)
REM MS VC++ 8.0  _MSC_VER = 1400 (Visual C++ 2005)
REM MS VC++ 7.1  _MSC_VER = 1310 (Visual C++ 7.1)
REM MS VC++ 7.0  _MSC_VER = 1300 (Visual C++ 7.0)
REM MS VC++ 6.0  _MSC_VER = 1200 (Visual C++ 6.0)
REM MS VC++ 5.0  _MSC_VER = 1100 (Visual C++ 5.0)
echo.
echo.
echo ==========ע�⣺������Visual C++���ǰ�װ��C:\Program Files (x86)Ŀ¼==========
echo ==========   �������Ȱ�Ctrl-C��Ȼ�����ı��༭���޸� setenv.bat ������  ==========
echo.
echo.

set VC10PATH=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC
set VC12PATH=C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC
set VC13PATH=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
set VC15PATH=C:\Program Files (x86)\Microsoft Visual Studio 13.0\VC

rem MSBuild ��������VC++�İ�װ����װVC++Ҳ����MSBuild���룩
rem ����VS2013 Express��ֻ��ʹ��MSBuild 2013ִ�������б���
rem ���ص�ַ��http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs#DownloadFamilies_3
set MSBUILD13PATH=C:\Program Files (x86)\MSBuild\12.0
rem �����汾��MSBuild(�����еĻ�)
set MSBUILD10PATH=C:\Program Files (x86)\MSBuild\10.0
set MSBUILD12PATH=C:\Program Files (x86)\MSBuild\11.0
set MSBUILD15PATH=C:\Program Files (x86)\MSBuild\13.0

echo.
echo.
echo ==========ע�⣺�����ʹ��mingw��gcc/g++�����룬�����Ȱ�Ctrl-C�˳�������==========
echo ==========      Ȼ�����ı��༭���޸� setenv.bat �й���mingw������      ==========
echo.
echo.
pause

rem set MINGWMAKEFILE=mingw
rem set MINGWPATH=C:\Dev-Cpp
set MINGW_BASE_PATH=D:\mingw
set MINGW32_472DIR=mingw-4.7.2
set MINGW32_472PATH=%MINGW_BASE_PATH%\%MINGW32_472DIR%
set MINGW32_481DIR=mingw-4.8.1
set MINGW32_481PATH=%MINGW_BASE_PATH%\%MINGW32_481DIR%
rem set MINGW64_492DIR=mingw64-4.9.2
rem set MINGW64_492PATH=%MINGW_BASE_PATH%\%MINGW64_492DIR%

REM ��������http://www.drangon.org/mingw/��mingw64-4.9.2�޷�����32λ�⣬�ʴ˷���
REM ��Ϊʹ��TDM���е�MinGW64
set MINGW64_492DIR=TDM-GCC-64
set MINGW64_492PATH=%MINGW_BASE_PATH%\%MINGW64_492DIR%

rem ����ʹ��Dev-C++���Դ���MinGW��
rem ע�⣺����Ҫȥ�����Dev-Cpp��װ�����ĸ��汾��MinGW��
set MINGW32_DIR=mingw32
set MINGW32PATH=C:\Program Files (x86)\Dev-Cpp\MinGW32
set MINGW64_DIR=mingw64
set MINGW64PATH=C:\Program Files (x86)\Dev-Cpp\MinGW64