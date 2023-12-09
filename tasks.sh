#!/usr/bin/env bash

cd "$(dirname "$0")"
PROJECT_DIR=$(pwd)

CMAKE_VS_DIR="$PROJECT_DIR/build"

# 如果是在 vscode 里面调用, 那么不给错误码, 否则会导致 vscode tasks 无法正常执行任务并显示错误.

if [[ "$TERM_PROGRAM" =~ vscode ]]; then
    export EXIT_WHEN_FAILED=false
else
    export EXIT_WHEN_FAILED=true
fi

export BUILD_TARGET="xege" # 默认只构建 xege 静态库

# 默认开Release模式
export CMAKE_BUILD_TYPE="Release"
function MY_CMAKE_BUILD_DEFINE() {
    echo "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
}

if ! command -v grealpath && command -v realpath; then
    function grealpath() {
        realpath $@
    }
fi

function isWsl() {
    [[ -d "/mnt/c" ]] || command -v wslpath &>/dev/null
}

function isWindows() {
    # check mingw and cygwin
    isWsl || [[ -d "/c" ]] || [[ -d "/cygdrive/c" ]]
}

function isMacOS() {
    [[ "$(uname)" == "Darwin" ]]
}

function loadCMakeProject() {
    if mkdir -p "$CMAKE_VS_DIR" &&
        cd "$CMAKE_VS_DIR" &&
        cmake "$(MY_CMAKE_BUILD_DEFINE)" \
            ..; then
        echo "CMake Project Loaded!"
    else
        echo "CMake Project Load Failed!"
        exit 1
    fi
}

function cmakeCleanAll() {
    pushd $PROJECT_DIR
    git clean -ffdx build
    popd
}

function reloadCMakeProject() {
    cmakeCleanAll && loadCMakeProject
}

function cmakeBuildAll() {
    pushd "$CMAKE_VS_DIR"
    if isWindows; then

        if [[ -n "$CMAKE_BUILD_TYPE" ]]; then
            export WIN_CMAKE_BUILD_DEFINE="--config $CMAKE_BUILD_TYPE"
        fi

        echo start: cmake.exe --build . --target "$BUILD_TARGET" $WIN_CMAKE_BUILD_DEFINE -- /m
        # ref: https://stackoverflow.com/questions/11865085/out-of-a-git-console-how-do-i-execute-a-batch-file-and-then-return-to-git-conso
        if ! cmd "/C cmake.exe --build . --target "$BUILD_TARGET" $WIN_CMAKE_BUILD_DEFINE -- /m" && $EXIT_WHEN_FAILED; then
            exit 1
        fi
        echo end: cmake.exe --build . --target "$BUILD_TARGET" $WIN_CMAKE_BUILD_DEFINE -- /m
    else
        if ! cmake --build . --target "$BUILD_TARGET" $(test -n "$CMAKE_BUILD_TYPE" && echo --config $CMAKE_BUILD_TYPE) -- -j $(nproc) && $EXIT_WHEN_FAILED; then
            exit 1
        fi
    fi
    popd
}

if [[ $# -eq 0 ]]; then
    echo "usage: [--load] [--reload] [--clean] [--build]"
fi

while [[ $# > 0 ]]; do

    PARSE_KEY="$1"

    case "$PARSE_KEY" in
    --load)
        echo "loadCMakeProject"
        loadCMakeProject
        shift # past argument
        ;;
    --reload)
        echo "reloadCMakeProject"
        reloadCMakeProject
        shift # past argument
        ;;
    --clean)
        echo "clean"
        cmakeCleanAll
        shift # past argument
        ;;
    --build)
        echo "build"
        cmakeBuildAll
        shift # past argument
        ;;
    --debug)
        echo "enable debug mode"
        export CMAKE_BUILD_TYPE="Debug"
        shift
        ;;
    --release)
        echo "enable release mode"
        export CMAKE_BUILD_TYPE="Release"
        shift # past argument
        ;;
    --target)
        echo "set build target to $2"
        export BUILD_TARGET="$2"
        shift
        shift
        ;;
    --run)
        if isWindows; then
            echo "run $CMAKE_VS_DIR/$CMAKE_BUILD_TYPE/$2"
            "$CMAKE_VS_DIR/$CMAKE_BUILD_TYPE/$2"
        elif isMacOS; then
            echo run "$CMAKE_VS_DIR/$2"
            wine64 "$CMAKE_VS_DIR/$2"
        fi
        shift
        shift
        ;;
    *)
        echo "unknown option $PARSE_KEY..."
        exit 1
        ;;
    esac
done
