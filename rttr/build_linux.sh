#!/usr/bin/env bash

cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P
mkdir linux
cmake -H. -Blinux -DCMAKE_INSTALL_PATH=./linux/install
cd linux
cmake --build . --target install
