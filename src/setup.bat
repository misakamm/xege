@echo off
call setenv

mkdir "..\include\ege"
copy graphics.h "..\include\"
copy ege.h "..\include\"
copy /y "ege" "..\include\ege"
echo.
echo.
echo.
echo ���ֶ���ege�İ����ļ��Լ����ļ���������ı���������ӦĿ¼��
echo ------------------------------------
echo ����VC������
echo.
echo �뽫ege\include�µ��������ݿ�����VC++��includeĿ¼��
echo �뽫ege\lib\vcxx�µ��������ݿ�����vc++�Ŀ�Ŀ¼��
echo ���磬����VC++2013:
echo ��ege\include��������ݿ�����%VC13PATH%\include����
echo ��ege\lib\vc2013��������ݿ�����%VC13PATH%\lib����
echo.
echo ------------------------------------
echo ����mingw������
echo.
echo �뽫ege\include�µ��������ݿ�����mingw��includeĿ¼��
echo �뽫ege\lib\mingwxxxx�µ��������ݿ�����mingw�Ŀ�Ŀ¼��
echo ���磬����mingw4.7.2:
echo ��ege\include��������ݿ�����%MINGW32_472PATH%\include����
echo ��ege\lib\ming32-xxx��������ݿ�����%MINGW32_472PATH%\lib����
echo ���� mingw64-3.3.0
echo ��ege\include��������ݿ�����%MINGW64_330PATH%\x86_64-w64-mingw32\include����
echo ��ege\lib\vc2013��������ݿ�����%MINGW64_330PATH%\x86_64-w64-mingw32\lib����
echo.
pause
rem  mkdir "%VC10PATH%\include\ege"
rem  copy ..\include\*.h "%VC10PATH%\include\"
rem  copy ..\include\ege\*.h "%VC10PATH%\include\ege"
rem  copy /Y ..\lib\vc2010\* "%VC10PATH%\lib\"
rem  copy /Y ..\lib\vc2010\amd64\* "%VC10PATH%\lib\amd64\"
rem  
rem  mkdir "%VC12PATH%\include\ege"
rem  copy ..\include\*.h "%VC12PATH%\include\"
rem  copy ..\include\ege\*.h "%VC12PATH%\include\ege"
rem  copy /Y ..\lib\vc2012\* "%VC12PATH%\lib\"
rem  copy /Y ..\lib\vc2012\amd64\* "%VC12PATH%\lib\amd64\"
rem  
rem  mkdir "%VC13PATH%\include\ege"
rem  copy ..\include\*.h "%VC12PATH%\include\"
rem  copy ..\include\ege\*.h "%VC12PATH%\include\ege"
rem  copy /Y ..\lib\vc2013\* "%VC12PATH%\lib\"
rem  copy /Y ..\lib\vc2013\amd64\* "%VC12PATH%\lib\amd64\"
rem  
rem  mkdir "%VC15PATH%\include\ege"
rem  copy ..\include\*.h "%VC12PATH%\include\"
rem  copy ..\include\ege\*.h "%VC12PATH%\include\ege"
rem  copy /Y ..\lib\vc2015\* "%VC12PATH%\lib\"
rem  copy /Y ..\lib\vc2015\amd64\* "%VC12PATH%\lib\amd64\"
rem  
rem  mkdir "%MINGW472PATH%\include\ege"
rem  copy ..\include\*.h "%MINGW472PATH%\include\"
rem  copy ..\include\ege\*.h "%MINGW472PATH%\include\ege\"
rem  copy /y ..\lib\mingw4.7.2\* "%MINGW472PATH%\lib\"
rem  
rem  mkdir "%MINGW64PATH%\x86_64-w64-mingw32\include\ege"
rem  copy ..\include\*.h "%MINGW64PATH%\x86_64-w64-mingw32\include\"
rem  copy ..\include\ege\*.h "%MINGW64PATH%\x86_64-w64-mingw32\include\ege\"
rem  copy /y ..\lib\mingw64\* "%MINGW64PATH%\x86_64-w64-mingw32\lib\"
rem  