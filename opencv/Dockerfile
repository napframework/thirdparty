ARG inst_dir="/tmp/out"
ARG image_name
FROM nap-${image_name} AS builder
ARG inst_dir
ARG nap_linux_arch

ENV src_dir="/input"
COPY source ${src_dir}/opencv
COPY ffmpeg ${src_dir}/opencv/ffmpeg
ENV PKG_CONFIG_PATH=${src_dir}/ffmpeg/lib/pkgconfig

RUN \
  mv ${src_dir}/opencv/ffmpeg ${src_dir} && \
  cd ${src_dir}/opencv && \
  mkdir build && \
  cd build && \
  cmake \
    -DCMAKE_BUILD_TYPE=RELEASE \
    -DBUILD_opencv_world:BOOL="1" \
    -DBUILD_JPEG:BOOL="1"  \
    -DBUILD_JASPER=ON  \
    -DBUILD_PNG=ON  \
    -DBUILD_TIFF=ON  \
    -DBUILD_ZLIB=ON  \
    -DBUILD_OPENEXR=ON \ 
    -DBUILD_opencv_ts:BOOL="0" \
    -DBUILD_PERF_TESTS:BOOL="0" \
    -DVTK_DIR:PATH="VTK_DIR-NOTFOUND" \
    -DBUILD_JAVA:BOOL="0" \
    -DWITH_VTK:BOOL="0" \
    -DMKL_ROOT_DIR:PATH="MKL_ROOT_DIR-NOTFOUND" \
    -DBUILD_opencv_python_tests:BOOL="0" \
    -DBUILD_opencv_java_bindings_generator:BOOL="0" \
    -DWITH_LAPACK:BOOL="0" \
    -DBUILD_opencv_highgui:BOOL="0" \
    -DWITH_WEBP:BOOL="0" \
    -DBUILD_opencv_python3:BOOL="0" \
    -DBUILD_WEBP:BOOL="0" \
    -DENABLE_FAST_MATH:BOOL="1" \
    -DBUILD_opencv_apps:BOOL="0" \
    -DBUILD_LIST:STRING="" \
    -DWITH_PROTOBUF:BOOL="0" \
    -DBUILD_opencv_python_bindings_generator:BOOL="0" \
    -DMKL_INCLUDE_DIRS:PATH="MKL_ROOT_DIR-NOTFOUND/include" \
    -DWITH_WIN32UI:BOOL="0" \
    -DWITH_GTK:BOOL="0" \
    -DWITH_QT:BOOL="0" \
    -DBUILD_PROTOBUF:BOOL="0" \
    -DWITH_GSTREAMER:BOOL="0" \
    -DWITH_FFMPEG:BOOL="1" \
    -DWITH_OPENCL:BOOL="1" \
    -DCMAKE_INSTALL_PREFIX=${inst_dir} \
    .. && \
  make install && \
  patchelf --set-rpath \$ORIGIN/.:\$ORIGIN/../../FFmpeg/lib/:\$ORIGIN/../../../../ffmpeg/linux/${nap_linux_arch}/lib/ ${inst_dir}/lib/*opencv*.so && \
  rm -rf ${src_dir}

FROM scratch
ARG inst_dir
COPY --from=builder ${inst_dir} /
