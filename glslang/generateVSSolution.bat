@echo off
mkdir msvc64
cd msvc64
cmake ../ -DCMAKE_INSTALL_PREFIX="../install" -G "Visual Studio 14 2015 Win64" -DENABLE_HLSL=OFF