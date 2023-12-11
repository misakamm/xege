#!/usr/bin/env bash
set -x
x86_64-w64-mingw32-g++ -D_FORTIFY_SOURCE=0 $* -lgraphics64 -lgdiplus -lgdi32 -limm32 -lmsimg32 -lole32 -loleaut32 -lwinmm -luuid -mwindows -static
