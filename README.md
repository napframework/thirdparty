<p align="center">
  <img width=384 src="https://docs.nap.tech/content/nap_logo_blue.svg">
</p>

This repository contains all cross-platform third party dependencies to build [NAP](https://github.com/napframework/nap) from source on Windows, Linux and macOS, except Qt. The third party master is guaranteed to be in-sync with the current NAP master.

- For NAP compilation instructions visit: [https://github.com/napframework/nap](https://github.com/napframework/nap)

## Third-Party Compilation

Linkage should be dynamic and not violate the MPL2.0 license policy. Static linkage is discouraged unless recommended by the third-party library or when a NAP application, that uses the module, doesn't require the library to link and run. In that case all third-party code is compiled into the module when NAP is packaged. Third-party dependencies must work cross-platform and must be compiled using:
```
Clang targeting macOS 10.14 (MacOS)		
MSVC, Platform Toolset v142 (Windows10)
GCC 9.3.0 x86-64 (Ubuntu LTS Linux)		
```

Leave platform specific compilation instructions in the form of a `NAP_NOTES.txt` on macOS and Windows. On Linux a Docker Buildx process should be provided via `Dockerfile` and `docker-bake.hcl` instead (see down below).

Example `NAP_NOTES.txt`:
```
Built from SDL2-2.0.12.tar.gz

Configure flags:
./configure --prefix=`pwd`/macos/x86_64 --without-x --enable-static=no

Afterwards:
install_name_tool -id @rpath/libSDL2-2.0.0.dylib libSDL2-2.0.0.dylib
```

Keep the third-party source code in the repository if possible.


## Docker Buildx Process For Linux Cross-Architecture Compilation

Docker Buildx is used to build each package, running eg. `docker buildx bake <arch>` in the package directory, where the architecture is one of `x86_64`, `armhf` or `arm64`. A Docker image for the destination architecture should be created first.

More detailed instructions on the process, including creation of the images, are in the README under `/docker_images`.

Going forward this is the preferred method of compilation on Linux and may later be explored on other platforms.
