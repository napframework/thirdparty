#!/usr/bin/env bash
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX="../osx/install" -DENABLE_HLSL=OFF
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
