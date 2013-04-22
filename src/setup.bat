call setenv

mkdir "..\include\ege"
copy graphics.h "..\include\"
copy ege.h "..\include\"
copy /y "ege" "..\include\ege"

mkdir "%VC6PATH%\include\ege"
copy ..\include\*.h "%VC6PATH%\include\"
copy ..\include\ege\*.h "%VC6PATH%\include\ege"
copy /Y ..\lib\vc6\* "%VC6PATH%\lib\"

mkdir "%VC9PATH%\include\ege"
copy ..\include\*.h "%VC9PATH%\include\"
copy ..\include\ege\*.h "%VC9PATH%\include\ege"
copy /Y ..\lib\vc2008\* "%VC9PATH%\lib\"
copy /Y ..\lib\vc2008\amd64\* "%VC9PATH%\lib\amd64\"

mkdir "%VC10PATH%\include\ege"
copy ..\include\*.h "%VC10PATH%\include\"
copy ..\include\ege\*.h "%VC10PATH%\include\ege"
copy /Y ..\lib\vc2010\* "%VC10PATH%\lib\"
copy /Y ..\lib\vc2010\amd64\* "%VC10PATH%\lib\amd64\"

mkdir "%VC12PATH%\include\ege"
copy ..\include\*.h "%VC12PATH%\include\"
copy ..\include\ege\*.h "%VC12PATH%\include\ege"
copy /Y ..\lib\vc2012\* "%VC12PATH%\lib\"
copy /Y ..\lib\vc2012\amd64\* "%VC12PATH%\lib\amd64\"

mkdir "%MINGW35PATH%\include\ege"
copy ..\include\*.h "%MINGW35PATH%\include\"
copy ..\include\ege\*.h "%MINGW35PATH%\include\ege\"
copy /y ..\lib\mingw3.4\* "%MINGW35PATH%\lib\"

mkdir "%MINGW47PATH%\include\ege"
copy ..\include\*.h "%MINGW47PATH%\include\"
copy ..\include\ege\*.h "%MINGW47PATH%\include\ege\"
copy /y ..\lib\mingw4.7\* "%MINGW47PATH%\lib\"

mkdir "%MINGW472PATH%\include\ege"
copy ..\include\*.h "%MINGW472PATH%\include\"
copy ..\include\ege\*.h "%MINGW472PATH%\include\ege\"
copy /y ..\lib\mingw4.7.2\* "%MINGW472PATH%\lib\"

mkdir "%MINGW64PATH%\x86_64-w64-mingw32\include\ege"
copy ..\include\*.h "%MINGW64PATH%\x86_64-w64-mingw32\include\"
copy ..\include\ege\*.h "%MINGW64PATH%\x86_64-w64-mingw32\include\ege\"
copy /y ..\lib\mingw64\* "%MINGW64PATH%\x86_64-w64-mingw32\lib\"
