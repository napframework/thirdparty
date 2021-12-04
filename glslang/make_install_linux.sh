#!/usr/bin/env bash
mkdir build
cd build
cmake ../source -DCMAKE_INSTALL_PREFIX="../linux/x86_64" -DENABLE_HLSL=OFF
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
