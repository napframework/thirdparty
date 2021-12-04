#!/usr/bin/env bash
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX="../osx/install" -DSPIRV_CROSS_FORCE_PIC=ON
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
