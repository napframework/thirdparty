export ANDROID_NDK=$NDK_ROOT
export PATH="$ANDROID_NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/:$ANDROID_NDK:$ANDROID_NDK/tools:/usr/local/bin:/usr/bin:/bin:$PATH"
export ARCH="armeabi"
export CFLAGS="-DANDROID -mandroid -fomit-frame-pointer --sysroot $ANDROID_NDK/platforms/android-21/arch-arm64"
export CXXFLAGS="$CFLAGS"
export CC="arm-linux-androideabi-gcc $CFLAGS"
export CXX="arm-linux-androideabi-g++ $CXXFLAGS"
export CXX="arm-linux-androideabi-g++"
export AR="arm-linux-androideabi-ar"
export RANLIB="arm-linux-androideabi-ranlib"
export STRIP="arm-linux-androideabi-strip --strip-unneeded"
export READELF="arm-linux-androideabi-readelf"
export MAKE="make -j4 CROSS_COMPILE_TARGET=yes"

$MAKE