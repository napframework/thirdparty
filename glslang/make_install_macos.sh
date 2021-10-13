#!/usr/bin/env bash
cd source
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX="../../macos/x86_64" -DENABLE_HLSL=OFF
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
