@echo off
mkdir msvc64
cmake -H. -Bmsvc64  -G "Visual Studio 14 2015 Win64" -DBUILD_WITH_RTTI=OFF
cmake --build msvc64 --target install --config Release
cmake --build msvc64 --target install --config Debug