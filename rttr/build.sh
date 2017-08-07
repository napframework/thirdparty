#!/usr/bin/env bash
mkdir linux
cmake -H. -Blinux -DCMAKE_INSTALL_PATH=./linux/install
cd linux
cmake --build . --target install