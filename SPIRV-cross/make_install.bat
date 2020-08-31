@echo off
mkdir msvc64
cd msvc64
cmake ../ -DCMAKE_INSTALL_PREFIX="../msvc/install" -G "Visual Studio 14 2015 Win64"
cmake --build . --config Release --target install
cmake --build . --config Debug --target install