#!/usr/bin/env bash

cd "$(dirname "$0")/.."

EGE_DIR=$(pwd)

git clean -ffdx build Release

declare -a FAILED_TASKS=()

if [[ $(uname -s) == "Darwin" ]]; then
    # macos/linux
    if ./tasks.sh --clean --release --load --target xege --build; then
        mkdir -p Release/lib/macOS
        cp -rf build/*.a Release/lib/macOS
        echo "Copy macOS libs done: $(pwd)/Release/lib/macOS"
    else
        echo "Build macOS failed!" >&2
        FAILED_TASKS+=("macOS")
    fi
elif [[ $(uname -s) == "Linux" ]]; then
    if ./tasks.sh --clean --release --load --target xege --build; then
        # 目前暂时只提供 mingw-w64-debian 的版本, 所以默认 mingw-w64-debian. 后续如果要支持更多, 那么再改.
        mkdir -p Release/lib/mingw-w64-debian
        cp -rf build/*.a Release/lib/mingw-w64-debian
        echo "Copy mingw-w64-debian libs done: $(pwd)/Release/lib/mingw-w64-debian"
    else
        echo "Build mingw-w64-debian failed!" >&2
        FAILED_TASKS+=("mingw-w64-debian")
    fi
else
    # vs2022 - 64bit
    if ./tasks.sh --clean --release --toolset v143 --arch x64 --target xege --load --build; then
        mkdir -p Release/lib/vs2022/x64
        cp -rf build/Release/*.lib Release/lib/vs2022/x64
        git clean -ffdx build/Release
        echo "Copy vs2022 x64 libs done: $(pwd)/Release/lib/vs2022/x64"
    else
        echo "Build vs2022 x64 failed!" >&2
        FAILED_TASKS+=("vs2022 x64")
    fi

    # vs2022 - 32bit
    if ./tasks.sh --clean --release --toolset v143 --arch Win32 --target xege --load --build; then
        mkdir -p Release/lib/vs2022/x86
        cp -rf build/Release/*.lib Release/lib/vs2022/x86
        git clean -ffdx build/Release
        echo "Copy vs2022 x86 libs done: $(pwd)/Release/lib/vs2022/x86"
    else
        echo "Build vs2022 x86 failed!" >&2
        FAILED_TASKS+=("vs2022 x86")
    fi

    # vs2019 - 64bit
    if ./tasks.sh --clean --release --target xege --toolset v142 --arch x64 --load --build; then
        mkdir -p Release/lib/vs2019/x64
        cp -rf build/Release/*.lib Release/lib/vs2019/x64
        git clean -ffdx build/Release
        echo "Copy vs2019 x64 libs done: $(pwd)/Release/lib/vs2019/x64"
    else
        echo "Build vs2019 x64 failed!" >&2
        FAILED_TASKS+=("vs2019 x64")
    fi

    # vs2019 - 32bit
    if ./tasks.sh --clean --release --target xege --toolset v142 --arch Win32 --load --build; then
        mkdir -p Release/lib/vs2019/x86
        cp -rf build/Release/*.lib Release/lib/vs2019/x86
        git clean -ffdx build/Release
        echo "Copy vs2019 x86 libs done: $(pwd)/Release/lib/vs2019/x86"
    else
        echo "Build vs2019 x86 failed!" >&2
        FAILED_TASKS+=("vs2019 x86")
    fi

    # vs2017 - 64bit
    if ./tasks.sh --clean --release --target xege --toolset v141 --arch x64 --load --build; then
        mkdir -p Release/lib/vs2017/x64
        cp -rf build/Release/*.lib Release/lib/vs2017/x64
        git clean -ffdx build/Release
        echo "Copy vs2017 x64 libs done: $(pwd)/Release/lib/vs2017/x64"
    else
        echo "Build vs2017 x64 failed!" >&2
        FAILED_TASKS+=("vs2017 x64")
    fi

    # vs2017 - 32bit
    if ./tasks.sh --clean --release --target xege --toolset v141 --arch Win32 --load --build; then
        mkdir -p Release/lib/vs2017/x86
        cp -rf build/Release/*.lib Release/lib/vs2017/x86
        git clean -ffdx build/Release
        echo "Copy vs2017 x86 libs done: $(pwd)/Release/lib/vs2017/x86"
    else
        echo "Build vs2017 x86 failed!" >&2
        FAILED_TASKS+=("vs2017 x86")
    fi

    # vs2015 - 64bit

    if ./tasks.sh --clean --release --target xege --toolset v140 --arch x64 --load --build; then
        mkdir -p Release/lib/vs2015/amd64
        cp -rf build/Release/*.lib Release/lib/vs2015/amd64
        git clean -ffdx build/Release
        echo "Copy vs2015 x64 libs done: $(pwd)/Release/lib/vs2015/amd64"
    else
        echo "Build vs2015 x64 failed!" >&2
        FAILED_TASKS+=("vs2015 x64")
    fi

    # vs2015 - 32bit
    if ./tasks.sh --clean --release --target xege --toolset v140 --arch Win32 --load --build; then
        mkdir -p Release/lib/vs2015
        cp -rf build/Release/*.lib Release/lib/vs2015
        git clean -ffdx build/Release
        echo "Copy vs2015 x86 libs done: $(pwd)/Release/lib/vs2015"
    else
        echo "Build vs2015 x86 failed!" >&2
        FAILED_TASKS+=("vs2015 x86")
    fi

    # vs2010 - 64bit
    if ./tasks.sh --clean --release --target xege --toolset v100 --arch x64 --load --build; then
        mkdir -p Release/lib/vs2010/amd64
        cp -rf build/Release/*.lib Release/lib/vs2010/amd64
        git clean -ffdx build/Release
        echo "Copy vs2010 x64 libs done: $(pwd)/Release/lib/vs2010/amd64"
    else
        echo "Build vs2010 x64 failed!" >&2
        FAILED_TASKS+=("vs2010 x64")
    fi

    # vs2010 - 32bit
    if ./tasks.sh --clean --release --target xege --toolset v100 --arch Win32 --load --build; then
        mkdir -p Release/lib/vs2010
        cp -rf build/Release/*.lib Release/lib/vs2010
        git clean -ffdx build/Release
        echo "Copy vs2010 x86 libs done: $(pwd)/Release/lib/vs2010"
    else
        echo "Build vs2010 x86 failed!" >&2
        FAILED_TASKS+=("vs2010 x86")
    fi
fi

if [[ ${#FAILED_TASKS[@]} -gt 0 ]]; then
    echo "Failed tasks:" >&2
    printf "  %s\n" "${FAILED_TASKS[@]}" >&2
    exit ${#FAILED_TASKS[@]}
fi
