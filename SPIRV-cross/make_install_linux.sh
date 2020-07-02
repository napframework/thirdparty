#!/usr/bin/env bash
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX="../linux/install"
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
