@echo off
call setenv

mkdir "..\include\ege"
copy graphics.h "..\include\"
copy ege.h "..\include\"
copy /y "ege" "..\include\ege"
echo.
echo.
echo.
echo 请手动将ege的包含文件以及库文件拷贝到你的编译器的相应目录下
echo ------------------------------------
echo 对于VC的配置
echo.
echo 请将ege\include下的所有内容拷贝到VC++的include目录中
echo 请将ege\lib\vcxx下的所有内容拷贝到vc++的库目录中
echo 例如，对于VC++2013:
echo 将ege\include下面的内容拷贝到%VC13PATH%\include下面
echo 将ege\lib\vc2013下面的内容拷贝到%VC13PATH%\lib下面
echo.
echo ------------------------------------
echo 对于mingw的配置
echo.
echo 请将ege\include下的所有内容拷贝到mingw的include目录中
echo 请将ege\lib\mingwxxxx下的所有内容拷贝到mingw的库目录中
echo 例如，对于mingw4.7.2:
echo 将ege\include下面的内容拷贝到%MINGW32_472PATH%\include下面
echo 将ege\lib\ming32-xxx下面的内容拷贝到%MINGW32_472PATH%\lib下面
echo 对于 mingw64-3.3.0
echo 将ege\include下面的内容拷贝到%MINGW64_330PATH%\x86_64-w64-mingw32\include下面
echo 将ege\lib\vc2013下面的内容拷贝到%MINGW64_330PATH%\x86_64-w64-mingw32\lib下面
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