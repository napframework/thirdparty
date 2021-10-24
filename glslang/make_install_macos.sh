#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"
mkdir build
cd build
../../cmake/macos/x86_64/bin/cmake ../source -DCMAKE_INSTALL_PREFIX="../macos/x86_64" -DENABLE_HLSL=OFF
../../cmake/macos/x86_64/bin/cmake --build . --config Release --target install
../../cmake/macos/x86_64/bin/cmake --build . --config Debug --target install
