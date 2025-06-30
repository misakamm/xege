#!/usr/bin/env bash

function isWsl() {
    [[ -d "/mnt/c" ]] || command -v wslpath &>/dev/null
}

function isWindows() {
    [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]] || isWsl || [[ -n "$WINDIR" ]]
}

if isWsl; then
    # Switch to Git Bash when running in WSL
    echo "You're using WSL, but WSL linux is not supported! Tring to run with Git Bash!" >&2
    GIT_BASH_PATH_WIN=$(/mnt/c/Windows/system32/cmd.exe /C "where bash.exe" | grep -i Git | head -n 1 | tr -d '\n\r')
    GIT_BASH_PATH_WSL=$(wslpath -u "$GIT_BASH_PATH_WIN")
    echo "== GIT_BASH_PATH_WIN=$GIT_BASH_PATH_WIN"
    echo "== GIT_BASH_PATH_WSL=$GIT_BASH_PATH_WSL"
    if [[ -f "$GIT_BASH_PATH_WSL" ]]; then
        THIS_BASE_NAME=$(basename "$0")
        "$GIT_BASH_PATH_WSL" "$THIS_BASE_NAME" $@
        exit $?
    else
        echo "Git Bash not found, please install Git Bash!" >&2
        exit 1
    fi
fi

cd "$(dirname "$0")"
PROJECT_DIR=$(pwd)

CMAKE_VS_DIR="$PROJECT_DIR/build"

# 如果是在 vscode 里面调用, 那么不给错误码, 否则会导致 vscode tasks 无法正常执行任务并显示错误.

if [[ "$TERM_PROGRAM" =~ vscode ]]; then
    export EXIT_WHEN_FAILED=false
else
    export EXIT_WHEN_FAILED=true
fi

export BUILD_TARGET="" # 默认只构建 xege 静态库

# 默认开Release模式
export CMAKE_BUILD_TYPE="Release"

if [[ -z "$WIN_CMAKE_BUILD_DEFINE" ]]; then
    export WIN_CMAKE_BUILD_DEFINE=""
fi

if [[ -z "$CMAKE_CONFIG_DEFINE" ]]; then
    export CMAKE_CONFIG_DEFINE="-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
fi

function MY_CMAKE_BUILD_DEFINE() {
    echo "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ${CMAKE_CONFIG_DEFINE}"
}

if ! command -v grealpath && command -v realpath; then
    function grealpath() {
        realpath $@
    }
fi

function loadCMakeProject() {

    echo "Run cmake command: cmake "$(MY_CMAKE_BUILD_DEFINE)" .."

    if mkdir -p "$CMAKE_VS_DIR" &&
        cd "$CMAKE_VS_DIR" &&
        cmake $(MY_CMAKE_BUILD_DEFINE) ..; then
        echo "CMake Project Loaded: MY_CMAKE_BUILD_DEFINE=$(MY_CMAKE_BUILD_DEFINE)"
    elif $EXIT_WHEN_FAILED; then
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

    if [[ -n "$BUILD_TARGET" ]]; then
        TARGET_RULE="--target $BUILD_TARGET"
    else
        TARGET_RULE=""
    fi

    if isWindows; then

        if [[ -n "$CMAKE_BUILD_TYPE" ]]; then
            export WIN_CMAKE_BUILD_DEFINE="$WIN_CMAKE_BUILD_DEFINE --config $CMAKE_BUILD_TYPE"
        fi

        echo start: cmake.exe --build . $TARGET_RULE $WIN_CMAKE_BUILD_DEFINE -- /m
        # ref: https://stackoverflow.com/questions/11865085/out-of-a-git-console-how-do-i-execute-a-batch-file-and-then-return-to-git-conso
        if ! cmd "/C cmake.exe --build . $TARGET_RULE $WIN_CMAKE_BUILD_DEFINE -- /m" && $EXIT_WHEN_FAILED; then
            exit 1
        fi
        echo end: cmake.exe --build . $TARGET_RULE $WIN_CMAKE_BUILD_DEFINE -- /m
    else
        if ! cmake --build . $TARGET_RULE $(test -n "$CMAKE_BUILD_TYPE" && echo --config $CMAKE_BUILD_TYPE) -- -j $(nproc) && $EXIT_WHEN_FAILED; then
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
        if [[ ! -f "$CMAKE_VS_DIR/CMakeCache.txt" ]]; then
            loadCMakeProject
        fi
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
        if [[ $BUILD_TARGET == "demos" ]]; then
            export CMAKE_CONFIG_DEFINE="$CMAKE_CONFIG_DEFINE -DEGE_BUILD_DEMO=ON"
        fi
        shift
        shift
        ;;
    --toolset)
        echo "set toolset to $2"
        export CMAKE_CONFIG_DEFINE="$CMAKE_CONFIG_DEFINE -T $2"
        shift
        shift
        ;;
    --arch)
        echo "set arch to $2"
        export CMAKE_CONFIG_DEFINE="$CMAKE_CONFIG_DEFINE -A $2"
        shift
        shift
        ;;
    --run)
        if isWindows; then
            echo "run $CMAKE_VS_DIR/demo/$CMAKE_BUILD_TYPE/$2"
            "$CMAKE_VS_DIR/demo/$CMAKE_BUILD_TYPE/$2"
        else
            echo run "$CMAKE_VS_DIR/demo/$2"
            if command -v wine64 &>/dev/null; then
                wine64 "$CMAKE_VS_DIR/demo/$2"
            elif command -v wine &>/dev/null; then
                wine "$CMAKE_VS_DIR/demo/$2"
            else
                echo "Command 'wine64' not found, please install wine first."
            fi
        fi
        shift
        shift
        ;;
    *)
        echo "unknown option $PARSE_KEY..."
        if $EXIT_WHEN_FAILED; then
            exit 1
        fi
        ;;
    esac
done
