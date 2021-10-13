@echo off
cd source
mkdir msvc64
cd msvc64
cmake ../ -DCMAKE_INSTALL_PREFIX="../../msvc/x86_64" -G "Visual Studio 14 2015 Win64" -DENABLE_HLSL=OFF
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
