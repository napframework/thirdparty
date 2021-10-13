#!/usr/bin/env bash

cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P
mkdir xcode
cmake -H. -Bxcode -DCMAKE_INSTALL_PATH=./xcode/install
cd xcode
cmake --build . --target install --config Release -- -j 4
cmake --build . --target install --config Debug -- -j 4
install_name_tool -id @rpath/librttr_core_d.0.9.6.dylib install/bin/librttr_core_d.0.9.6.dylib
install_name_tool -id @rpath/librttr_core.0.9.6.dylib install/bin/librttr_core.0.9.6.dylib