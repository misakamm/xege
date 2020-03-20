:: 以下命令用于将各版本库文件方便地输出到 build\libs 中


:: MinGW
cmake -G "MinGW Makefiles" -S . -B build\MinGW -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="%CD%\build\libs\MinGW"
cmake --build build\MinGW --config Release -j

:: Visual Studio 2008 32bit
:: FIXME: UNTESTED
cmake -G "Visual Studio 9 2008" -S . -B build\vs2008 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2008"
cmake --build build\vs2008 --config Release -j

:: Visual Studio 2008 64bit
:: FIXME: UNTESTED
cmake -G "Visual Studio 9 2008 Win64" -S . -B build\vs2008_64 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2008\amd64"
cmake --build build\vs2008_64 --config Release -j

:: Visual Studio 2010 32bit
cmake -G "Visual Studio 10 2010" -S . -B build\vs2010 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2010"
cmake --build build\vs2010 --config Release -j

:: Visual Studio 2010 64bit
cmake -G "Visual Studio 10 2010 Win64" -S . -B build\vs2010_64 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2010\amd64"
cmake --build build\vs2010_64 --config Release -j

:: Visual Studio 2012 32bit
:: FIXME: UNTESTED
cmake -G "Visual Studio 10 2012" -S . -B build\vs2012 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2012"
cmake --build build\vs2012 --config Release -j

:: Visual Studio 2012 64bit
:: FIXME: UNTESTED
cmake -G "Visual Studio 10 2012 Win64" -S . -B build\vs2012_64 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2012\amd64"
cmake --build build\vs2012_64 --config Release -j

:: Visual Studio 2013 32bit
:: FIXME: UNTESTED
cmake -G "Visual Studio 12 2013" -S . -B build\vs2013 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2013"
cmake --build build\vs2013 --config Release -j

:: Visual Studio 2013 64bit
:: FIXME: UNTESTED
cmake -G "Visual Studio 12 2013 Win64" -S . -B build\vs2013_64 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2013\amd64"
cmake --build build\vs2013_64 --config Release -j

:: Visual Studio 2015 32bit
cmake -G "Visual Studio 14 2015" -S . -B build\vs2015 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2015"
cmake --build build\vs2015 --config Release -j

:: Visual Studio 2015 64bit
cmake -G "Visual Studio 14 2015 Win64" -S . -B build\vs2015_64 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2015\amd64"
cmake --build build\vs2015_64 --config Release -j

:: Visual Studio 2017 32bit
cmake -G "Visual Studio 15 2017" -S . -B build\vs2017 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2017\x86"
cmake --build build\vs2017 --config Release -j

:: Visual Studio 2017 64bit
cmake -G "Visual Studio 15 2017 Win64" -S . -B build\vs2017_64 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2017\x64"
cmake --build build\vs2017_64 --config Release -j

:: Visual Studio 2019 32bit
cmake -G "Visual Studio 16 2019" -A Win32 -S . -B build\vs2019 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2019\x86"
cmake --build build\vs2019 --config Release -j

:: Visual Studio 2019 64bit
cmake -G "Visual Studio 16 2019" -A x64 -S . -B build\vs2019_64 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE="%CD%\build\libs\vs2019\x64"
cmake --build build\vs2019_64 --config Release -j

:: Visual C++ 6.0
:: 请使用 CMD 执行
:: 执行命令前请确保按 BUILD.md 中 `编译配置 -- Visual C++ 6.0` 一节做好设置
cmake -G "NMake Makefiles" -S . -B build\vc6 -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="%CD%\build\libs\vc6"
cmake --build build\vc6 --config Release
