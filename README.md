<p align="center">
  <img width=256 height=256 src="https://www.napframework.com/png/nap_logo_small.png">
</p>

This repository contains all cross-platform third party dependencies to build [NAP](https://github.com/napframework/nap) from source on Windows, Linux and OSX, except QT. The third party master is guaranteed to be in-sync with the current NAP master.

- For NAP compilation instructions vist: https://github.com/napframework/nap

## Third-Party Compilation

Linkage should be dynamic and not violate the MPL2.0 license policy. Static linkage is discouraged unless recommended by the third-party library or when a NAP application, that uses the module, doesn't require the library to link and run. In that case all third-party code is compiled into the module when NAP is packaged. Third-party dependencies must work cross-platform and must be compiled using:
```
Clang targeting OSX 10.9	MacOS		
MSVC, VS2015x64				Windows10
GCC <= 9.3.0 x86-64			Linux		
```

Leave platform specific compilation instructions in the form of a `NAP_NOTES.txt`
```
Built from SDL2-2.0.9.tar.gz

Configure flags:
./configure --prefix=`pwd`/osx/install --without-x --enable-static=no

Afterwards:
install_name_tool -id @rpath/libSDL2-2.0.0.dylib libSDL2-2.0.0.dylib
```

Keep the third-party source code in the repository if possible.

