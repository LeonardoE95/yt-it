#!/usr/bin/env sh

rm -rf ./lib
mkdir lib
cd lib
git clone --depth 1 https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
mv libraylib.a ..
cp raylib.h  ../../../src/
cd ../../../
