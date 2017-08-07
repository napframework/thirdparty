#!/usr/bin/env bash

cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P
mkdir xcode
cmake -H. -Bxcode -DCMAKE_INSTALL_PATH=./xcode/install
cd xcode
cmake --build . --target install
