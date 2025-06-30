#!/usr/bin/env bash

cd "$(dirname "$0")/.."
EGE_DIR=$(pwd)

cd utils

# 先执行一遍 release.sh
./release.sh || exit 1

if [[ ! -d "xege_libs/.git" ]]; then
    git clone git@github.com:wysaid/xege.org.git xege_libs
fi

if ! cd xege_libs; then
    echo "Failed to enter xege_libs" >&2
    exit 1
fi

git checkout master
git reset --hard origin/master
git pull

cp -rf "$EGE_DIR/Release/lib" .
cp -rf "$EGE_DIR/include" .
cp -rf "$EGE_DIR/man" .
