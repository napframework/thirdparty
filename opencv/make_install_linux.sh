sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_opencv_world:BOOL="1" -DBUILD_opencv_ts:BOOL="0" -DBUILD_PERF_TESTS:BOOL="0" -DVTK_DIR:PATH="VTK_DIR-NOTFOUND" -DBUILD_JAVA:BOOL="0" -DWITH_VTK:BOOL="0" -DMKL_ROOT_DIR:PATH="MKL_ROOT_DIR-NOTFOUND" -DBUILD_opencv_python_tests:BOOL="0" -DBUILD_opencv_java_bindings_generator:BOOL="0" -DWITH_LAPACK:BOOL="0" -DBUILD_opencv_highgui:BOOL="0" -DWITH_WEBP:BOOL="0" -DBUILD_opencv_python3:BOOL="0" -DBUILD_WEBP:BOOL="0" -DENABLE_FAST_MATH:BOOL="1" -DBUILD_opencv_apps:BOOL="0" -DBUILD_LIST:STRING="" -DWITH_PROTOBUF:BOOL="0" -DBUILD_opencv_python_bindings_generator:BOOL="0" -DMKL_INCLUDE_DIRS:PATH="MKL_ROOT_DIR-NOTFOUND/include" -DWITH_WIN32UI:BOOL="0" -DWITH_GTK:BOOL="0" -DWITH_QT:BOOL="0" -DBUILD_PROTOBUF:BOOL="0" -DWITH_GSTREAMER:BOOL="0" -DWITH_FFMPEG:BOOL="1" -DWITH_OPENCL:BOOL="1" -DCMAKE_INSTALL_PREFIX=../linux .. 
make install
