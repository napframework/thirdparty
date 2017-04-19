@echo off
mkdir msvc64
mkdir msvc32
cmake -H. -Bmsvc64  -G "Visual Studio 14 2015 Win64" -DBUILD_WITH_RTTI=OFF 
cmake -H. -Bmsvc32  -G "Visual Studio 14 2015" -DBUILD_WITH_RTTI=OFF
